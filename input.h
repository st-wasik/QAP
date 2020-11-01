#ifndef INPUT_H
#define INPUT_H

#include "common.h"
#include <QFile>

class Input
{
public:
    Input();

    QSharedPointer<Matrix> distances, interactions;

    int getDimension() const;

    void readFromFile(QString filename);
private:
    int _dim;
};

#endif // INPUT_H
