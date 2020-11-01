#ifndef INPUT_H
#define INPUT_H

#include "common.h"
#include <QFile>

class Input
{
public:
    Input(QString filename);
    QPair<QSharedPointer<Matrix>, QSharedPointer<Matrix>> input;
    QSharedPointer<Matrix> distances, interactions;
    QString getFilename() const;
    int getDimension() const;

private:
    QString _filename;
    int _dim;

    QPair<QSharedPointer<Matrix>, QSharedPointer<Matrix>> qapReadFile(QString filename);
};

#endif // INPUT_H
