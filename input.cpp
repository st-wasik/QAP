#include "input.h"

Input::Input()
{
}

Input::Input(Matrix dist, Matrix interact)
{
    _dim = dist.count();

    distances = QSharedPointer<Matrix>::create(dist);
    interactions = QSharedPointer<Matrix>::create(interact);
}

void Input::readFromFile(QString filename)
{
    QFile file(filename);

    QSharedPointer<Matrix> A, B;

    if(!file.open(QIODevice::ReadOnly))
    {
        qFatal("%s", QString("File " + filename + " open error!").toStdString().c_str());
    }

    QTextStream input(&file);

    bool ok;
    const int size = input.readLine().toInt(&ok);

    if(!ok)
    {
        qFatal("Cannot cast size to int!");
    }

    _dim = size;

    A = QSharedPointer<Matrix>::create(size, QVector<int>(size));
    B = QSharedPointer<Matrix>::create(size, QVector<int>(size));
    qDebug() << "Size is " + QString::number(size);

    input.readLine(); // empty line
    qDebug() << "Reading first matrix...";

    for(int i=0;i<size;i++)
    {
        QStringList words = input.readLine().split(" ");

        for(int j=0;j<size;j++)
        {
            auto& word = words[j];
            bool ok;
            auto wordAsInt = word.toInt(&ok);

            if(!ok)
            {
                qFatal("%s", QString("Cannot cast word '%0' [ %1 %2 ] into int!").arg(word).arg(i).arg(j).toStdString().c_str());
            }

            (*A)[i][j] = wordAsInt;
        }
    }

    input.readLine(); // empty line
    qDebug() << "Reading second matrix...";

    for(int i=0;i<size;i++)
    {
        QStringList words = input.readLine().split(" ", QString::SkipEmptyParts);

        for(int j=0;j<size;j++)
        {
            auto& word = words[j];
            bool ok;
            auto wordAsInt = word.toInt(&ok);

            if(!ok)
            {
                 qFatal("%s", QString("Cannot cast word '%0' [ %1 %2 ] into int!").arg(word).arg(i).arg(j).toStdString().c_str());
            }

            (*B)[i][j] = wordAsInt;
        }
    }

    distances = A;
    interactions = B;
}

int Input::getDimension() const {return _dim;}
