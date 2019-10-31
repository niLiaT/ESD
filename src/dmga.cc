#include "dmga.h"
#include "connection.h"
#include "fitness.h"

#define max_number 10
#define parents_number 2
#define mutation_rate 0.3

template <typename T> int sgn (T val);
template <typename T> vector<T> read_vector_range(vector<T> vec, int start_index, int count);
template <typename T> void write_vector_range(vector<T> &target_vec, vector<T> source_vec, int start_index);

int evaluation_times = 0;

void dmga(const int iterations, int dimension, int population) {
    float best_fitness = 0;
    vector<Monkey> monkeys(population, Monkey(dimension));

    initialization(monkeys);

    while (evaluation_times < iterations) {
        climb(monkeys);

        watch_jump(monkeys);

        cooperation(monkeys);

        crossover_mutation(monkeys);

        somersault(monkeys);

        for (vector<Monkey>::iterator each_monkey = monkeys.begin(); each_monkey != monkeys.end(); ++each_monkey) {
            for (vector<bool>::iterator each_bit = each_monkey->position.begin(); each_bit != each_monkey->position.end(); ++each_bit) {
                cout << *each_bit << " ";
            }
            cout << endl;
        }

        // cout << iteration / population << "," << best_fitness << endl;
    }
}

Monkey::Monkey(int dimension) {
    position.resize(dimension);
}

void initialization(vector<Monkey> &monkeys) {
    //Random generate the solution of each monkey
    connect();
    for (vector<Monkey>::iterator each_monkey = monkeys.begin(); each_monkey != monkeys.end(); ++each_monkey) {
        for (vector<bool>::iterator each_bit = each_monkey->position.begin(); each_bit != each_monkey->position.end(); ++each_bit) {
            *each_bit = rand() % 2;
        }
        each_monkey->fitness = one_max(each_monkey->position);
    }
}

vector<bool> large_step(vector<bool> solution) {
    vector<bool> temp;

    temp = solution;

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

    temp = solution;
    temp[bit_index] = temp[bit_index] + (solution[bit_index] + 1) % 2;

    return temp;
}

void climb(vector<Monkey> &monkeys) {
    int new_fitness;
    vector<bool> temp_position;

    for (vector<Monkey>::iterator each_monkey = monkeys.begin(); each_monkey != monkeys.end(); ++each_monkey) {
        for (int step_number = 0; step_number < max_number; ++step_number) {
            temp_position = large_step(each_monkey->position);
            for (int each_bit = 0; each_bit < temp_position.size(); ++each_bit) {
                temp_position[each_bit] = temp_position[each_bit] + each_monkey->position[each_bit];
            }
            new_fitness = one_max(temp_position);
            evaluation_times++;
            if (new_fitness > each_monkey->fitness) {
                each_monkey->position = temp_position;
                each_monkey->fitness = new_fitness;
            }
        }
        for (int step_number = 0; step_number < max_number; ++step_number) {
            temp_position = small_step(each_monkey->position);
            for (int each_bit = 0; each_bit < temp_position.size(); ++each_bit) {
                temp_position[each_bit] = temp_position[each_bit] + each_monkey->position[each_bit];
            }
            new_fitness = one_max(temp_position);
            evaluation_times++;
            if (new_fitness > each_monkey->fitness) {
                each_monkey->position = temp_position;
                each_monkey->fitness = new_fitness;
            }
        }
    }
}

void jump(Monkey &monkey, int start_bit, int device_number) {
    vector<bool> temp, candidate;
    int new_fitness;
    start_bit = 0;
    candidate.assign(monkey.position.begin(), monkey.position.end());
    temp.resize(device_number);
    write_vector_range(temp, monkey.position, start_bit);
    temp = large_step(temp);
    write_vector_range(candidate, temp, start_bit);
    new_fitness = one_max(candidate);
    evaluation_times++;
    if (new_fitness > monkey.fitness) {
        monkey.position.assign(candidate.begin(), candidate.end());
        monkey.fitness = new_fitness;
    }
    
}

void watch_jump(vector<Monkey> &monkeys) {
    for (vector<Monkey>::iterator each_monkey = monkeys.begin(); each_monkey != monkeys.end(); ++each_monkey) {
        for (int jump_number = 0; jump_number < max_number; ++jump_number) {
            jump(*each_monkey, 0, gateway_number);
        }
        for (int jump_number = 0; jump_number < max_number; ++jump_number) {
            jump(*each_monkey, gateway_number, fog_server_number);
        }
        for (int jump_number = 0; jump_number < max_number; ++jump_number) {
            jump(*each_monkey, gateway_number + fog_server_number, edge_server_number);
        }
    }
}

