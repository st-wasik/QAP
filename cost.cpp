#include "cost.h"

Cost::Cost(QSharedPointer<const Input> inputData)
{
    _inputData = inputData;
    _cost = 0;
}

void Cost::calculateCost(QSharedPointer<QVector<int>> solutionPtr)
{
    auto& cost = _cost;
    auto& solution = *solutionPtr;

    cost = 0;
    const int dim = _inputData->getDimension();

    for (int i = 0; i < dim; ++i)
    {
        for (int j = 0; j < dim; ++j)
        {
            cost += (*_inputData->distances)[solution[i]][solution[j]] * (*_inputData->interactions)[i][j];
        }
    }
}

/**
 * @brief Cost::updateCost
 * @param solution: the new solution
 * @param posA: solutions's first changed element index
 * @param posB: solutions's second changed element index
 * example when [1,2,3,4] -> [4,2,3,1]; then posA is 0, posB is 3
 */
long long Cost::getUpdatedCost(QSharedPointer<QVector<int>> solutionPtr, int posA, int posB)
{
    long long cost = _cost;

    const int dim = _inputData->getDimension();
    int i=0, j=0;

    {
        std::swap((*solutionPtr)[posA], (*solutionPtr)[posB]);

        auto &s = *solutionPtr;

        // SUBTRACT SUM OF ROW VALUES
        for (int j = 0; j < dim; ++j)
        {
            cost -= (*_inputData->distances)[s[posA]][s[j]] * (*_inputData->interactions)[posA][j]
                  + (*_inputData->distances)[s[posB]][s[j]] * (*_inputData->interactions)[posB][j];
        }
        // END

        // SUBTRACT SUM OF COLUMN VALUES
        j = posA;
        for (int i = 0; i < dim; ++i)
        {
            cost -= (*_inputData->distances)[s[i]][s[posA]] * (*_inputData->interactions)[i][posA]
                  + (*_inputData->distances)[s[i]][s[posB]] * (*_inputData->interactions)[i][posB];
        }
        // END

        // ADD TWICE SUBTRACTED VALUES
        cost += (*_inputData->distances)[s[posA]][s[posA]] * (*_inputData->interactions)[posA][posA]
              + (*_inputData->distances)[s[posA]][s[posB]] * (*_inputData->interactions)[posA][posB]
              + (*_inputData->distances)[s[posB]][s[posA]] * (*_inputData->interactions)[posB][posA]
              + (*_inputData->distances)[s[posB]][s[posB]] * (*_inputData->interactions)[posB][posB];

        std::swap((*solutionPtr)[posA], (*solutionPtr)[posB]);
    }

    // REPEAT THESE STEPS FOR NEW SOLUTION (BUT ADD COST)
    {
        auto &s = *solutionPtr;

        // ADD SUM OF ROW VALUES
        i = posA;
        for (int j = 0; j < dim; ++j)
        {
            cost += (*_inputData->distances)[s[posA]][s[j]] * (*_inputData->interactions)[posA][j]
                  + (*_inputData->distances)[s[posB]][s[j]] * (*_inputData->interactions)[posB][j];
        }
        // END

        // ADD SUM OF COLUMN VALUES
        j = posA;
        for (int i = 0; i < dim; ++i)
        {
            cost += (*_inputData->distances)[s[i]][s[posA]] * (*_inputData->interactions)[i][posA]
                  + (*_inputData->distances)[s[i]][s[posB]] * (*_inputData->interactions)[i][posB];
        }
        // END

        // SUBTRACT TWICE ADDED VALUES
        cost -= (*_inputData->distances)[s[posA]][s[posA]] * (*_inputData->interactions)[posA][posA]
              + (*_inputData->distances)[s[posA]][s[posB]] * (*_inputData->interactions)[posA][posB]
              + (*_inputData->distances)[s[posB]][s[posA]] * (*_inputData->interactions)[posB][posA]
              + (*_inputData->distances)[s[posB]][s[posB]] * (*_inputData->interactions)[posB][posB];
    }

    return cost;
}

long long Cost::getCost()
{
    return _cost;
}

void Cost::setCost(long long cost)
{
    _cost = cost;
}
