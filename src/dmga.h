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

void dmga(const int evaluationTimes, float step_length, float eyesight, int dimension, int population);
void initialization(vector<Monkey> &monkeys);
void climb(vector<Monkey> &monkeys, const float step_length);
void watch_jump(vector<Monkey> &monkeys, float &best_fitness, const float eyesight, int *evaluationTimes);
void somersault(vector<Monkey> &monkeys);

#endif