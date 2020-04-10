#include "ga.h"

int evaluate_times = 0;

void initialization(vector<Chromosome> &);
vector<Chromosome> selection(vector<Chromosome>, int);
vector<Chromosome> crossover(vector<Chromosome>, float);
vector<Chromosome> mutation(vector<Chromosome>, float);
void evaluation(vector<Chromosome> &);
Chromosome determination(vector<Chromosome>, Chromosome);

void ga(int max_evaluate_times, int dimension, int chromosome_quantity, int player_quantity, float crossover_rate, float mutation_rate) {
    vector<Chromosome> chromosomes(chromosome_quantity, Chromosome(dimension));
    Chromosome best_chromosome = Chromosome(dimension);

    initialization(chromosomes);

    while (evaluate_times < max_evaluate_times) {
        chromosomes = selection(chromosomes, player_quantity);
        chromosomes = crossover(chromosomes, crossover_rate);
        chromosomes = mutation(chromosomes, mutation_rate);
        evaluation(chromosomes);
        best_chromosome = determination(chromosomes, best_chromosome);

        cout << evaluate_times << "," << best_chromosome.fitness << endl;
    }
}

void initialization(vector<Chromosome> &chromosomes) {
    for (vector<Chromosome>::iterator each_chromosome = chromosomes.begin(); each_chromosome != chromosomes.end(); ++each_chromosome) {
        for (vector<bool>::iterator each_bit = each_chromosome->dna.begin(); each_bit != each_chromosome->dna.end(); ++each_bit) {
            *each_bit = rand() % 2;
        }
        each_chromosome->fitness = cost_evaluation(each_chromosome->dna);
    }
}

bool sort_by_fitness(Chromosome &first, Chromosome &second) {
    return first.fitness < second.fitness;
}

vector<Chromosome> selection(vector<Chromosome> chromosomes, int tournament_size) {
    double probability = 0.8;
    vector<Chromosome> tournaments;
    vector<Chromosome> winners;

    while (winners.size() < chromosomes.size()) {
        tournaments.clear();
        for (int number = 0; number < tournament_size; ++number) {
            tournaments.push_back(chromosomes.at(rand() % chromosomes.size()));
        }
        sort(tournaments.begin(), tournaments.end(), sort_by_fitness);
        for (int round = 0; round < tournament_size; ++round) {
            if (((double)rand() / (double)RAND_MAX) < (probability * pow(1 - probability, round))) {
                winners.push_back(tournaments.at(round));
                break;
            }
        }
    }

    return winners;
}

vector<Chromosome> crossover(vector<Chromosome> children, float crossover_rate) {
    int index1, index2, start, interval = 5;
    bool temp;

    for (int number = 0; number < children.size() / 2; ++number) {
        if ((double)rand() / (double)DBL_MAX < crossover_rate) {
            index1 = number * 2;
            index2 = index1 + 1;
            start = rand() % children.at(index1).dna.size();
            for (int bit = start; bit < start + interval; ++bit) {
                temp = children.at(index1).dna.at(bit % children.at(index1).dna.size());
                children.at(index1).dna.at(bit % children.at(index1).dna.size()) = children.at(index2).dna.at(bit % children.at(index2).dna.size());
                children.at(index2).dna.at(bit % children.at(index2).dna.size()) = temp;
            }
        }
    }

    return children;
}

vector<Chromosome> mutation(vector<Chromosome> children, float mutation_rate) {
    int index;
    for (int number = 0; number < children.size(); ++number) {
        if ((double)rand() / (double)DBL_MAX < mutation_rate) {
            index = rand() % children.at(number).dna.size();
            children.at(number).dna.at(index) = !children.at(number).dna.at(index);
        }
    }

    return children;
}

void evaluation(vector<Chromosome> &chromosomes) {
    for (vector<Chromosome>::iterator each_chromosome = chromosomes.begin(); each_chromosome != chromosomes.end(); ++each_chromosome) {
        each_chromosome->fitness = cost_evaluation(each_chromosome->dna);
        evaluate_times++;
    }
}

Chromosome determination(vector<Chromosome> candidates, Chromosome winner) {
    for (vector<Chromosome>::iterator each_candidate = candidates.begin(); each_candidate != candidates.end(); ++each_candidate) {
        if (each_candidate->fitness < winner.fitness) {
            winner = *each_candidate;
        }
    }

    return winner;
}

Chromosome::Chromosome(int dna_length) {
    this->dna.resize(dna_length);
    this->fitness = DBL_MAX;
};