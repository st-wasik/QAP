#include "cost.h"

Cost::Cost(Input &input)
{
    _input = input;
}

int Cost::calculateCost(QVector<int> &solution)
{
    int cost = 0;
    int dim = _input.getDimension();
    for (int i = 0; i < dim; ++i)
    {
        for (int j = 0; j < dim; ++j) {
            cost += (*_input.distances)[solution[i]][solution[j]] * (*_input.interactions)[i][j];
        }
    }

    return cost;
}

int Cost::updateCost(QVector<int> &solution, int i, int j)
{
    int cost = 0;
}
