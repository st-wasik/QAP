#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H

#include <random>
#include "common.h"
#include "cost.h"
#include "input.h"

class Greedy
{
public:
    Greedy(QSharedPointer<const Input> inputData, int seed=42);

    QSharedPointer<QVector<int>> getSolution();

    QPair<long long, QVector<int>> run();
private:
    QSharedPointer<QVector<int>> _solution;
    QSharedPointer<const Input>  _inputData;
};

#endif // LOCALSEARCH_H
