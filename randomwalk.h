#ifndef RANDOMWALK_H
#define RANDOMWALK_H

#include "input.h"
#include "irunnable.hpp"

class RandomWalk : public IRunnable
{
public:
    RandomWalk(QSharedPointer<const Input> inputData, int seed = 42) ;
    QPair<long long, QVector<int> > run(long timeLimitMSec);
private:
    QSharedPointer<QVector<int>> _solution;
    QSharedPointer<const Input>  _inputData;

    // IRunnable interface
public:
    void runAlg(int timeMSec);
};

#endif // RANDOMWALK_H
