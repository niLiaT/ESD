#include "tf.h"

void tf(int solution_length) {
    vector<bool> solution(solution_length, true);
    Priority priority;
    int counter = 0;

    double fitness = cost_evaluation(solution), candidate_fitness = DBL_MAX;
    priority = find_loading(solution);

    for (vector<bool>::iterator each_bit = solution.begin(); each_bit != solution.end(); ++each_bit) {
        *each_bit = false;
    }

    for (multimap<int, int, greater<int>>::iterator each_gate_way = priority.gate_way_loading.begin(); each_gate_way != priority.gate_way_loading.end(); ++each_gate_way) {
        solution.at(each_gate_way->second) = true;
        
        for (multimap<int, int, greater<int>>::iterator each_fog_device = priority.fog_device_loading.begin(); each_fog_device != priority.fog_device_loading.end(); ++each_fog_device) {
            solution.at(each_fog_device->second) = true;

            for (multimap<int, int, greater<int>>::iterator each_edge_device = priority.edge_device_loading.begin(); each_edge_device != priority.edge_device_loading.end(); ++each_edge_device) {
                solution.at(each_edge_device->second) = true;

                candidate_fitness = cost_evaluation(solution);
                counter++;
                if (candidate_fitness < fitness) {
                    fitness = candidate_fitness;
                }
                cout << counter << "," << fitness << endl;
            }
        }
    }
}