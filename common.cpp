#include "common.h"
#include "random"

QSharedPointer<QVector<int>> randomPermutation(const int n, int seed)
{
    static std::mt19937 randomGenerator(seed);

    auto result = QSharedPointer<QVector<int>>::create(n);

    QSet<int> usedElements;

    int randomValue;
    for(int i=0; i<n; ++i)
    {
        do
        {
            randomValue = std::abs(static_cast<int>(randomGenerator())) % n;
        }
        while(usedElements.contains(randomValue));

        usedElements.insert(randomValue);
        (*result)[i] = randomValue;
    }

    return result;
}

int newton2(int n)
{
    constexpr int k=2;
    return (n * (n-1)) / k;
}

int random(int n, int seed)
{
    static std::mt19937 randomGenerator(seed);

    return std::abs(static_cast<int>(randomGenerator())) % n;
}

QString vectorToString(const QVector<int>& v)
{
    QStringList res;

    for(const auto& x : v)
        res << QString::number(x);

    return res.join(" ");
}
