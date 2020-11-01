#ifndef COST_H
#define COST_H

#include "common.h"
#include "input.h"

class Cost
{
public:
    Cost(Input &input);
    // "Monożymy odległość razy siłę interakcji [...]"
    int calculateCost(QVector<int> &solution);
    // "By uzyskać czas liniowy, musimy zauważyć, że jeżeli zmieniły się tylko dwa elementy w permutacji pi,
    // to wpłynęło to tylko na dwa wiersze i dwie kolumny"
    int updateCost(QVector<int> &solution, int i, int j);

private:
    int _cost;
    Input _input;

};

#endif // COST_H
