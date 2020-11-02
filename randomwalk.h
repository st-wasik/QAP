#ifndef RANDOMWALK_H
#define RANDOMWALK_H

#include "input.h"

class RandomWalk
{
public:
    RandomWalk(QSharedPointer<const Input> inputData, int seed);
    QPair<long long, QVector<int> > run(int steps);
private:
    QSharedPointer<QVector<int>> _solution;
    QSharedPointer<const Input>  _inputData;
};

#endif // RANDOMWALK_H