void cooperation(vector<Monkey> &monkeys) {
    vector<bool> temp;
    vector<bool> best_position;
    vector<Monkey>::iterator best_monkey;
    int best_fitness = 0, temp_fitness = 0;

    for (vector<Monkey>::iterator each_monkey = monkeys.begin(); each_monkey != monkeys.end(); ++each_monkey) {
        if (each_monkey->fitness > best_fitness) {
            best_fitness = each_monkey->fitness;
            best_monkey = each_monkey;
        }
    }

    best_position = best_monkey->position;

    for (vector<Monkey>::iterator each_monkey = monkeys.begin(); each_monkey != monkeys.end(); ++each_monkey) {
        for (int cooperation_number = 0; cooperation_number < max_number; ++cooperation_number) {
            temp.resize(each_monkey->position.size(), false);
            for (int each_bit = 0; each_bit < each_monkey->position.size(); ++each_bit) {
                if (rand() % 2 == 0) {
                    temp[each_bit] = best_monkey->position[each_bit] - each_monkey->position[each_bit];
                }
                temp[each_bit] = temp[each_bit] + each_monkey->position[each_bit];
            }
            temp_fitness = one_max(temp);
            evaluation_times++;
            if (temp_fitness > each_monkey->fitness) {
                each_monkey->fitness = temp_fitness;
                each_monkey->position = temp;
            }
        }
    }
}

void crossover_mutation(vector<Monkey> &monkeys) {
    int random_bit;
    int random_monkey1;
    int random_monkey2;
    int temp_fitness;
    vector<Monkey> candidate = monkeys;

    for (int crossover_number = 0; crossover_number < parents_number; ++crossover_number) {
        random_monkey1 = rand() % monkeys.size();
        random_monkey2 = rand() % monkeys.size();
        random_bit = rand() % candidate[0].position.size();

        swap(candidate[random_monkey1].position[random_bit], candidate[random_monkey2].position[random_bit]);
    }

    for (vector<Monkey>::iterator each_candidate = candidate.begin(); each_candidate != candidate.end(); ++each_candidate) {
        if (rand() / RAND_MAX < mutation_rate) {
            random_bit = rand() % each_candidate->position.size();
            each_candidate->position[random_bit] = !each_candidate->position[random_bit];
        }
        each_candidate->fitness = one_max(each_candidate->position);
        evaluation_times++;
    }
    for (int each_monkey = 0; each_monkey < candidate.size(); ++each_monkey) {
        if (candidate[each_monkey].fitness > monkeys[each_monkey].fitness) {
            monkeys[each_monkey].position = candidate[each_monkey].position;
            monkeys[each_monkey].fitness = candidate[each_monkey].fitness;
        }
    }
}

void somersault(vector<Monkey> &monkeys) {
    vector<float> accumulator(monkeys[0].position.size(), 0.0);
    vector<bool> pivot(accumulator.size(), false);
    vector<bool> temp(accumulator.size(), false);
    int new_fitness, random_monkey;
    bool same_flag = true;

    for (vector<Monkey>::iterator each_monkey = monkeys.begin(); each_monkey != monkeys.end(); ++each_monkey) {
        for (int each_bit = 0; each_bit < accumulator.size(); ++each_bit) {
            accumulator[each_bit] += each_monkey->position[each_bit];
        }
    }

    for (int each_bit = 0; each_bit < accumulator.size(); ++each_bit) {
        accumulator[each_bit] /= monkeys.size();
        pivot[each_bit] = round(accumulator[each_bit]);
    }

    for (vector<Monkey>::iterator each_monkey = monkeys.begin(); each_monkey != monkeys.end(); ++each_monkey) {
        for (int somersault_number = 0; somersault_number < max_number; ++ somersault_number) {
            for (int each_bit = 0; each_bit < each_monkey->position.size(); ++ each_bit) {
                if (rand() % 2 == 0) {
                    temp[each_bit] = pivot[each_bit] - each_monkey->position[each_bit];
                }
            }

            new_fitness = one_max(each_monkey->position);
            evaluation_times++;
            if (new_fitness > each_monkey->fitness) {
                each_monkey->fitness = new_fitness;
                each_monkey->position = temp;
            }
        }
    }

    for (vector<Monkey>::iterator each_monkey = monkeys.begin(); each_monkey != monkeys.end(); ++each_monkey) {
        for (int each_bit = 0; each_bit < each_monkey->position.size(); ++each_bit) {
            if (each_monkey->position[each_bit] ^ pivot[each_bit]) {
                same_flag = false;
                break;
            }
        }
        if (same_flag) {
            random_monkey = rand() % monkeys.size();
            for (int each_bit = 0; each_bit < monkeys[random_monkey].position.size(); ++each_bit) {
                monkeys[random_monkey].position[each_bit] = !monkeys[random_monkey].position[each_bit];
            }
        }
        same_flag = true;
    }
}

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

template <typename T> vector<T> read_vector_range(vector<T> vec, int start_index, int end_index) {
    return vector<T>(vec.begin() + start_index, vec.begin() + end_index + 1);
}

template <typename T> void write_vector_range(vector<T> &target_vec, vector<T> source_vec, int start_index) {

    for (int index = 0; index < min(target_vec.size(), source_vec.size()); ++index) {
        target_vec[index + start_index] = source_vec[index];
    }
}