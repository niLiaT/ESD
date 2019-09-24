#include "dmga.h"
#include "constraint.h"
#include "fitness.h"

#define step_length 0.00001
#define eyesight 0.5

template <typename T> int sgn (T val);

void dmga(int dimension, int population) {
    float best_fitness = 0;
    vector<Monkey> monkeys(population, Monkey(dimension));

    initialization(monkeys);

    for (int iteration = 0; iteration < 100000; ++iteration) {
        climb(monkeys);

        watch_jump(monkeys, best_fitness);

        somersault(monkeys);

        cout << iteration << "," << best_fitness << endl;
    }
}

Monkey::Monkey(int dimension) {
    position.resize(dimension);
}

void initialization(vector<Monkey> &monkeys) {
    for (vector<Monkey>::iterator each_monkey = monkeys.begin(); each_monkey != monkeys.end(); ++each_monkey) {
        do {
            for (vector<float>::iterator each_bit = each_monkey->position.begin(); each_bit != each_monkey->position.end(); ++each_bit) {
                *each_bit = 10 * (rand() / (float)(RAND_MAX - 1));
            }
        } while (!constraint(each_monkey->position[0], each_monkey->position[1]));
    }
}

void climb(vector<Monkey> &monkeys) {
    vector<float> pseudo_gradient, temp, candidate;
    float fitness_differential;

    for (vector<Monkey>::iterator each_monkey = monkeys.begin(); each_monkey != monkeys.end(); ++each_monkey) {
        pseudo_gradient.resize(each_monkey->position.size());
        candidate.resize(each_monkey->position.size());
        for (int each_bit = 0; each_bit < each_monkey->position.size(); ++each_bit) {
            if (rand() / (float)(RAND_MAX) < 0.5) {
                temp.push_back(step_length);
            }
            else {
                temp.push_back(-step_length);
            }
        }

        fitness_differential = (fitness_func(each_monkey->position[0] + temp[0], each_monkey->position[1] + temp[1])\
             - fitness_func(each_monkey->position[0] - temp[0], each_monkey->position[1] - temp[1]));

        for (int each_bit = 0; each_bit < each_monkey->position.size(); ++each_bit) {
            pseudo_gradient[each_bit] = fitness_differential / (2 * temp[each_bit]);
            candidate[each_bit] = each_monkey->position[each_bit] + step_length * sgn(pseudo_gradient[each_bit]);
        }

        if (constraint(candidate[0], candidate[1])) {
            for (int each_bit = 0; each_bit < each_monkey->position.size(); ++each_bit) {
                each_monkey->position.assign(candidate.begin(), candidate.end());
            }
        }

        temp.clear();
        pseudo_gradient.clear();
        candidate.clear();
    }
}

void watch_jump(vector<Monkey> &monkeys, float &best_fitness) {
    vector<float> candidate;

    for (vector<Monkey>::iterator each_monkey = monkeys.begin(); each_monkey != monkeys.end(); ++each_monkey) {
        candidate.resize(each_monkey->position.size());
        
        for (int each_bit = 0; each_bit < each_monkey->position.size(); ++each_bit) {
            if (rand() / (float)RAND_MAX <= 0.5) {
                candidate[each_bit] = each_monkey->position[each_bit] - eyesight;
            }
            else {
                candidate[each_bit] = each_monkey->position[each_bit] + eyesight;
            }
        }

        if (constraint(candidate[0], candidate[1]) && fitness_func(candidate[0], candidate[1]) > fitness_func(each_monkey->position[0], each_monkey->position[1])) {
            each_monkey->position.assign(candidate.begin(), candidate.end());
        }

        if (fitness_func(each_monkey->position[0], each_monkey->position[1]) > best_fitness) {
            best_fitness = fitness_func(each_monkey->position[0], each_monkey->position[1]);
        }

        candidate.clear();
    }
}

void somersault(vector<Monkey> &monkeys) {
    float alpha;
    vector<float> pivot;
    vector<float> candidate;
    
    pivot.resize(monkeys[0].position.size());

    for (int each_bit = 0; each_bit < pivot.size(); ++each_bit) {
        for (vector<Monkey>::iterator each_monkey = monkeys.begin(); each_monkey != monkeys.end(); ++each_monkey) {
            pivot[each_bit] += each_monkey->position[each_bit];
        }
        pivot[each_bit] /= monkeys.size();
    }

    for (vector<Monkey>::iterator each_monkey = monkeys.begin(); each_monkey != monkeys.end(); ++each_monkey) {

        do {
            candidate.resize(each_monkey->position.size());
            for (int each_bit = 0; each_bit < candidate.size(); ++each_bit) {
                alpha = rand() / (float)RAND_MAX * 2 - 1;
                candidate[each_bit] = each_monkey->position[each_bit] + alpha * (pivot[each_bit] - each_monkey->position[each_bit]);
            }
        } while (!constraint(candidate[0], candidate[1]));

        each_monkey->position.assign(candidate.begin(), candidate.end());        

        candidate.clear();
    }
    
}

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}