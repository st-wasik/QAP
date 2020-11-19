#ifndef RANDOM_H
#define RANDOM_H

#include "input.h"
#include "irunnable.hpp"

#include <QTime>

class Random : public IRunnable
{
public:
    Random(QSharedPointer<const Input> inputData, int seed=42);

    QPair<long long, QVector<int>> run(long timeLimitMSec);
private:
    QSharedPointer<QVector<int>> _solution;
    QSharedPointer<const Input>  _inputData;

    // IRunnable interface
public:
    void runAlg(int timeMSec);
};

#endif // RANDOM_H
