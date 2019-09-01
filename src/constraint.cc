#include "constraint.h"

bool constraint(float x1, float x2) {
    if (x1 >= 0 && x1 <= 10 && x2 >= 0 && x2 <=10) {
        if ((pow(x1, 2) - x2 + 1 <= 0) && (1 - x1 + pow(x2 - 4, 2) <= 0)) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}