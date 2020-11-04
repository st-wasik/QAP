#include <random>
#include <QElapsedTimer>
#include <QFile>
#include <QMutex>
#include <QSet>
#include <QFuture>
#include <QtConcurrent/QtConcurrentRun>

#include "common.h"
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
    constexpr auto vectorSize = 150;
    auto perm = QSharedPointer<QVector<int>>::create(vectorSize);
    std::iota(perm->begin(), perm->end(), 0);
    Two_OPT opt(perm->count(), perm);

    QElapsedTimer t;
    t.start();
    auto i = 0;
    QSharedPointer<QVector<int>> next;
    while( (next = opt.next()) != nullptr )
    {
        i++;
//        qDebug() << *next;
    }
    qDebug() << "Elapsed" << t.elapsed() << "msec" << "   Generated perms:" << i;
}

void greedyTest(QSharedPointer<const Input> inputData)
{
    QElapsedTimer t;
    t.start();

    srand(static_cast<quint32>(time(nullptr)));
    randomPermutation(0, rand()); // set seed

    constexpr auto attempts = 5000;

    QMutex mutex;
    auto best = QSharedPointer<Greedy>::create(inputData)->run();

    QVector<QFuture<void>> futures;

    for(int i=1;i<attempts;i++)
    {
        futures << QtConcurrent::run([&,i]
        {
            qDebug() << "Running instance no." << i;

            auto greedy = QSharedPointer<Greedy>::create(inputData);
            auto result = greedy->run();

            qDebug() << "Instance" << i << "finished";

            QMutexLocker lock(&mutex);
            if(result.first < best.first)
            {
                best.first = result.first;
                best.second = result.second;
            }
        });
    }

    for(auto& future : futures)
        future.waitForFinished();

    qDebug() << "";
    qDebug() << QString(50, '*');
    qDebug() << "BEST SOLUTION";
    qDebug() << "Solution:" << best.second;
    qDebug() << "Best cost" << best.first;
    qDebug() << QString(50, '*');
    qDebug() << "Total time" << t.elapsed() << "msec";
}

void steepestTest(QSharedPointer<const Input> inputData)
{
    srand(static_cast<quint32>(time(nullptr)));

    constexpr auto attempts = 125;

    auto steepest = QSharedPointer<Steepest>::create(inputData, rand());
    auto best = steepest->run();

    for(int i=1;i<attempts;i++)
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

    constexpr auto attempts = 125;

    constexpr auto rwalkTimeMSec = 250;

    auto rwalk = QSharedPointer<RandomWalk>::create(inputData, rand());
    auto best = rwalk->run(rwalkTimeMSec);

    for(int i=1;i<attempts;i++)
    {
        rwalk = QSharedPointer<RandomWalk>::create(inputData, rand());

        auto result = rwalk->run(rwalkTimeMSec);

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
    QElapsedTimer t;
    t.start();

    srand(static_cast<quint32>(time(nullptr)));
    randomPermutation(0, rand()); // set seed

    constexpr auto attempts = 200;

    QMutex mutex;
    auto best = QSharedPointer<Heuristic>::create(inputData)->run();

    QVector<QFuture<void>> futures;

    std::mt19937 randGenerator(rand());

    for(int i=1;i<attempts;i++)
    {
        futures << QtConcurrent::run([&,i]
        {
            qDebug() << "Running instance no." << i;

            const auto size = inputData->getDimension();
            auto initialSolution = QSharedPointer<QVector<int>>::create(size, -1);
            (*initialSolution)[std::abs(static_cast<int>(randGenerator())) % size] = std::abs(static_cast<int>(randGenerator())) % size;

            const bool distancesDesc = static_cast<int>(randGenerator()) % 2;
            auto result = QSharedPointer<Heuristic>::create(inputData, initialSolution)->run(distancesDesc);

            qDebug() << "Instance" << i << "finished";

            QMutexLocker lock(&mutex);
            if(result.first < best.first)
            {
                best.first = result.first;
                best.second = result.second;
            }
        });
    }

    for(auto& future : futures)
        future.waitForFinished();

    qDebug() << "";
    qDebug() << QString(50, '*');
    qDebug() << "BEST SOLUTION";
    qDebug() << "Solution:" << best.second;
    qDebug() << "Best cost" << best.first;
    qDebug() << QString(50, '*');
    qDebug() << "Total time" << t.elapsed() << "msec";
}

void randomTest(QSharedPointer<const Input> inputData)
{
    srand(static_cast<quint32>(time(nullptr)));

    constexpr auto attempts = 125;

    constexpr auto randomTimeMSec = 50;

    auto random = QSharedPointer<Random>::create(inputData, rand());
    auto best = random->run(randomTimeMSec);

    for(int i=1;i<attempts;i++)
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
    inputData->readFromFile("cleanedData/lipa80a.dat2");

    Matrix A{{1,2,3,5},{4,5,6,2},{7,8,9,1},{5,5,2,7}};
    Matrix B{{5,3,1,55},{2,98,6,54},{38,66,1,12},{5,31,2,2}};
    auto inputData2 = QSharedPointer<Input>::create(A,B);

//    costTest(inputData);

//    twoOptTest();

//    greedyTest(inputData);

//    steepestTest(inputData2);

//    randomWalkTest(inputData);

    heuristicTest(inputData2);

//    randomTest(inputData);

    return 0;
}
