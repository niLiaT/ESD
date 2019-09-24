#include <iostream>
#include <math.h>
#include <time.h>
#include "dmga.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));

    const int evaluationTimes = atoi(argv[1]);
    const int solution_length = atoi(argv[2]);
    const int monkey_quantity = atoi(argv[3]);
    const float step_length = atof(argv[4]);
    const float eyesight = atof(argv[5]);

    dmga(evaluationTimes, step_length, eyesight, solution_length, monkey_quantity);
}