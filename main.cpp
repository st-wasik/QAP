#include <random>
#include "common.h"
#include <QElapsedTimer>
#include <QFile>

#include "two_opt.h"
#include "input.h"
#include "localsearch.h"

void costTest(QSharedPointer<const Input> inputData)
{
    Cost costA(inputData);
    Cost costB(inputData);

    QVector<int> solution;
    solution.resize(26);
    for(int i=0;i<26;++i) solution[i] = i;

    costA.calculateCost(solution);
    costB.calculateCost(solution);

    srand(static_cast<quint32>(time(nullptr)));
    int X=0;
    long long A=0, B=0;

    QElapsedTimer t;
    constexpr auto testsCount = 10000;

    for(int i = 0; i < testsCount; ++i)
    {
        int a = rand() % 26, b = rand() % 26;
        std::swap(solution[a], solution[b]);

        t.start();
        costA.calculateCost(solution);
        A += t.nsecsElapsed();

        t.start();
        costB.updateCost(solution, a, b);
        B += t.nsecsElapsed();

        X += costA.getCost() == costB.getCost();
    }

    qDebug() << "calculate cost time " + QString::number(A / 1000000) + " msec";
    qDebug() << "update cost time " + QString::number(B / 1000000) + " msec";
    qDebug() << "passed tests " + QString::number(X) + "/" + QString::number(testsCount);
}

int main()
{
    auto inputData = QSharedPointer<Input>::create();
    inputData->readFromFile("data/bur26f.dat");

    costTest(inputData);

//    LocalSearch localSearch(inputData);
//    QElapsedTimer elapsedTimer;

//    elapsedTimer.start();
//    localSearch.run();

//    qDebug() << "Elapsed time: " + QString::number(elapsedTimer.nsecsElapsed()) + " nsec";
//    qDebug() << "Elapsed time: " + QString::number(elapsedTimer.elapsed()) + " msec";

    return 0;
}
