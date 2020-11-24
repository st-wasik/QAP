#include "globaloutput.hpp"

#include <QTextStream>

GlobalOutput::GlobalOutput() : _output("global.csv")
{
}

GlobalOutput &GlobalOutput::getInstance()
{
    static GlobalOutput instance;

    return instance;
}

void GlobalOutput::write(QString data)
{
    QMutexLocker lock(&_mutex);

    if(!_output.open(QIODevice::Append)) qCritical("Cannot open global output file!");

    QTextStream fin(&_output);

    fin << data << "\n";

    _output.close();
}

void GlobalOutput::resetFileContent()
{
    QMutexLocker lock(&_mutex);

    if(!_output.open(QIODevice::WriteOnly)) qCritical("Cannot open global output file!");

    QTextStream fin(&_output);

//    fin << "instance,algorithm,cost,time_msec,steps,checked_solutions,initial_cost\n";
//    fin << "instance,algorithm,step,cost\n";
        fin << "instance,algorithm,cost,permutation\n";

    _output.close();
}
