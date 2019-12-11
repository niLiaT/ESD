#include "bat.h"

#define alpha 0.98
#define gamma 0.98

int evaluation_times = 0;

// vector<Bat>::iterator best_bat;
vector<Bat>::iterator best_bat;

void bat(const int iterations, int dimension, int population) {
    vector<Bat> bats(population, Bat(dimension));
    // int counter = 0;


    initialization(bats);
    best_bat = get_best_bats(bats);

    while (evaluation_times < iterations) {
        echolocation(bats);
        best_bat = get_best_bats(bats);
        // cout << counter << " " << best_bat->fitness << endl;
        // counter++;
        cout << evaluation_times << "," << best_bat->fitness << endl;
    }
}

Bat::Bat(int dimension) {
    position.resize(dimension);
}

int hamming_distance(vector<int> position1, vector<int> position2) {
    int distance = 0;

    for (int index = 0; index < position1.size(); ++index) {
        // if (position1[index] ^ position2[index]) {
        if (position1[index] != position2[index]) {
            distance++;
        }
    }

    return distance;
}

void initialization(vector<Bat> &bats) {
    for (vector<Bat>::iterator each_bat = bats.begin(); each_bat != bats.end(); ++each_bat) {
        each_bat->initialized();
    }
}

void echolocation(vector<Bat> &bats) {
    for (vector<Bat>::iterator each_bat = bats.begin(); each_bat != bats.end(); ++each_bat) {
        each_bat->locate();
        each_bat->movement();
    }
}

void Bat::initialized() {
    // for (vector<bool>::iterator each_bit = position.begin(); each_bit != position.end(); ++each_bit) {
        // *each_bit = rand() % 2;
    // }

    for (int each_index = 0; each_index < position.size(); ++each_index) {
        position[each_index] = each_index;
    }

    random_shuffle(position.begin(), position.end());

    candidate_position = position;
    pulse_rate = 0.4 * rand() / RAND_MAX;
    init_pulse_rate = pulse_rate;
    loudness = 0.3 * rand() / RAND_MAX + 0.7;
    // velocity = rand() % hamming_distance(position, best_bat->position) + 1;
    fitness = cost_evaluation(position);
    candidate_fitness = 0;
    time_step = 0;
}

void two_opt(vector<int> solution, vector<int> &candidate) {
    int random1, random2, smaller, larger;

    random1 = rand() % solution.size();
    random2 = rand() % solution.size();
    smaller = min(random1, random2);
    larger = max(random1, random2);

    vector<int>::iterator first = solution.begin() + smaller;
    vector<int>::iterator last = solution.begin() + larger;

    reverse(first, last);

    candidate = solution;
}

void three_opt(vector<int> solution, vector<int> &candidate) {
    vector<int> random(6);
    vector<int> index(solution.size());

    vector<int>::iterator first;
    vector<int>::iterator last;

    for (int each_index = 0; each_index < index.size(); ++each_index) {
        index[each_index] = each_index;
    }
    random_shuffle(index.begin(), index.end());

    for (int each_index = 0; each_index < random.size(); ++each_index) {
        random[each_index] = index[each_index];
    }
    sort(random.begin(), random.end());

    for (int round = 0; round < 3; ++round) {
        first = solution.begin() + random[round * 2];
        last = solution.begin() + random[round * 2 + 1];

        reverse(first, last);
    }
    candidate = solution;
}

void Bat::locate() {
    vector<int> temp_position;
    double temp_fitness;

    // random_device device;
    // default_random_engine generator(device());
    // uniform_int_distribution<int> uniform(0, hamming_distance(position, best_bat->position));

    // cout << hamming_distance(position, best_bat->position) << endl;

    candidate_fitness = DBL_MAX;

    if (&(*best_bat) != this)
        velocity = rand() % hamming_distance(position, best_bat->position);

    else
        velocity = 0;
    // velocity = uniform(generator) + 1;

    if (velocity < position.size() / 2) {
        for (int each_neighbor = 0; each_neighbor < velocity; ++each_neighbor) {
            two_opt(position, temp_position);
            if ((temp_fitness = cost_evaluation(temp_position)) < candidate_fitness) {
                candidate_fitness = temp_fitness;
                candidate_position = temp_position;
            }
        }
    }
    else {
        for (int each_neighbor = 0; each_neighbor < velocity; ++each_neighbor) {
            three_opt(position, temp_position);
            if ((temp_fitness = cost_evaluation(temp_position)) < candidate_fitness) {
                candidate_fitness = temp_fitness;
                candidate_position = temp_position;
            }
        }
    }
    
    evaluation_times++;

    if (rand() / RAND_MAX > pulse_rate) {
        two_opt(temp_position, temp_position);
    }
}

void Bat::movement() {
    if (rand() / RAND_MAX < loudness && candidate_fitness < fitness) {
        fitness = candidate_fitness;
        position = candidate_position;

        loudness = alpha * loudness;
        pulse_rate = init_pulse_rate * (1 - exp(-(gamma * time_step)));
        time_step++;
    }
}

vector<Bat>::iterator get_best_bats(vector<Bat> &bats) {
    vector<Bat>::iterator best = bats.begin();

    for (vector<Bat>::iterator each_bat = bats.begin() + 1; each_bat != bats.end(); ++each_bat) {
        if (each_bat->fitness < best->fitness) {
            best = each_bat;
        }
    }

    return best;
}

int main() {
    srand(time(NULL));

    input();
    bat(250000, 51, 50);
}