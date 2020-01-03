#include "bat.h"

#define alpha 0.98
#define gamma 0.98

int evaluation_times = 0;

void bat(const int iterations, int dimension, int bat_quantity) {
    Bat best_bat(dimension);

    build();

    vector<Bat> bats(bat_quantity, Bat(dimension));

    for (vector<Bat>::iterator each_bat = bats.begin(); each_bat != bats.end(); ++each_bat) {
        each_bat->initialization();
    }

    get_best_bat(bats, best_bat);

    while (evaluation_times < iterations) {

        echolocation(bats, best_bat);

        get_best_bat(bats, best_bat);
        
        cout << evaluation_times << "," << best_bat.fitness << endl;
    }
}

Bat::Bat(int dimension) {
    position.resize(dimension);
    velocity.resize(dimension);
    candidate.resize(dimension);
    distribution.resize(dimension);

    fitness = DBL_MAX;
}

void Bat::initialization() {
    for (vector<bool>::iterator each_bit = position.begin(); each_bit != position.end(); ++each_bit) {
        *each_bit = rand() % 2;
    }

    for (vector<double>::iterator each_bit = velocity.begin(); each_bit != velocity.end(); ++each_bit) {
        *each_bit = 0;
    }

    max_frequency = 1;
    min_frequency = 0;
    loudness = (double)rand() / (RAND_MAX + 1.0);
    pulse_rate = (double)rand() / (RAND_MAX + 1.0);
    fitness = cost_evaluation(position);
}

void echolocation(vector<Bat> &bats, Bat leader) {
    for (vector<Bat>::iterator each_bat = bats.begin(); each_bat != bats.end(); ++each_bat) {
        each_bat->movement(leader);
        evaluation_times++;
    }
}

void Bat::movement(Bat leader) {
    if ((double)rand() / (RAND_MAX + 1.0) > pulse_rate) {
        for (int each_bit = 0; each_bit < distribution.size(); ++each_bit) {
            distribution[each_bit] = position[each_bit] + (2 * ((double)rand() / (RAND_MAX + 1.0)) - 1) * ((double)rand() / (RAND_MAX + 1.0));
            if ((double)rand() / (RAND_MAX + 1.0) < 1 / (1 + exp(-distribution[each_bit])))
                candidate[each_bit] = 1;
            else
                candidate[each_bit] = 0;
        }
        for (int each_bit = 0; each_bit < position.size(); ++each_bit) {
            position[each_bit] = candidate[each_bit];
        }
    }

    candidate_fitness = cost_evaluation(candidate);

    if ((double)rand() / (RAND_MAX + 1.0) < loudness && candidate_fitness < leader.fitness) {
        frequency = min_frequency + (max_frequency - min_frequency) * ((double)rand() / (RAND_MAX + 1.0));
        for (int each_bit = 0; each_bit < velocity.size(); ++each_bit) {
            velocity[each_bit] = velocity[each_bit] + (position[each_bit] - leader.position[each_bit]) * frequency;
        }
        for (int each_bit = 0; each_bit < distribution.size(); ++each_bit) {
            distribution[each_bit] = velocity[each_bit];
            if ((double)rand() / (RAND_MAX + 1.0) < (1 / (1 + exp(-distribution[each_bit]))))
                candidate[each_bit] = 1;
            else
                candidate[each_bit] = 0;
        }
        for (int each_bit = 0; each_bit < position.size(); ++each_bit) {
            position[each_bit] = candidate[each_bit];
        }
    }

    fitness = cost_evaluation(position);
}

void get_best_bat(vector<Bat> &bats, Bat &best_bat) {
    for (vector<Bat>::iterator each_bat = bats.begin(); each_bat != bats.end(); ++each_bat) {
        if (each_bat->fitness < best_bat.fitness) {
            best_bat = *each_bat;
        }
    }
}