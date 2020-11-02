#include "cost.h"
#include "randomwalk.h"
#include "two_opt.h"

#include <qelapsedtimer.h>

RandomWalk::RandomWalk(QSharedPointer<const Input> inputData, int seed)
{
    _inputData = inputData;
    _solution  = randomPermutation(inputData->getDimension(), seed);
}

QPair<long long, QVector<int>> RandomWalk::run(int steps)
{
    QElapsedTimer t;
    t.start();

    Cost cost(_inputData);
    cost.calculateCost(_solution);
    auto bestCost = cost.getCost();

    auto nextSolution = QSharedPointer<QVector<int>>::create(*_solution);
    Two_OPT opt(nextSolution->count(), nextSolution);

    int betterFound = 0;

    int nbors = newton2(_inputData->getDimension());

    for(int i=0; i<steps; ++i)
    {
        auto nborIndex = random(nbors);

        for(int j=0; j<=nborIndex;++j)
        {
            nextSolution = opt.next();
        }

        auto updatedCost = cost.getUpdatedCost(nextSolution, opt.getI(), opt.getJ());
        cost.setCost(updatedCost);
        opt.reset();

        if(updatedCost < bestCost)
        {
            *_solution = *nextSolution;
            bestCost = updatedCost;
            betterFound++;
        }
    }

    auto elapsed = t.elapsed();

    qDebug() << QString(50, '*');
    qDebug() << "Solution:" << *_solution;
    qDebug() << "Best cost:" << bestCost;
    qDebug() << "Total steps:" << steps;
    qDebug() << "Solution improved" << betterFound << "times";
    qDebug() << "Elapsed time:" << elapsed << "msec";
    qDebug() << QString(50, '*');

    return QPair<long long, QVector<int>>{bestCost, *_solution};
}
