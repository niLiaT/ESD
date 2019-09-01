#include "fitness.h"

float fitness_func(float x1, float x2) {
    return pow(sin(2 * M_PI * x1), 3) * sin(2 * M_PI * x2) / (pow(x1, 3) * (x1 + x2));
}