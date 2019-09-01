#include <iostream>
#include <math.h>
#include <time.h>
#include "dmga.h"

#define solution_length 2
#define monkey_quantity 10

int main() {
    srand(time(NULL));

    dmga(solution_length, monkey_quantity);
}