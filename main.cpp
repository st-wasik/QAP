#include <random>
#include <QDebug>
#include <Qt>
#include <algorithm>
#include <QElapsedTimer>
#include <QFile>

QSharedPointer<QVector<int>> randomPermutation(const int n)
{
    static std::mt19937 randomGenerator;

    auto result = QSharedPointer<QVector<int>>::create();
    result->resize(n);

    QSet<int> usedElements;

    int randomValue;
    for(int i=0; i<n; ++i)
    {
        do
        {
            randomValue = std::abs(static_cast<int>(randomGenerator())) % n + 1;
        }
        while(usedElements.contains(randomValue));

        usedElements.insert(randomValue);
        (*result)[i] = randomValue;
    }

    return result;
}

using Matrix = QVector<QVector<int>>;

QPair<QSharedPointer<Matrix>, QSharedPointer<Matrix>> qapReadFile(QString filename)
{
    auto A = QSharedPointer<Matrix>::create();
    auto B = QSharedPointer<Matrix>::create();

    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly))
    {
        qFatal("%s", QString("File " + filename + " open error!").toStdString().c_str());
    }
    else
    {
        QTextStream input(&file);

        bool ok;
        const int size = input.readLine().toInt(&ok);

        if(ok)
        {
            A->resize(size);
            for(int i=0;i<size;i++)
                (*A)[i].resize(size);

            B->resize(size);
            for(int i=0;i<size;i++)
                (*B)[i].resize(size);

            qDebug() << "Size is " + QString::number(size);
        }
        else
        {
            qFatal("Cannot cast size to int!");
        }

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

                if(ok)
                {
                    (*A)[i][j] = wordAsInt;
                }
                else
                {
                    qFatal("%s", QString("Cannot cast word '%0' [ %1 %2 ] into int!").arg(word).arg(i).arg(j).toStdString().c_str());
                }
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

                if(ok)
                {
                    (*B)[i][j] = wordAsInt;
                }
                else
                {
                     qFatal("%s", QString("Cannot cast word '%0' [ %1 %2 ] into int!").arg(word).arg(i).arg(j).toStdString().c_str());
                }
            }
        }
    }

    return {A,B};
}

int main()
{
    auto X = qapReadFile("data/bur26f.dat");

    auto fst = X.first;
    auto snd = X.second;

    for(int i=0;i<20; i++)
    {
        QElapsedTimer elapsedTimer;
        elapsedTimer.start();
        auto result = randomPermutation(25);

        qDebug() << "Elapsed time: " + QString::number(elapsedTimer.nsecsElapsed()) + " nsec";
        qDebug() << *result;
    }

    return 0;
}
