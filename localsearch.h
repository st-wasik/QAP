#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H

#include <random>
#include "common.h"
#include "cost.h"
#include "input.h"

class LocalSearch
{
public:
    LocalSearch(QSharedPointer<const Input> inputData);
    void run(bool greedy = true);

    Input input;

private:
    Cost cost();
    void swap(QVector<int> &solution, int i, int j);

    QSharedPointer<QVector<int>> _solution;
    QSharedPointer<const Input> _inputData;
};

#endif // LOCALSEARCH_H
