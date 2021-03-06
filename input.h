#ifndef INPUT_H
#define INPUT_H

#include "common.h"
#include <QFile>

class Input
{
public:
    Input();
    Input(Matrix dist, Matrix interact);

    QSharedPointer<Matrix> distances, interactions;

    int getDimension() const;

    void readFromFile(QString filename);

    QString getFilename() const;

private:
    int _dim;

    QString filename;
};

#endif // INPUT_H
