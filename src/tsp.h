#ifndef TSP_H_
#define TSP_H_

#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>

using namespace std;

class City {
    public:
        City();

    // private:
        int x, y;
};

extern vector<City> cities;

void input();
double cost_evaluation(vector<int> &);

#endif