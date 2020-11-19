#ifndef HEURISTIC_H
#define HEURISTIC_H

#include "input.h"
#include "irunnable.hpp"

class Heuristic : public IRunnable
{
public:
    Heuristic(QSharedPointer<const Input> inputData, QSharedPointer<QVector<int> > initialSolution=nullptr);

    QPair<long long, QVector<int>> run(bool distancesDesc = false);
private:
    QSharedPointer<QVector<int>> _solution;
    QSharedPointer<const Input>  _inputData;

    bool isMappingAvailable(QSharedPointer<QVector<int> > solution, QPair<int, int> interactIndex, QPair<int, int> distIndex);

    // IRunnable interface
public:
    void runAlg(int timeMSec);
    Heuristic(QSharedPointer<const Input> inputData, bool initialSolution);
};

#endif // HEURISTIC_H
