#include "cost.h"
#include "globaloutput.hpp"
#include "randomwalk.h"
#include "two_opt.h"

#include <qdatetime.h>
#include <qelapsedtimer.h>

RandomWalk::RandomWalk(QSharedPointer<const Input> inputData, int seed)
{
    _inputData = inputData;
    _solution  = randomPermutation(inputData->getDimension(), seed);
}

QPair<long long, QVector<int>> RandomWalk::run(long timeLimitMSec)
{
    QElapsedTimer t;
    t.start();

    Cost cost(_inputData);
    cost.calculateCost(_solution);
    auto bestCost = cost.getCost();

    auto nextSolution = QSharedPointer<QVector<int>>::create(*_solution);
    Two_OPT opt(nextSolution->count(), nextSolution);

    int betterFound = 0;
    int steps = 0;

    int nbors = newton2(_inputData->getDimension());
    auto initialDateTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    while((QDateTime::currentDateTime().toMSecsSinceEpoch() - initialDateTime) <= timeLimitMSec)
    {
        auto nborIndex = random(nbors);

        steps++;

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

//    qDebug() << QString(50, '*');
//    qDebug() << "Solution:" << *_solution;
//    qDebug() << "Best cost:" << bestCost;
//    qDebug() << "Total steps:" << steps;
//    qDebug() << "Solution improved" << betterFound << "times";
//    qDebug() << "Elapsed time:" << elapsed << "msec";
//    qDebug() << QString(50, '*');

    QStringList row;
    row << _inputData->getFilename() << "RANDOMWALK" << QString::number(bestCost) << QString::number(elapsed) << QString::number(-1) /*jumps*/ << QString::number(steps) /*checked solutions*/;

    GlobalOutput::getInstance().write(row.join(","));

    return QPair<long long, QVector<int>>{bestCost, *_solution};
}

void RandomWalk::runAlg(int timeMSec)
{
    run(timeMSec);
}
