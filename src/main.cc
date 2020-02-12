#include <iostream>
#include <math.h>
#include <time.h>
// #include "dmga.h"
// #include "bat.h"
#include "se.h"

using namespace std;

int main(int argc, char *argv[]) {
    srand(time(NULL));

    char *dataset = argv[1];
    int iterations = atoi(argv[2]);
    int solution_length = atoi(argv[3]);
    int regions = atoi(argv[5]);
    int searchers = atoi(argv[4]);
    int goods = atoi(argv[6]);
    int players = atoi(argv[7]);

    input(dataset);

    se(iterations, solution_length, regions, searchers, goods, players);

    return 0;
}