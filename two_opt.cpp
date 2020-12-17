#include "two_opt.h"

Two_OPT::Two_OPT(int dim, QSharedPointer<QVector<int>> solution)
{
    _dim = dim;
    _solution = solution;
    reset();
}

void Two_OPT::reset()
{
    i = 0;
    j = 0;
}

int Two_OPT::getI()
{
    return i;
}

int Two_OPT::getJ()
{
    return j;
}

QSharedPointer<QVector<int>> Two_OPT::next()
{
    if(!(i == 0 && j == 0))
    {
        // swap solution back to state before previous 'next' call
        std::swap((*_solution)[i], (*_solution)[j]);
    }

    j++;
    if (j == _dim)
    {
        i++;
        if(i == (_dim - 1))
        {
            return nullptr; // is done
        }
        else
        {
            j = i + 1;
        }
    }

    std::swap((*_solution)[i], (*_solution)[j]);

    return _solution;
}

void Two_OPT::random() {
    i = (rand() % _dim - 1) + 1;
    do {
        j = (rand() % (_dim - 2)) + 1;
    } while (i == j);
}
