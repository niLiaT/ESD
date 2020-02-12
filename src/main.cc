#include <iostream>
#include <math.h>
#include <time.h>
#include "dmga.h"

using namespace std;

int main(int argc, char *argv[]) {
    srand(time(NULL));

    char *dataset = argv[1];
    int iterations = atoi(argv[2]);
    int solution_length = atoi(argv[3]);
    int population = atoi(argv[4]);

    input(dataset);

    dmga(iterations, solution_length, population);

    return 0;
}