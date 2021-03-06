#include "annealing.h"
#include "cost.h"
#include "globaloutput.h"
#include "two_opt.h"
#include <QElapsedTimer>
#include <QVector>
#include <math.h>

#define STEP_CONSTANT 1.2
#define MINIMAL_TEMP 0.01
#define ALPHA 0.9
#define STOP_CONSTANT 10
#define LANDSCAPE_SCAN_SIZE 10000
#define P 0.99

Annealing::Annealing(QSharedPointer<const Input> inputData, int seed) {
    _inputData = inputData;
    _solution = randomPermutation(inputData->getDimension(), seed);
    _initialTemp = _getInitialTemp();
    _stepLenght = _getStepLenght(inputData->getDimension());  
}

QPair<long long, QVector<int>> Annealing::run() {   
    QElapsedTimer t;
    t.start();

    Cost cost(_inputData);
    cost.calculateCost(_solution);

    int jumps=0, checkedSols=0;

    auto nextSolution = QSharedPointer<QVector<int>>::create(*_solution);
    QSharedPointer<QVector<int>> tempSol;
    Two_OPT opt(nextSolution->count(), nextSolution);

    const long long initCost = cost.getCost();
    long long currentCost = cost.getCost();

    long long updatedCost = 0;

    float temperature = _initialTemp;
    int noImprovemnt = 0;

    long long allTimeBestCost = currentCost;

    while (temperature > MINIMAL_TEMP) {

        for (int i=0; i < _stepLenght; i++) {
            // Generate neighbours for whole temperature step
            tempSol = opt.next(false);
            if (tempSol == nullptr) {
                opt.reset();
                tempSol = opt.next(false);
            }
            bool swapBack = true;

            *nextSolution = *tempSol;

            checkedSols++;

            updatedCost = cost.getUpdatedCost(tempSol, opt.getI(), opt.getJ());

            // If neighbour is better then move to it
            if (updatedCost < currentCost) {

                if (updatedCost < allTimeBestCost) allTimeBestCost = updatedCost;

                *_solution = *nextSolution;
                currentCost = updatedCost;
                cost.setCost(updatedCost);
                opt.random();

                noImprovemnt = 0;
                jumps++;
                continue;
            }

            // If neighbour is worse then there is a chance to move to it
            double excitation = exp(-(abs(updatedCost - currentCost))/temperature);
            if (excitation > ((double) rand() / RAND_MAX)) {              
                *_solution = *nextSolution;
                currentCost = updatedCost;
                cost.setCost(updatedCost);
                jumps++;
                opt.random();
                swapBack = false;
            }
            // If we did not move to neighbour then swap back
            if (swapBack) { opt.doSwapBack(); }

            // If you reached this code than there was no improvement
            if (++noImprovemnt >= STOP_CONSTANT * _stepLenght) break;
        }

        // Early stopping
        if (noImprovemnt >= STOP_CONSTANT * _stepLenght) { break; }

        // Annealing
        temperature *= ALPHA;
    }

    _AllTimeBestCost = allTimeBestCost;

    auto elapsed = t.elapsed();

    QStringList row;
    row << _inputData->getFilename() << "ANNEALING" << QString::number(currentCost) << QString::number(elapsed) << QString::number(jumps) /*jumps*/ << QString::number(checkedSols) /*checked solutions*/ << QString::number(initCost);
    GlobalOutput::getInstance().write(row.join(","));

    return QPair<long long, QVector<int>>{currentCost, *_solution};
}

float Annealing::_getInitialTemp() {
    float delta = _scanLandscape();
    return -(delta/log(P));
}

int Annealing::_getStepLenght(int n) {
//    return (int)(pow(n, 2) * STEP_CONSTANT);
    return (int)(((n*(n-1))/2) * STEP_CONSTANT);
}

void Annealing::runAlg(int timeMSec)
{
    run();
}

float Annealing::_scanLandscape() {
    float sum = 0;
    int dim = _inputData->getDimension();
    Cost cost(_inputData);

    long long neighbourCost = 0;
    QSharedPointer<QVector<int>> randomSolution = randomPermutation(dim);
    Two_OPT opt(randomSolution->count(), randomSolution);

    // Probe landscape LANDSCAPE_SCAN_SIZE times
    for (int i=0; i < LANDSCAPE_SCAN_SIZE; i++) {
        randomSolution = randomPermutation(dim);
        cost.calculateCost(randomSolution);
        float solutionCost = cost.getCost();

        opt.random();
        auto neighbour = opt.next(false);
        neighbourCost = cost.getUpdatedCost(neighbour, opt.getI(), opt.getJ());

        sum += abs(solutionCost - neighbourCost);
    }

    return sum / LANDSCAPE_SCAN_SIZE;
}

long long Annealing::getBestCost() { return _AllTimeBestCost; }
