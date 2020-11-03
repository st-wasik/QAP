#include <random>
#include "common.h"
#include <QElapsedTimer>
#include <QFile>
#include <QSet>

#include "two_opt.h"
#include "input.h"
#include "greedy.h"
#include "steepest.h"
#include "randomwalk.h"
#include "heuristic.h"
#include "random.h"

void costTest(QSharedPointer<const Input> inputData)
{
    Cost costA(inputData);
    Cost costB(inputData);

    auto solution = QSharedPointer<QVector<int>>::create();
    solution->resize(inputData->getDimension());
    for(int i=0;i<inputData->getDimension();++i) (*solution)[i] = i;

    costA.calculateCost(solution);
    costB.calculateCost(solution);

    srand(static_cast<quint32>(time(nullptr)));
    int X=0;
    long long A=0, B=0;

    QElapsedTimer t;
    constexpr auto testsCount = 15000;

    for(int i = 0; i < testsCount; ++i)
    {
        int a = rand() % inputData->getDimension(), b = rand() % inputData->getDimension();
        std::swap((*solution)[a], (*solution)[b]);

        t.start();
        costA.calculateCost(solution);
        A += t.nsecsElapsed();

        t.start();
        costB.setCost(costB.getUpdatedCost(solution, a, b));
        B += t.nsecsElapsed();

        X += costA.getCost() == costB.getCost();
    }

    qDebug() << "calculate cost time " + QString::number(A / 1000000) + " msec";
    qDebug() << "update cost time " + QString::number(B / 1000000) + " msec";
    qDebug() << "passed tests " + QString::number(X) + "/" + QString::number(testsCount);
}

void twoOptTest()
{
    auto perm = QSharedPointer<QVector<int>>::create(QVector<int>{1,2,3});
    Two_OPT opt(3, perm);

    QSharedPointer<QVector<int>> next;
    while( (next = opt.next()) != nullptr )
    {
        qDebug() << *next;
    }
}

void greedyTest(QSharedPointer<const Input> inputData)
{
    srand(static_cast<quint32>(time(nullptr)));

    constexpr auto steps = 125;

    auto greedy = QSharedPointer<Greedy>::create(inputData, rand());
    auto best = greedy->run();

    for(int i=1;i<steps;i++)
    {
        greedy = QSharedPointer<Greedy>::create(inputData, rand());

        auto result = greedy->run();

        if(result.first < best.first)
        {
            best.first = result.first;
            best.second = result.second;
        }
    }

    qDebug() << "";
    qDebug() << QString(50, '*');
    qDebug() << "BEST SOLUTION";
    qDebug() << "Solution:" << best.second;
    qDebug() << "Best cost" << best.first;
    qDebug() << QString(50, '*');
}

void steepestTest(QSharedPointer<const Input> inputData)
{
    srand(static_cast<quint32>(time(nullptr)));

    constexpr auto steps = 125;

    auto steepest = QSharedPointer<Steepest>::create(inputData, rand());
    auto best = steepest->run();

    for(int i=1;i<steps;i++)
    {
        steepest = QSharedPointer<Steepest>::create(inputData, rand());

        auto result = steepest->run();

        if(result.first < best.first)
        {
            best.first = result.first;
            best.second = result.second;
        }
    }

    qDebug() << "";
    qDebug() << QString(50, '*');
    qDebug() << "BEST SOLUTION";
    qDebug() << "Solution:" << best.second;
    qDebug() << "Best cost" << best.first;
    qDebug() << QString(50, '*');
}

void randomWalkTest(QSharedPointer<const Input> inputData)
{
    srand(static_cast<quint32>(time(nullptr)));

    constexpr auto steps = 125;

    constexpr auto rwalkSteps = 25000;

    auto rwalk = QSharedPointer<RandomWalk>::create(inputData, rand());
    auto best = rwalk->run(rwalkSteps);

    for(int i=1;i<steps;i++)
    {
        rwalk = QSharedPointer<RandomWalk>::create(inputData, rand());

        auto result = rwalk->run(rwalkSteps);

        if(result.first < best.first)
        {
            best.first = result.first;
            best.second = result.second;
        }
    }

    qDebug() << "";
    qDebug() << QString(50, '*');
    qDebug() << "BEST SOLUTION";
    qDebug() << "Solution:" << best.second;
    qDebug() << "Best cost" << best.first;
    qDebug() << QString(50, '*');
}

void heuristicTest(QSharedPointer<const Input> inputData)
{
    srand(static_cast<quint32>(time(nullptr)));

    auto heuristic = QSharedPointer<Heuristic>::create(inputData, rand());
    auto best = heuristic->run(false);

    heuristic = QSharedPointer<Heuristic>::create(inputData, rand());
    auto result = heuristic->run(true);

    qDebug() << "First all unique:" << (QSet<int>::fromList(QVector<int>(best.second).toList()).count() == best.second.count());
    qDebug() << "Second all unique:" << (QSet<int>::fromList(QVector<int>(result.second).toList()).count() == result.second.count());

    if(result.first < best.first)
    {
        best.first = result.first;
        best.second = result.second;
    }

    qDebug() << "";
    qDebug() << QString(50, '*');
    qDebug() << "BEST SOLUTION";
    qDebug() << "Solution:" << best.second;
    qDebug() << "Best cost" << best.first;
    qDebug() << QString(50, '*');
}


void randomTest(QSharedPointer<const Input> inputData)
{
    srand(static_cast<quint32>(time(nullptr)));

    constexpr auto steps = 125;

    constexpr auto randomTimeMSec = 50;

    auto random = QSharedPointer<Random>::create(inputData, rand());
    auto best = random->run(randomTimeMSec);

    for(int i=1;i<steps;i++)
    {
        random = QSharedPointer<Random>::create(inputData, rand());

        auto result = random->run(randomTimeMSec);

        if(result.first < best.first)
        {
            best.first = result.first;
            best.second = result.second;
        }
    }

    qDebug() << "";
    qDebug() << QString(50, '*');
    qDebug() << "BEST SOLUTION";
    qDebug() << "Solution:" << best.second;
    qDebug() << "Best cost" << best.first;
    qDebug() << QString(50, '*');
}

int main()
{
    auto inputData = QSharedPointer<Input>::create();
    inputData->readFromFile("data/bur26a.dat");

    Matrix A{{1,2,3,5},{4,5,6,2},{7,8,9,1},{5,5,2,7}};
    Matrix B{{5,3,1,55},{2,98,6,54},{38,66,1,12},{5,31,2,2}};
    auto inputData2 = QSharedPointer<Input>::create(A,B);

//    costTest(inputData);

//    twoOptTest();

//    greedyTest(inputData);

//    steepestTest(inputData2);

//    randomWalkTest(inputData2);

//    heuristicTest(inputData);

    randomTest(inputData);

    return 0;
}
