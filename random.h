#ifndef RANDOM_H
#define RANDOM_H

#include "input.h"

#include <QTime>

class Random
{
public:
    Random(QSharedPointer<const Input> inputData, int seed=42);

    QPair<long long, QVector<int>> run(long timeLimitMSec);
private:
    QSharedPointer<QVector<int>> _solution;
    QSharedPointer<const Input>  _inputData;
};

#endif // RANDOM_H
