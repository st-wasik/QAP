#include "two_opt.h"

Two_OPT::Two_OPT(int dim)
{
    _dim = dim;
    reset();
}

bool Two_OPT::isDone() {return _done;}

void Two_OPT::reset()
{
    i = 0;
    j = 1;
    _done = false;
}

QVector<int> Two_OPT::next()
{
    if (j == _dim)
        j = ++i + 1;

    if (i == _dim - 1) ... // fuck this

    return swap(i, j++);
}

QVector<int> Two_OPT::swap(int i, int j)
{
//    int temp;
//    temp = solution[i];
//    solution[i] = solution[j];
//    solution[j] = temp;
}
