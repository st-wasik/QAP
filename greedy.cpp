#include "greedy.h"
#include "two_opt.h"

#include <qelapsedtimer.h>

Greedy::Greedy(QSharedPointer<const Input> inputData, int seed)
{
    _inputData = inputData;
    _solution  = randomPermutation(inputData->getDimension(), seed);
}

QPair<long long, QVector<int>> Greedy::run()
{
    QElapsedTimer t;
    t.start();

    Cost cost(_inputData);
    cost.calculateCost(_solution);
    auto bestCost = cost.getCost();

    auto nextSolution = QSharedPointer<QVector<int>>::create(*_solution);
    Two_OPT opt(nextSolution->count(), nextSolution);

    int neighbor = 0, totalSteps = 0, betterFound = 0;
    long long updatedCost = 0;

    while ((nextSolution = opt.next()) != nullptr)
    {
        neighbor++;
        totalSteps++;

        // qDebug() << QString("Total steps: %0, current neighbor: %1, better solutions found: %2.").arg(totalSteps).arg(neighbor).arg(betterFound);
        // Generuj neighbour z _solution
        // Oblicz updateCost(_solution, neighbour)
        // Jeśli neighbour jest lepszy to _solution = neighbour
        // Jeśli żaden neighbour nie był lepszy improvment = false

        updatedCost = cost.getUpdatedCost(nextSolution, opt.getI(), opt.getJ());

        if(updatedCost < bestCost)
        {
            *_solution = *nextSolution;
            bestCost = updatedCost;
            cost.setCost(updatedCost);
            opt.reset();
            betterFound++;
            neighbor = 0;
            continue;
        }
    }

    auto elapsed = t.elapsed();

    qDebug() << QString(50, '*');
    qDebug() << "Solution:" << *_solution;
    qDebug() << "Best cost:" << bestCost;
    qDebug() << "Total steps:" << totalSteps;
    qDebug() << "Solution improved" << betterFound << "times";
    qDebug() << "Elapsed time:" << elapsed << "msec";
    qDebug() << QString(50, '*');

    return QPair<long long, QVector<int>>{bestCost, *_solution};
}
