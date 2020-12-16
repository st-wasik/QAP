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

    long long bestCost = cost.getCost();
    long long currentCost = cost.getCost();
    auto bestSolution = QSharedPointer<QVector<int>>::create(*_solution);

    QVector<QPair<long long, QPair<int, int>>> movesCost;

    int stepsWithoutBetterSolution = 0;

    while(stepsWithoutBetterSolution < limit)
    {
//        for(int i=0;i<_tabu.size();++i)
//            for(int j=i+1;j<_tabu.size();++j)
//                _tabu[i][j] = std::max(0, _tabu[i][j]-1);

        auto nextSolution = QSharedPointer<QVector<int>>::create(*_solution);
        Two_OPT opt(nextSolution->count(), nextSolution);

        movesCost.clear();
        movesCost.reserve(newton2(nextSolution->count()));

        while ((nextSolution = opt.next()) != nullptr)
        {
            auto updatedCost = cost.getUpdatedCost(nextSolution, opt.getI(), opt.getJ());

            movesCost.append({updatedCost - currentCost, {opt.getI(), opt.getJ()}});
        }

        std::sort(movesCost.begin(), movesCost.end(), [](QPair<long long, QPair<int,int>> v, QPair<long long, QPair<int,int>> v2){return v.first < v2.first;});

        const auto n = movesCost.size()/10;
        for(int i=0; i < n; ++i)
        {
            const auto mc = movesCost[i];

            std::swap((*_solution)[mc.second.first], (*_solution)[mc.second.second]);
//            bool moveAllowed = _tabu[mc.second.first][mc.second.second] == 0;
            bool moveAllowed = !_tabuQueue->contains(*_solution);
                        std::swap((*_solution)[mc.second.first], (*_solution)[mc.second.second]);

            if(moveAllowed || (!moveAllowed && (mc.first + currentCost) < bestCost) || i == (n-1))
            {
                std::swap((*_solution)[mc.second.first], (*_solution)[mc.second.second]);
                _tabuQueue->append((*_solution));
//                _tabu[mc.second.first][mc.second.second] = cadency;
                currentCost = currentCost + mc.first;
                cost.setCost(currentCost);
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

    return QPair<long long, QVector<int>>{bestCost, *bestSolution};
}

void Tabu::runAlg(int timeMSec)
{
    Q_UNUSED(timeMSec)

    run(10);
}
