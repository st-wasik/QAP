#ifndef STEEPEST_HPP
#define STEEPEST_HPP

#include "input.h"

#include <QSharedPointer>

class Steepest
{
public:
    Steepest(QSharedPointer<const Input> inputData, int seed=42);

    QPair<long long, QVector<int>> run();
private:
    QSharedPointer<QVector<int>> _solution;
    QSharedPointer<const Input>  _inputData;
};

#endif // STEEPEST_HPP
