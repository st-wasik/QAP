#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H

#include <random>
#include "common.h"
#include "cost.h"
#include "input.h"

class LocalSearch
{
public:
    LocalSearch(QString filename);
    void run(bool greedy = true);

    Input input;
    QSharedPointer<QVector<int>> randomPermutation(const int n);

private:
    Cost cost();
    void swap(QVector<int> &solution, int i, int j);

    QSharedPointer<QVector<int>> _solution;
};

#endif // LOCALSEARCH_H
