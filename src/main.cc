#include <iostream>
#include <math.h>
#include <time.h>
#include "ga.h"

using namespace std;

int main(int argc, char *argv[]) {
    srand(time(NULL));

    char *dataset = argv[1];
    int iterations = atoi(argv[2]);
    int solution_length = atoi(argv[3]);
    int population = atoi(argv[4]);
    int players = atoi(argv[5]);
    float crossover_rate = atof(argv[6]);
    float mutation_rate = atof(argv[7]);

    input(dataset);

    ga(iterations, solution_length, population, players, crossover_rate, mutation_rate);

    return 0;
}