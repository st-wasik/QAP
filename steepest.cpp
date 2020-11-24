#include "cost.h"
#include "globaloutput.hpp"
#include "steepest.h"
#include "two_opt.h"

#include <QElapsedTimer>

Steepest::Steepest(QSharedPointer<const Input> inputData, int seed)
{
    _inputData = inputData;
    _solution  = randomPermutation(inputData->getDimension(), seed);
}

QPair<long long, QVector<int> > Steepest::run()
{
    QElapsedTimer t;
    t.start();

    Cost cost(_inputData);
    cost.calculateCost(_solution);
    long long bestCost = cost.getCost();
    const auto initialCost = cost.getCost();

    int neighbor = 0, totalSteps = 0, betterFound = 0;
    bool improvement = true;

//    QStringList row;
//    row << _inputData->getFilename() << "STEEPEST" << QString::number(betterFound) << QString::number(bestCost);

//    GlobalOutput::getInstance().write(row.join(","));

    while(improvement)
    {
        neighbor = 0;
        improvement = false;

        auto nextSolution = QSharedPointer<QVector<int>>::create(*_solution);
        Two_OPT opt(nextSolution->count(), nextSolution);

        while ((nextSolution = opt.next()) != nullptr)
        {
            neighbor++;
            totalSteps++;

//             qDebug() << QString("Total steps: %0, current neighbor: %1, better solutions found: %2, current cost: %3.").arg(totalSteps).arg(neighbor).arg(betterFound).arg(bestCost);

            // Generuj neighbour z _solution
            // Oblicz updateCost(_solution, neighbour)
            // Jeśli neighbour jest lepszy to _solution = neighbour
            // Jeśli żaden neighbour nie był lepszy improvment = false

            auto updatedCost = cost.getUpdatedCost(nextSolution, opt.getI(), opt.getJ());

            if(updatedCost < bestCost)
            {
                *_solution = *nextSolution;
                bestCost = updatedCost;
                betterFound++;
                improvement = true;

//                QStringList row;
//                row << _inputData->getFilename() << "STEEPEST" << QString::number(betterFound) << QString::number(bestCost);

//                GlobalOutput::getInstance().write(row.join(","));
            }
        }

        cost.setCost(bestCost);
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
//    row << _inputData->getFilename() << "STEEPEST" << QString::number(bestCost) << QString::number(elapsed) << QString::number(betterFound) /*jumps*/ << QString::number(totalSteps) /*checked solutions*/ << QString::number(initialCost);
    row << _inputData->getFilename() << "STEEPEST" << QString::number(bestCost) << vectorToString(*_solution );

    GlobalOutput::getInstance().write(row.join(","));

    return QPair<long long, QVector<int>>{bestCost, *_solution};
}

void Steepest::runAlg(int timeMSec)
{
    Q_UNUSED(timeMSec)

    run();
}
