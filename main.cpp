#include <random>
#include <QDebug>
#include <algorithm>
#include <QElapsedTimer>

QList<int> randomPermutation(const int n)
{
    static std::mt19937 randomGenerator;

    QList<int> result;
    QSet<int>  usedElements;

    int randomValue;
    for(int i=0; i<n; ++i)
    {
        do
        {
            randomValue = std::abs(static_cast<int>(randomGenerator())) % n + 1;
        }
        while(usedElements.contains(randomValue));

        usedElements.insert(randomValue);
        result.append(randomValue);
    }

    return result;
}


int main()
{
    for(int i=0;i<20; i++)
    {
        QElapsedTimer elapsedTimer;
        elapsedTimer.start();
        auto result = randomPermutation(25);

        qDebug() << "Elapsed time: " + QString::number(elapsedTimer.nsecsElapsed()) + " nsec";
        qDebug() << result;
    }

    return 0;
}
