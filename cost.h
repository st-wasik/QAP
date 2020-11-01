#ifndef COST_H
#define COST_H

#include "common.h"
#include "input.h"

class Cost
{
public:
    Cost(QSharedPointer<const Input> inputData);
    // "Monożymy odległość razy siłę interakcji [...]"
    void calculateCost(QVector<int> &solution);
    // "By uzyskać czas liniowy, musimy zauważyć, że jeżeli zmieniły się tylko dwa elementy w permutacji pi,
    // to wpłynęło to tylko na dwa wiersze i dwie kolumny"
    void updateCost(QVector<int> &solution, int i, int j);

    long long getCost();

private:
    long long _cost;
    QSharedPointer<const Input> _inputData;

};

#endif // COST_H
