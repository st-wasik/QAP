#ifndef TWO_OPT_H
#define TWO_OPT_H

#include "common.h"

class Two_OPT
{
public:
    Two_OPT(int dim);
    QVector<int> next();
    void reset();
    bool isDone();

private:
    int _dim, i, j;
    bool _done;
    QVector<int> _solution;

    QVector<int> swap(int i, int j);
};

#endif // TWO_OPT_H
