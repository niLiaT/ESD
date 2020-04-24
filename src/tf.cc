#include "tf.h"

void tf() {
    vector<bool> solution(880, true);

    double fitness = cost_evaluation(solution);
    // find_loading(solution);

    cout << fitness << endl;
}