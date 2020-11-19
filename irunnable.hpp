#ifndef IRUNNABLE_HPP
#define IRUNNABLE_HPP

class IRunnable
{
public:
    virtual void runAlg(int timeMSec = 0) = 0;

    virtual ~IRunnable();
};

#endif // IRUNNABLE_HPP
