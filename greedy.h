#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H

#include <random>
#include "common.h"
#include "cost.h"
#include "input.h"
#include "irunnable.hpp"

class Greedy : public IRunnable
{
public:
    Greedy(QSharedPointer<const Input> inputData, int seed=42);

    QPair<long long, QVector<int>> run();
private:
    QSharedPointer<QVector<int>> _solution;
    QSharedPointer<const Input>  _inputData;

    // IRunnable interface
public:
    void runAlg(int timeMSec);
};

#endif // LOCALSEARCH_H
