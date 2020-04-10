#ifndef GA_H_
#define GA_H_

#include <iostream>
#include <vector>
#include "connection.h"
#include <algorithm>

class Chromosome {
public:
    vector<bool> dna;
    double fitness;

    Chromosome(int);
};

void ga(int, int, int, int, float, float);

#endif