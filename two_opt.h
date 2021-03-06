#ifndef TWO_OPT_H
#define TWO_OPT_H

#include "common.h"

class Two_OPT
{
public:
    Two_OPT(int dim, QSharedPointer<QVector<int>> solution);
    QSharedPointer<QVector<int>> next(bool swapBack=true);
    void reset();
    int getI();
    int getJ();
    void random();
    void doSwapBack();

private:
    int _dim, i, j;
    QSharedPointer<QVector<int>> _solution;
};

#endif // TWO_OPT_H
