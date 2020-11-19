#ifndef GLOBALOUTPUT_HPP
#define GLOBALOUTPUT_HPP

#include <QFile>
#include <QMutex>

class GlobalOutput
{
public:
    static GlobalOutput& getInstance();

    GlobalOutput(const GlobalOutput&) = delete;

    void write(QString data);

    void resetFileContent();
private:
    GlobalOutput();

    QFile _output;

    QMutex _mutex;
};

#endif // GLOBALOUTPUT_HPP
