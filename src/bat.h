#ifndef BAT_H_
#define BAT_H_

#include <iostream>
#include <vector>
#include "connection.h"
#include <random>
#include <float.h>
#include <algorithm>

using namespace std;

class Bat {
public:
    Bat(int);

    vector<bool> position; //X

    double fitness;
    float max_frequency, min_frequency;

    void initialization();
    void locate();
    void movement(Bat);
    void local_search();

private:
    vector<double> velocity; //v
    vector<bool> candidate;
    vector<double> distribution; //x
    double candidate_fitness;
    double frequency; //f
    double loudness; //L
    double pulse_rate; //r
};

// extern vector<Bat>::iterator best_bat;

void bat(int, int, int);
void echolocation(vector<Bat> &, Bat);
Bat get_best_bat(vector<Bat> &, Bat);

#endif