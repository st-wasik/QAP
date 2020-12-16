#ifndef TABU_HPP
#define TABU_HPP

#include "input.h"
#include "irunnable.hpp"

#include <QQueue>
#include <QSharedPointer>

#include <exception>

template <typename T> class RingQueue
{
public:
    RingQueue(int size) : _endPos(0), _size(size), _elemsAdded(0)
    {
        if(size <= 0)
            size = 1;

        _queue.resize(size);
    }

    void append(const T& val)
    {
        _queue[_endPos] = val;

        _elemsAdded++;
        if(_elemsAdded == _size)
            _elemsAdded -= 1;

        _endPos++;
        if(_endPos == _size)
            _endPos = 0;
    }

    const T& operator[](int i)
    {
        return _queue[i];
    }

    bool contains(const T& val)
    {
        auto limit = std::max(_elemsAdded, _endPos);
        for(int i=0;i<limit;i++)
        {
            if(val == _queue[i]) return true;
        }
        return false;
    }

    int count()
    {
        return _size;
    }

private:
    QVector<T> _queue;
    int _endPos;
    int _size;
    int _elemsAdded;
};

class Tabu : public IRunnable
{
public:
    Tabu(QSharedPointer<const Input> inputData, int seed=42);

    QPair<long long, QVector<int>> run(int limit);

    // IRunnable interface
    void runAlg(int timeMSec);

private:
    QSharedPointer<QVector<int>> _solution;
    QSharedPointer<const Input>  _inputData;
    QSharedPointer<RingQueue<QVector<int>>> _tabuQueue;

    Matrix _tabu;
};

#endif // TABU_HPP
