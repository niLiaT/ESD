#include "fitness.h"

int one_max(vector<bool> solution) {
    int sum = 0;

    for (vector<bool>::iterator each_bit = solution.begin(); each_bit != solution.end(); ++each_bit) {
        sum += *each_bit;
    }

    return sum;
}