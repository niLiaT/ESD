#include "dmga.h"
// #include "connection.h"
#include "fitness.h"

#define max_number 10

template <typename T> int sgn (T val);

void dmga(const int evaluationTimes, const float step_length, const float eyesight, int dimension, int population) {
    int iteration = 0;
    float best_fitness = 0;
    vector<Monkey> monkeys(population, Monkey(dimension));

    initialization(monkeys);

    while (iteration < evaluationTimes) {
        climb(monkeys, step_length);

        iteration++;

        // watch_jump(monkeys, best_fitness, eyesight, &iteration);

        // somersault(monkeys);

        // cout << iteration / population << "," << best_fitness << endl;
    }
}

Monkey::Monkey(int dimension) {
    position.resize(dimension);
}

void initialization(vector<Monkey> &monkeys) {
    //Random generate the solution of each monkey
    for (vector<Monkey>::iterator each_monkey = monkeys.begin(); each_monkey != monkeys.end(); ++each_monkey) {
        for (vector<bool>::iterator each_bit = each_monkey->position.begin(); each_bit != each_monkey->position.end(); ++each_bit) {
            *each_bit = rand() % 2;
        }
        each_monkey->fitness = one_max(each_monkey->position);
    }
}

vector<bool> large_step(vector<bool> solution) {
    vector<bool> temp;

    temp.assign(solution.begin(), solution.end());

    for (int each_bit = 0; each_bit < solution.size(); ++each_bit) {
        if (rand() % 2 == 0) {
            temp[each_bit] = (solution[each_bit] + 1) % 2;
        }
    }

    return temp;
}

vector<bool> small_step(vector<bool> solution) {
    vector<bool> temp;
    int bit_index = rand() % solution.size();

    temp.assign(solution.begin(), solution.end());
    temp[bit_index] = temp[bit_index] + (solution[bit_index] + 1) % 2;

    return temp;
}

void climb(vector<Monkey> &monkeys, const float step_length) {
    int new_fitness;
    vector<bool> temp_position;
    for (vector<Monkey>::iterator each_monkey = monkeys.begin(); each_monkey != monkeys.end(); ++each_monkey) {
        for (int step_number = 0; step_number < max_number; ++step_number) {
            temp_position = large_step(each_monkey->position);
            new_fitness = one_max(temp_position);
            if (new_fitness > each_monkey->fitness) {
                each_monkey->position.assign(temp_position.begin(), temp_position.end());
                each_monkey->fitness = new_fitness;
            }
        }
        for (int step_number = 0; step_number < max_number; ++step_number) {
            temp_position = small_step(each_monkey->position);
            new_fitness = one_max(temp_position);
            if (new_fitness > each_monkey->fitness) {
                each_monkey->position.assign(temp_position.begin(), temp_position.end());
                each_monkey->fitness = new_fitness;
            }
        }
    }
}

// void watch_jump(vector<Monkey> &monkeys, float &best_fitness, const float eyesight, int *iteration) {
//     vector<float> candidate;

//     for (vector<Monkey>::iterator each_monkey = monkeys.begin(); each_monkey != monkeys.end(); ++each_monkey) {
//         candidate.resize(each_monkey->position.size());
        
//         for (int each_bit = 0; each_bit < each_monkey->position.size(); ++each_bit) {
//             if (rand() / (float)RAND_MAX <= 0.5) {
//                 candidate[each_bit] = each_monkey->position[each_bit] - eyesight;
//             }
//             else {
//                 candidate[each_bit] = each_monkey->position[each_bit] + eyesight;
//             }
//         }

//         if (constraint(candidate[0], candidate[1]) && fitness_func(candidate[0], candidate[1]) > fitness_func(each_monkey->position[0], each_monkey->position[1])) {
//             each_monkey->position.assign(candidate.begin(), candidate.end());
//         }

//         if (fitness_func(each_monkey->position[0], each_monkey->position[1]) > best_fitness) {
//             best_fitness = fitness_func(each_monkey->position[0], each_monkey->position[1]);
//         }

//         *iteration += 1;

//         candidate.clear();
//     }
// }

// void somersault(vector<Monkey> &monkeys) {
//     float alpha;
//     vector<float> pivot;
//     vector<float> candidate;
    
//     pivot.resize(monkeys[0].position.size());

//     for (int each_bit = 0; each_bit < pivot.size(); ++each_bit) {
//         for (vector<Monkey>::iterator each_monkey = monkeys.begin(); each_monkey != monkeys.end(); ++each_monkey) {
//             pivot[each_bit] += each_monkey->position[each_bit];
//         }
//         pivot[each_bit] /= monkeys.size();
//     }

//     for (vector<Monkey>::iterator each_monkey = monkeys.begin(); each_monkey != monkeys.end(); ++each_monkey) {

//         do {
//             candidate.resize(each_monkey->position.size());
//             for (int each_bit = 0; each_bit < candidate.size(); ++each_bit) {
//                 alpha = rand() / (float)RAND_MAX * 2 - 1;
//                 candidate[each_bit] = each_monkey->position[each_bit] + alpha * (pivot[each_bit] - each_monkey->position[each_bit]);
//             }
//         } while (!constraint(candidate[0], candidate[1]));

//         each_monkey->position.assign(candidate.begin(), candidate.end());        

//         candidate.clear();
//     }
    
// }

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}