#include "localsearch.h"

LocalSearch::LocalSearch(QSharedPointer<const Input> inputData)
{
    _inputData = inputData;
    _solution  = randomPermutation(input.getDimension());
}

void LocalSearch::run(bool greedy)
{
    Cost cost(_inputData);
    cost.calculateCost(*_solution);

    bool improvement = true;
    while (improvement)
    {
        // Generuj neighbour z _solution
        // Oblicz updateCost(_solution, neighbour)
        // Jeśli neighbour jest lepszy to _solution = neighbour
        // Jeśli żaden neighbour nie był lepszy improvment = false


        // remove this
        break;
    }
}

void LocalSearch::swap(QVector<int> &solution, int i, int j)
{
    std::swap(solution[i], solution[j]);
}
