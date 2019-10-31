#include <iostream>
#include <math.h>
#include <time.h>
#include "dmga.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));

    const int iterations = atoi(argv[1]);
    const int solution_length = atoi(argv[2]);
    const int monkey_quantity = atoi(argv[3]);

    dmga(iterations, solution_length, monkey_quantity);
}