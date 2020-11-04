#ifndef HEURISTIC_H
#define HEURISTIC_H

#include "input.h"

class Heuristic
{
public:
    Heuristic(QSharedPointer<const Input> inputData, QSharedPointer<QVector<int> > initialSolution=nullptr);

    QPair<long long, QVector<int>> run(bool distancesDesc = false);
private:
    QSharedPointer<QVector<int>> _solution;
    QSharedPointer<const Input>  _inputData;

    bool isMappingAvailable(QSharedPointer<QVector<int> > solution, QPair<int, int> interactIndex, QPair<int, int> distIndex);
};

#endif // HEURISTIC_H
