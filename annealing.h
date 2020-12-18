#ifndef ANNEALING_H
#define ANNEALING_H

#include "input.h"
#include "irunnable.hpp"
#include <QSharedPointer>

class Annealing : public IRunnable
{
public:
    Annealing(QSharedPointer<const Input> inputData, int seed=42);

    QPair<long long, QVector<int>> run();

private:
    QSharedPointer<QVector<int>> _solution;
    QSharedPointer<const Input>  _inputData;
    float _initialTemp;
    int _stepLenght;

    float _scanLandscape();
    float _getInitialTemp();
    int _getStepLenght(int n);

    // IRunnable interface
public:
    void runAlg(int timeMSec);
};

#endif // ANNEALING_H
