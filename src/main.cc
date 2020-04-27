#include <iostream>
#include <math.h>
#include <time.h>
#include "tf.h"

using namespace std;

int main(int argc, char *argv[]) {
    srand(time(NULL));

    char *dataset = argv[1];
    int device_number = atoi(argv[2]);

    input(dataset);

    tf(device_number);

    return 0;
}