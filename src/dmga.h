#ifndef DMGA_H_
#define DMGA_H_

#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

class Monkey {
    public:
        Monkey(int);
    
        vector<float> position;
};

void dmga(int dimension, int population);
void initialization(vector<Monkey> &monkeys);
void climb(vector<Monkey> &monkeys);
void watch_jump(vector<Monkey> &monkeys, float &best_fitness);
void somersault(vector<Monkey> &monkeys);

#endif