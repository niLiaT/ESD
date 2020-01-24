#include "fitness.h"

int calc_fitness(vector<bool> solution) {
    int accumulator = 0;

    for (vector<bool>::iterator each_bit = solution.begin(); each_bit != solution.end(); ++each_bit) {
        accumulator += *each_bit;
    }

    return accumulator;
}