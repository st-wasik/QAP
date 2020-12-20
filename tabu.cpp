#include "cost.h"
#include "globaloutput.h"
#include "tabu.hpp"
#include "two_opt.h"
#include <QElapsedTimer>
#include <QVector>

Tabu::Tabu(QSharedPointer<const Input> inputData, int seed)
{
    _inputData = inputData;
    _solution  = randomPermutation(inputData->getDimension(), seed);

    const auto size = _inputData->getDimension();

    _tabuQueue = QSharedPointer<RingQueue<QVector<int>>>::create(_inputData->getDimension() / 4);

//    _tabu.resize(size);

//    for(auto& v : _tabu)
//    {
//        v.resize(size);
//        for(auto& vv : v)
//            vv = 0;
//    }
}

QPair<long long, QVector<int>> Tabu::run(int limit)
{
    QElapsedTimer t;
    t.start();

    Cost cost(_inputData);
    cost.calculateCost(_solution);

    const long long initialCost = cost.getCost();
    long long bestCost = cost.getCost();
    long long currentCost = cost.getCost();
    auto bestSolution = QSharedPointer<QVector<int>>::create(*_solution);

    int jumps = 0;
    int checkedSols = 0;

    QVector<QPair<long long, QPair<int, int>>> movesCost;

    int stepsWithoutBetterSolution = 0;

    auto nextSolution = QSharedPointer<QVector<int>>::create(*_solution);
    QSharedPointer<QVector<int>> tempSol;
    Two_OPT opt(nextSolution->count(), nextSolution);

    while(stepsWithoutBetterSolution < limit)
    {
        movesCost.clear();
        movesCost.reserve(newton2(nextSolution->count()));

        while ((tempSol = opt.next()) != nullptr)
        {
            auto updatedCost = cost.getUpdatedCost(tempSol, opt.getI(), opt.getJ());

            movesCost.append({updatedCost - currentCost, {opt.getI(), opt.getJ()}});
        }

        std::sort(movesCost.begin(), movesCost.end(), [](QPair<long long, QPair<int,int>> v, QPair<long long, QPair<int,int>> v2){return v.first < v2.first;});

        checkedSols+=movesCost.count();

        const auto n = std::max(1, _inputData->getDimension()/10);
        for(int i=0; i < n; ++i)
        {
            const auto mc = movesCost[i];

            std::swap((*_solution)[mc.second.first], (*_solution)[mc.second.second]);
            bool moveAllowed = !_tabuQueue->contains(*_solution);
                        std::swap((*_solution)[mc.second.first], (*_solution)[mc.second.second]);

            if(moveAllowed || (!moveAllowed && (mc.first + currentCost) < bestCost))
            {
                std::swap((*_solution)[mc.second.first], (*_solution)[mc.second.second]);
                *nextSolution = *_solution;
                opt.reset();
                _tabuQueue->append((*_solution));
                currentCost = currentCost + mc.first;
                cost.setCost(currentCost);
                jumps++;
                break;
            }
            else if(i == (n-1))
            {
                const auto mc = movesCost[0];
                std::swap((*_solution)[mc.second.first], (*_solution)[mc.second.second]);
                *nextSolution = *_solution;
                opt.reset();
                _tabuQueue->append((*_solution));
                currentCost = currentCost + mc.first;
                cost.setCost(currentCost);
                jumps++;
                break;
            }
        }

        if(currentCost < bestCost)
        {
            stepsWithoutBetterSolution = 0;
            *bestSolution = *_solution;
            bestCost = currentCost;
        }
        else
        {
            stepsWithoutBetterSolution++;
        }
    }

    const auto elapsed = t.elapsed();

    QStringList row;
    row << _inputData->getFilename() << "TABU" << QString::number(bestCost) << QString::number(elapsed) << QString::number(jumps) /*jumps*/ << QString::number(checkedSols) /*checked solutions*/ << QString::number(initialCost);

    GlobalOutput::getInstance().write(row.join(","));

    return QPair<long long, QVector<int>>{bestCost, *bestSolution};
}

void Tabu::runAlg(int timeMSec)
{
    Q_UNUSED(timeMSec)

    run(10);
}
