#include "annealing.h"
#include "cost.h"
#include "globaloutput.h"
#include "two_opt.h"
#include <QElapsedTimer>
#include <QVector>
#include <math.h>

#define STEP_CONSTANT 1
#define MINIMAL_TEMP 0.01
#define ALPHA 0.9
#define STOP_CONSTANT 10
#define LANDSCAPE_SCAN_SIZE 1000

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

    long long currentCost = cost.getCost();

    auto nextSolution = QSharedPointer<QVector<int>>::create(*_solution);
    Two_OPT opt(nextSolution->count(), nextSolution);

    long long updatedCost = 0;

    float temperature = _initialTemp;
    int noImprovemnt = 0;

    while (temperature > MINIMAL_TEMP) {
        for (int i=0; i < _stepLenght; i++) {
            updatedCost = cost.getUpdatedCost(nextSolution, opt.getI(), opt.getJ());

            // If neighbour is better than move to it
            if (updatedCost < currentCost) {
                *_solution = *nextSolution;
                currentCost = updatedCost;
                cost.setCost(updatedCost);
                opt.reset();

                noImprovemnt = 0;
                continue;
            }

            // If neighbour is worse than there is chance to move to it
            double excitation = exp((currentCost - updatedCost)/temperature);
            if (excitation > ((double) rand() / RAND_MAX) + 1) {
                *_solution = *nextSolution;
                currentCost = updatedCost;
                cost.setCost(updatedCost);
                opt.reset();
            }

            // If you reached this code than there was no improvement
            if (++noImprovemnt >= STOP_CONSTANT * _stepLenght) break;
        }

        // Early stopping
        if (noImprovemnt >= STOP_CONSTANT * _stepLenght) break;

        // Annealing
        temperature *= ALPHA;
    }

    auto elapsed = t.elapsed();
    QStringList row;
    row << _inputData->getFilename() << "ANNEALING" << QString::number(currentCost) << vectorToString(*_solution);
    GlobalOutput::getInstance().write(row.join(","));

    return QPair<long long, QVector<int>>{currentCost, *_solution};
}

float Annealing::_getInitialTemp() {
    float delta = _scanLandscape();
    // temp = -(delta/log(0.99));
    // assert(temp > 0.0);
    // return temp;
    return 20.0;
}

int Annealing::_getStepLenght(int n) {
    return (int)(pow(n, 2) * STEP_CONSTANT);
}

float Annealing::_scanLandscape() {
    float sum = 0;
    int dim = _inputData->getDimension();
    Cost cost(_inputData);

    long long updatedCost = 0;
    QSharedPointer<QVector<int>> randomSolution;

    // Probe landscape LANDSCAPE_SCAN_SIZE times
    for (int i=0; i < LANDSCAPE_SCAN_SIZE; i++) {
        randomSolution = randomPermutation(dim);
        cost.calculateCost(randomSolution);
        float solutionCost = cost.getCost();

        // Neighbour cost
        // sum += abs(neigbourCost - solutionCost)
    }

    return sum;
}
