#include "cost.h"
#include "globaloutput.h"
#include "tabu.hpp"
#include "two_opt.h"
#include <QElapsedTimer>
#include <QVector>

Tabu::Tabu(QSharedPointer<const Input> inputData)
{
    _inputData = inputData;
    _solution  = randomPermutation(inputData->getDimension(), 42);

    const auto size = _inputData->getDimension();

    _tabu.resize(size);

    for(auto& v : _tabu)
    {
        v.resize(size);
        for(auto& vv : v)
            vv = 0;
    }
}

QPair<long long, QVector<int>> Tabu::run(int timeLimitMSec, int cadency)
{
    QElapsedTimer t;
    t.start();

    Cost cost(_inputData);
    cost.calculateCost(_solution);

    long long bestCost = cost.getCost();
    long long currentCost = cost.getCost();
    auto bestSolution = QSharedPointer<QVector<int>>::create(*_solution);

    QVector<QPair<long long, QPair<int, int>>> movesCost;

    while(t.elapsed() < timeLimitMSec)
    {
        for(int i=0;i<_tabu.size();++i)
            for(int j=i+1;j<_tabu.size();++j)
                _tabu[i][j] = std::max(0, _tabu[i][j]-1);

        auto nextSolution = QSharedPointer<QVector<int>>::create(*_solution);
        Two_OPT opt(nextSolution->count(), nextSolution);

        movesCost.clear();
        movesCost.reserve(newton2(nextSolution->count()));

        while ((nextSolution = opt.next()) != nullptr)
        {
            auto updatedCost = cost.getUpdatedCost(nextSolution, opt.getI(), opt.getJ());

            movesCost.append({updatedCost - currentCost, {opt.getI(), opt.getJ()}});
        }

        std::sort(movesCost.begin(), movesCost.end(), [](QPair<long long, QPair<int,int>> v, QPair<long long, QPair<int,int>> v2){return v.first > v2.first;});

        for(const auto& mc : movesCost)
        {
            bool moveAllowed = _tabu[mc.second.first][mc.second.second] == 0;
            if(moveAllowed || (!moveAllowed && (mc.first + currentCost) > bestCost))
            {
                _tabu[mc.second.first][mc.second.second] = cadency;
                currentCost = currentCost + mc.first;
                cost.setCost(currentCost);
                std::swap((*_solution)[mc.second.first], (*_solution)[mc.second.second]);
                break;
            }
        }

        if(currentCost > bestCost)
        {
            *bestSolution = *_solution;
            bestCost = currentCost;
        }
    }

    return QPair<long long, QVector<int>>{bestCost, *bestSolution};
}

void Tabu::runAlg(int timeMSec)
{
    run(timeMSec, 10);
}
