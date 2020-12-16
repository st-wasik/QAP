#ifndef TABU_HPP
#define TABU_HPP

#include "input.h"
#include "irunnable.hpp"

#include <QSharedPointer>

class Tabu : public IRunnable
{
public:
    Tabu(QSharedPointer<const Input> inputData);

    QPair<long long, QVector<int>> run(int timeLimitMSec, int cadency);

    // IRunnable interface
    void runAlg(int timeMSec);

private:
    QSharedPointer<QVector<int>> _solution;
    QSharedPointer<const Input>  _inputData;

    Matrix _tabu;
};

#endif // TABU_HPP
