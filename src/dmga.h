#ifndef DMGA_H_
#define DMGA_H_

#include <iostream>
#include <vector>
#include <math.h>
#include "connection.h"
#include <algorithm>

using namespace std;

class Monkey {
public:
    Monkey();
    Monkey(int);

    vector<bool> position;
    double fitness;
};

void dmga(const int evaluationTimes, int dimension, int population);
void initialization(vector<Monkey> &monkeys);
void climb(vector<Monkey> &monkeys);
void watch_jump(vector<Monkey> &monkeys);
void cooperation(vector<Monkey> &monkeys);
void crossover_mutation(vector<Monkey> &monkeys);
void somersault(vector<Monkey> &monkeys);
double best_monkey(vector<Monkey> &monkeys);

#endif