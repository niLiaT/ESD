#include "bat.h"

#define alpha 0.98
#define gamma 0.98

int evaluation_times = 0;

void bat(const int iterations, int dimension, int bat_quantity) {
    Bat best_bat(dimension);

    vector<Bat> bats(bat_quantity, Bat(dimension));

    for (vector<Bat>::iterator each_bat = bats.begin(); each_bat != bats.end(); ++each_bat) {
        each_bat->initialization();
    }

    best_bat = get_best_bat(bats, best_bat);

    while (evaluation_times < iterations) {

        echolocation(bats, best_bat);

        best_bat = get_best_bat(bats, best_bat);
        
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
    for (int each_bit = 0; each_bit < this->distribution.size(); ++each_bit) {
        this->distribution[each_bit] = this->position[each_bit] + (2 * ((double)rand() / (RAND_MAX + 1.0)) - 1) * ((double)rand() / (RAND_MAX + 1.0));
        if ((double)rand() / (RAND_MAX + 1.0) < 1 / (1 + exp(-this->distribution[each_bit])))
            // this->candidate[each_bit] = ~this->candidate[each_bit];
            this->candidate[each_bit] = 1;
            // this->candidate[each_bit] = leader.position[each_bit];
        else
            this->candidate[each_bit] = 0;
    }

    if ((double)rand() / (RAND_MAX + 1.0) > pulse_rate) {
        this->position = this->candidate;
        this->fitness = cost_evaluation(this->position);
    }

    frequency = min_frequency + (max_frequency - min_frequency) * ((double)rand() / (RAND_MAX + 1.0));
    for (int each_bit = 0; each_bit < this->velocity.size(); ++each_bit) {
        this->velocity[each_bit] = this->velocity[each_bit] + (this->position[each_bit] - leader.position[each_bit]) * frequency;
    }
    for (int each_bit = 0; each_bit < this->distribution.size(); ++each_bit) {
        this->distribution[each_bit] = this->velocity[each_bit];
        if ((double)rand() / (RAND_MAX + 1.0) < (1 / (1 + exp(-this->distribution[each_bit]))))
            this->candidate[each_bit] = ~this->candidate[each_bit];
        //     this->candidate[each_bit] = 1;
        // else
        //     this->candidate[each_bit] = 0;
    }

    this->candidate_fitness = cost_evaluation(this->candidate);

    if ( (double)rand() / (RAND_MAX + 1.0) < loudness) {// && this->candidate_fitness <= leader.fitness) {
        this->position = this->candidate;
        this->fitness = this->candidate_fitness;
    }
}

Bat get_best_bat(vector<Bat> &bats, Bat defender) {
    for (vector<Bat>::iterator each_bat = bats.begin(); each_bat != bats.end(); ++each_bat) {
        if (each_bat->fitness < defender.fitness) {
            defender = *each_bat;
        }
    }

    return defender;
}