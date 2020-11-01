#include <random>
#include "common.h"
#include <QElapsedTimer>
#include <QFile>

#include "two_opt.h"
#include "input.h"

QSharedPointer<QVector<int>> randomPermutation(const int n)
{
    static std::mt19937 randomGenerator;

    auto result = QSharedPointer<QVector<int>>::create(n);

    QSet<int> usedElements;

    int randomValue;
    for(int i=0; i<n; ++i)
    {
        do
        {
            randomValue = std::abs(static_cast<int>(randomGenerator())) % n + 1;
        }
        while(usedElements.contains(randomValue));

        usedElements.insert(randomValue);
        (*result)[i] = randomValue;
    }

    return result;
}


int main()
{
//    auto X = qapReadFile("data/bur26f.dat");
    Input X("data/bur26f.dat");

    auto fst = X.input.first;
    auto snd = X.input.second;

//    qDebug() << "Test";
//    Two_OPT test(X);
//    test.run();
//    qDebug() << test.getSolution();

    for(int i=0;i<20; i++)
    {
        QElapsedTimer elapsedTimer;
        elapsedTimer.start();
        auto result = randomPermutation(25);

        qDebug() << "Elapsed time: " + QString::number(elapsedTimer.nsecsElapsed()) + " nsec";
        qDebug() << *result;
    }

    return 0;
}
