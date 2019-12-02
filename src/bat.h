#ifndef BAT_H_
#define BAT_H_

#include <iostream>
#include <vector>
// #include "connection.h"
#include "tsp.h"
#include <random>
#include <float.h>
#include <algorithm>

#define max_frequency 20
#define min_frequency 500
#define max_wavelength (340 / min_frequency)
#define min_wavelength (340 / max_frequency)

using namespace std;

class Bat {
    public:
        Bat(int);

        vector<int> position, new_position, candidate_position;

        double fitness, candidate_fitness;

        void initialized();
        void locate();
        void movement();
    
    private:
        int velocity, time_step;
        double pulse_rate, init_pulse_rate;
        double loudness;
};

extern vector<Bat>::iterator best_bat;

void initialization(vector<Bat> &);
void echolocation(vector<Bat> &);
vector<Bat>::iterator get_best_bats(vector<Bat> &);

#endif