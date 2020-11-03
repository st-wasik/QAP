#include "cost.h"
#include "heuristic.h"
#include "two_opt.h"

#include <qelapsedtimer.h>

Heuristic::Heuristic(QSharedPointer<const Input> inputData, int seed)
{
    _inputData = inputData;
    _solution  = QSharedPointer<QVector<int>>::create(inputData->getDimension(), -1);
}

QPair<long long, QVector<int>> Heuristic::run(bool distancesDesc)
{
    QElapsedTimer t;
    t.start();

    QMultiMap<int, QPair<int,int>> distancesValuesToIndex;
    auto rowsCount = _inputData->distances->count();
    for(int i=0;i<rowsCount;++i)
    {
        const auto& row = *_inputData->distances;
        const auto  rowSize = row.count();
        for(int j=0;j<rowSize;++j)
        {
            distancesValuesToIndex.insert(row[i][j], {i,j});
        }
    }

    auto distancesKeys = distancesValuesToIndex.keys();
    std::sort(distancesKeys.begin(), distancesKeys.end());
    distancesKeys.erase(std::unique(distancesKeys.begin(), distancesKeys.end() ), distancesKeys.end());

    if(distancesDesc)
    {
        std::sort(distancesKeys.rbegin(), distancesKeys.rend());
    }

    QVector<QPair<int,int>> distancesIdx;
    for(const auto& key : distancesKeys)
    {
        for(const auto& val : distancesValuesToIndex.values(key))
        {
            distancesIdx << val;
        }
    }

    QMultiMap<int, QPair<int,int>> interactionsValuesToIndex;
    rowsCount = _inputData->interactions->count();
    for(int i=0;i<rowsCount;++i)
    {
        const auto& row = *_inputData->interactions;
        const auto  rowSize = row.count();
        for(int j=0;j<rowSize;++j)
        {
            interactionsValuesToIndex.insert(row[i][j], {i,j});
        }
    }

    auto interactionsKeys = interactionsValuesToIndex.keys();
    std::sort(interactionsKeys.begin(), interactionsKeys.end());
    interactionsKeys.erase(std::unique(interactionsKeys.begin(), interactionsKeys.end() ), interactionsKeys.end());

    if(!distancesDesc)
    {
        std::sort(interactionsKeys.rbegin(), interactionsKeys.rend());
    }

    QVector<QPair<int,int>> interactionsIdx;
    for(const auto& key : interactionsKeys)
    {
        for(const auto& val : interactionsValuesToIndex.values(key))
        {
            interactionsIdx << val;
        }
    }

    int foundPermutationElementsCount = 0;
    QSet<int> usedDistIdx;

    for(const auto& interactionIdx : interactionsIdx)
    {
        if(foundPermutationElementsCount == _inputData->getDimension())
            break;

        for(const auto& distanceIdx : distancesIdx)
        {
            if(isMappingAvailable(_solution, interactionIdx, distanceIdx)
                && !usedDistIdx.contains(distanceIdx.first)
                && !usedDistIdx.contains(distanceIdx.second))
            {
                (*_solution)[interactionIdx.first] = distanceIdx.first;
                (*_solution)[interactionIdx.second] = distanceIdx.second;

                usedDistIdx.insert(distanceIdx.first);
                usedDistIdx.insert(distanceIdx.second);

                foundPermutationElementsCount++;
                break;
            }
        }
    }

    auto elapsed = t.elapsed();

    Cost cost(_inputData);
    cost.calculateCost(_solution);
    auto bestCost = cost.getCost();

    qDebug() << QString(50, '*');
    qDebug() << "Solution:" << *_solution;
    qDebug() << "Best cost:" << bestCost;
//    qDebug() << "Total steps:" << totalSteps;
//    qDebug() << "Solution improved" << betterFound << "times";
    qDebug() << "Elapsed time:" << elapsed << "msec";
    qDebug() << QString(50, '*');

    return QPair<long long, QVector<int>>{bestCost, *_solution};
}

bool Heuristic::isMappingAvailable(QSharedPointer<QVector<int> > solution, QPair<int, int> interactIndex, QPair<int, int> distIndex)
{
    if(interactIndex.first == interactIndex.second)
    {
        if(distIndex.first == distIndex.second)
        {
            if((*solution)[interactIndex.first] == -1)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else // (interactIndex.first != interactIndex.second)
    {
        if(distIndex.first != distIndex.second)
        {
            if((*solution)[interactIndex.first] == -1 && (*solution)[interactIndex.second] == -1)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
}
