#include "cost.h"
#include "globaloutput.h"
#include "random.h"
#include "two_opt.h"

#include <QTimer>
#include <qelapsedtimer.h>

Random::Random(QSharedPointer<const Input> inputData, int seed)
{
    _inputData = inputData;
    _solution  = randomPermutation(inputData->getDimension(), seed);
}

QPair<long long, QVector<int>> Random::run(long timeLimitMSec)
{
    QElapsedTimer t;
    t.start();

    Cost cost(_inputData);
    cost.calculateCost(_solution);
    auto bestCost = cost.getCost();

    long long updatedCost = 0;
    QSharedPointer<QVector<int>> nextSolution;

    int totalSteps = 0, betterFound = 0;
    auto initialDateTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    while ((QDateTime::currentDateTime().toMSecsSinceEpoch() - initialDateTime) <= timeLimitMSec)
    {
        totalSteps++;

        nextSolution = randomPermutation(_inputData->getDimension());
        cost.calculateCost(nextSolution);
        updatedCost = cost.getCost();

        if(updatedCost < bestCost)
        {
            *_solution = *nextSolution;
            bestCost = updatedCost;
            betterFound++;
            continue;
        }
    }

    auto elapsed = t.elapsed();

//    qDebug() << QString(50, '*');
//    qDebug() << "Solution:" << *_solution;
//    qDebug() << "Best cost:" << bestCost;
//    qDebug() << "Total steps:" << totalSteps;
//    qDebug() << "Solution improved" << betterFound << "times";
//    qDebug() << "Elapsed time:" << elapsed << "msec";
//    qDebug() << QString(50, '*');

    QStringList row;
    row << _inputData->getFilename() << "RANDOM" << QString::number(bestCost) << QString::number(elapsed) << QString::number(-1) /*jumps*/ << QString::number(totalSteps) /*checked solutions*/ << QString::number(-1);

    GlobalOutput::getInstance().write(row.join(","));

    return QPair<long long, QVector<int>>{bestCost, *_solution};
}

void Random::runAlg(int timeMSec)
{
    run(timeMSec);
}

