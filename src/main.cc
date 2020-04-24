#include <iostream>
#include <math.h>
#include <time.h>
#include "tf.h"

using namespace std;

int main(int argc, char *argv[]) {
    srand(time(NULL));

    char *dataset = argv[1];

    input(dataset);

    tf();

    return 0;
}