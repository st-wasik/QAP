#ifndef COMMON_H
#define COMMON_H

#include <QDebug>
#include <Qt>
#include <algorithm>

using Matrix = QVector<QVector<int>>;

QSharedPointer<QVector<int>> randomPermutation(const int n);

#endif // COMMON_H
