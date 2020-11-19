#ifndef STEEPEST_HPP
#define STEEPEST_HPP

#include "input.h"
#include "irunnable.hpp"

#include <QSharedPointer>

class Steepest : public IRunnable
{
public:
    Steepest(QSharedPointer<const Input> inputData, int seed=42);

    QPair<long long, QVector<int>> run();
private:
    QSharedPointer<QVector<int>> _solution;
    QSharedPointer<const Input>  _inputData;

    // IRunnable interface
public:
    void runAlg(int timeMSec);
};

#endif // STEEPEST_HPP
