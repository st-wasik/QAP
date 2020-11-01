#include "common.h"
#include "random"

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
