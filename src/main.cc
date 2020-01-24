#include <iostream>
#include <math.h>
#include <time.h>
// #include "dmga.h"
// #include "bat.h"
#include "se.h"

using namespace std;

int main(int argc, char *argv[]) {
    srand(time(NULL));

    int iterations = atoi(argv[1]);
    int solution_length = atoi(argv[2]);
    // const int population = atoi(argv[3]);

    int regions = atoi(argv[3]);
    int searchers = atoi(argv[4]);
    int goods = atoi(argv[5]);
    int players = atoi(argv[6]);

    // dmga(iterations, solution_length, population);
    // bat(iterations, solution_length, population);
    se(iterations, solution_length, regions, searchers, goods, players);

    return 0;
}