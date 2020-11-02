#ifndef COMMON_H
#define COMMON_H

#include <QDebug>
#include <Qt>
#include <algorithm>

using Matrix = QVector<QVector<int>>;

QSharedPointer<QVector<int>> randomPermutation(const int n, int seed=42);

int newton2(int n);

int random(int n, int seed=42);

#endif // COMMON_H
