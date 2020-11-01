#include "localsearch.h"

LocalSearch::LocalSearch(QString filename)
{
    input = Input(filename);
    _solution = randomPermutation(input.getDimension());
}

QSharedPointer<QVector<int>>  LocalSearch::randomPermutation(const int n)
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

void LocalSearch::run(bool greedy)
{
    Cost cost(input);
    cost.calculateCost(*_solution);

    bool improvement = true;
    while (improvement)
    {
        // Generuj neighbour z _solution
        // Oblicz updateCost(_solution, neighbour)
        // Jeśli neighbour jest lepszy to _solution = neighbour
        // Jeśli żaden neighbour nie był lepszy improvment = false

    }
}

void LocalSearch::swap(QVector<int> &solution, int i, int j)
{
    int temp;
    temp = solution[i];
    solution[i] = solution[j];
    solution[j] = temp;
}
