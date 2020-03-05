#include "se.h"

int evaluate_times = 0;

void se(int max_evaluate_times, int dimension, int region_quantity, int searcher_quantity, int good_quantity, int player_quantity) {
    vector<Region> regions; //Regions of a market
    Good optimal_good = Good(0);

    regions = initialization(region_quantity, searcher_quantity, good_quantity, dimension);

    resource_arrangement(regions); //Section II.C in the paper

    while (evaluate_times < max_evaluate_times) {
        vision_search(regions, player_quantity); //Section II.D in the paper

        optimal_good = marketing_research(regions); //Section II.E in the paper

        cout << evaluate_times << "," << optimal_good.price << endl;
    }
}

vector<Region> initialization(int region_quantity, int searcher_quantity, int good_quantity, int dimension) {
    vector<Region> sub_markets(region_quantity, Region(searcher_quantity, good_quantity, dimension));

    for (vector<Region>::iterator each_submarket = sub_markets.begin(); each_submarket != sub_markets.end(); ++each_submarket) {
        each_submarket->best_good.price = DBL_MAX;
        for (vector<Good>::iterator each_good = each_submarket->goods.begin(); each_good != each_submarket->goods.end(); ++each_good) {
            if (each_good->price < each_submarket->best_good.price) {
                each_submarket->best_good = *each_good;
            }
        }
    }

    return sub_markets;
}

void resource_arrangement(vector<Region> &regions) {
    //Divide the market by the gate way numbers
    int quota = floor(gate_way_number / regions.size());
    for (int each_region = 0; each_region < regions.size(); ++each_region) {
        regions[each_region].id_bits_range = make_tuple(each_region * quota, (each_region + 1) * quota - 1);
    }
    get<1>(regions[regions.size()].id_bits_range) += gate_way_number % regions.size();

    //Allocate goods into each region
    int random_number;
    vector<bool> temp(gate_way_number), reset(gate_way_number, false);
    for (vector<Region>::iterator each_region = regions.begin(); each_region != regions.end(); ++each_region) {
        for (vector<Good>::iterator each_good = each_region->goods.begin(); each_good != each_region->goods.end(); ++each_good) {
            temp = reset;
            random_number = rand() % (get<1>(each_region->id_bits_range) - get<0>(each_region->id_bits_range) + 1) + get<0>(each_region->id_bits_range);
            for (int each_bit = 0; each_bit < random_number; ++each_bit) {
                temp.at(each_bit) = true;
            }
            random_shuffle(temp.begin(), temp.end());
            for (int each_bit = 0; each_bit < gate_way_number; ++each_bit) {
                each_good->utility.at(each_bit) = temp.at(each_bit);
            }
        }
    }
}

void vision_search(vector<Region> &regions, int player_quantity) {
    int random_index = 0;
    double investment_record = 0.0; //Mu
    double average_profit = 0.0; //Nu
    double best_price = 0.0; //Rho
    double total_price = 0.0;
    vector<Searcher>::iterator worst_searcher;
    vector<Region>::iterator defending_champion;
    vector<Region>::iterator challenger;

    //Transition
    for (vector<Region>::iterator each_region = regions.begin(); each_region != regions.end(); ++each_region) {
        for (vector<Searcher>::iterator each_searcher = each_region->searchers.begin(); each_searcher != each_region->searchers.end(); ++ each_searcher) {
            random_index = rand() % each_region->goods.size();

            /*Searchers transit its investment with the utility of a random good.
              The old investment will be replaced if it is worst than the new one, 
              and the new good will be pushed into the candidate list to be determined.*/
            each_region->candidate_goods.push_back(each_searcher->invest(each_region->goods[random_index]));
            evaluate_times++;
        }

        each_region->reset_id_bits();
    }

    //Calculate expected value
    for (vector<Region>::iterator each_region = regions.begin(); each_region != regions.end(); ++each_region) {
        //Calculate mu, formula (2)
        investment_record = (double)(each_region->univested_times) / (double)(each_region->invested_times);

        //Calculate nu, formula (3)
        average_profit = 0;
        worst_searcher = each_region->searchers.begin();
        for (vector<Searcher>::iterator each_searcher = each_region->searchers.begin(); each_searcher != each_region->searchers.end(); ++each_searcher) {
            average_profit += each_searcher->profit;
            if (each_searcher->profit > worst_searcher->profit) {
                worst_searcher = each_searcher;
            }
        }
        average_profit /= each_region->searchers.size();
        average_profit /= worst_searcher->profit;
    
        //Calculate rho, formula (4)
        total_price = 0;
        for (vector<Good>::iterator each_good = each_region->goods.begin(); each_good != each_region->goods.end(); ++each_good) {
            total_price += each_good->price;
        }
        best_price = each_region->best_good.price / total_price;

        //Calculate the expected value, formula (1)
        each_region->expected_value = investment_record * average_profit * best_price;
    }

    //Determination by tournament
    for (vector<Region>::iterator each_region = regions.begin(); each_region != regions.end(); ++each_region) {
        each_region->candidate_searchers.clear();
    }

    for (vector<Region>::iterator each_region = regions.begin(); each_region != regions.end(); ++each_region) {
        for (vector<Searcher>::iterator each_searcher = each_region->searchers.begin(); each_searcher != each_region->searchers.end(); ++each_searcher) {
            defending_champion = regions.begin() + (rand() % regions.size());
            for (int round = 0; round < player_quantity - 1; ++round) {
                challenger = regions.begin() + (rand() % regions.size());
                if (challenger->expected_value > defending_champion->expected_value) {
                    defending_champion = challenger;
                }
            }

            defending_champion->candidate_searchers.push_back(*each_searcher);
        }
    }

    for (vector<Region>::iterator each_region = regions.begin(); each_region != regions.end(); ++each_region) {
        each_region->searchers = each_region->candidate_searchers;
    }
}

Good marketing_research(vector<Region> &regions) {
    vector<Good>::iterator worst_good;
    Good optimal = Good(0);
    optimal.price = DBL_MAX;

    for (vector<Region>::iterator each_region = regions.begin(); each_region != regions.end(); ++each_region) {
        //Update the candidate good of each region
        for (vector<Good>::iterator each_candidate = each_region->candidate_goods.begin(); each_candidate != each_region->candidate_goods.end(); ++each_candidate) {
            worst_good = each_region->goods.begin();
            for (vector<Good>::iterator each_good = each_region->goods.begin() + 1; each_good != each_region->goods.end(); ++each_good) {
                if (worst_good->price < each_good->price) {
                    worst_good = each_good;
                }
            }

            if (each_candidate->price < worst_good->price) {
                *worst_good = *each_candidate;
            }
        }

        //Find the best good of a region
        for (vector<Good>::iterator each_good = each_region->goods.begin(); each_good != each_region->goods.end(); ++each_good) {
            if (each_good->price < each_region->best_good.price) {
                each_region->best_good = *each_good;
            }
        }

        //Find the best good of the market
        if (each_region->best_good.price < optimal.price) {
            optimal = each_region->best_good;
        }

        //Update the investment record
        if (each_region->searchers.size() > 0) {
            each_region->univested_times = 1;
            each_region->invested_times += each_region->searchers.size();
        }
        else {
            each_region->invested_times = 1;
            each_region->univested_times += 1;
        }
    }

    return optimal;
}

Searcher::Searcher(int dimension) {
    //Initialize the investment and its profit of a searcher
    this->investment.resize(dimension);
    for (vector<bool>::iterator each_dimension = (this->investment).begin(); each_dimension != (this->investment).end(); ++each_dimension) {
        *each_dimension = rand() % 2;
    }
    this->profit = cost_evaluation(this->investment);
};

Good Searcher::invest(Good good) {
    bool temp;
    int start_bit = rand() % good.utility.size();
    int length = 20, random_index;
    int size = good.utility.size();
    vector<bool> id_bits(gate_way_number);

    this->candidate_investment = this->investment;
    for (int each_bit = 0; each_bit < id_bits.size(); ++each_bit) {
        id_bits.at(each_bit) = good.utility.at(each_bit);
    }

    //Crossover
    for (int index = 0; index < length; ++index) {
        temp = this->candidate_investment[(start_bit + index) % size];
        this->candidate_investment[(start_bit + index) % size] = good.utility[(start_bit + index) % size];
        good.utility[(start_bit + index) % size] = temp;
    }

    //Mutation
    random_index = rand() % size;
    this->candidate_investment[random_index] = ~this->candidate_investment[random_index];
    good.utility[random_index] = ~good.utility[random_index];

    for (int each_bit = 0; each_bit < id_bits.size(); ++each_bit) {
        good.utility.at(each_bit) = id_bits.at(each_bit);
    }

    //Update investment
    good.price = cost_evaluation(good.utility);
    this->candidate_profit = cost_evaluation(this->candidate_investment);
    if (this->candidate_profit < this->profit) {
        this->profit = this->candidate_profit;
        this->investment = this->candidate_investment;
    }

    return good;
}

Good::Good(int dimension) {
    //Initialization the good utility and its price of a good
    this->utility.resize(dimension);
    for (vector<bool>::iterator each_dimension = (this->utility).begin(); each_dimension != (this->utility).end(); ++each_dimension) {
        *each_dimension = rand() % 2;
    }
    this->price = cost_evaluation(this->utility);
};

Region::Region(int searcher_quantity, int goods_quantity, int dimension) {
    this->searchers.resize(searcher_quantity, Searcher(dimension));
    this->goods.resize(goods_quantity, Good(dimension));
    this->invested_times = 1;
    this->univested_times = 1;
    this->expected_value = 0.0;
};

void Region::reset_id_bits () {
    // int random_number;
    // vector<bool> temp(gate_way_number), reset(gate_way_number, false);
    // Good temp_good(0);

    // for (vector<Good>::iterator each_good = this->goods.begin(); each_good != this->goods.end(); ++each_good) {
    //     temp = reset;
    //     random_number = rand() % (get<1>(this->id_bits_range) - get<0>(this->id_bits_range) + 1) + get<0>(this->id_bits_range);
    //     for (int each_bit = 0; each_bit < random_number; ++each_bit) {
    //         temp.at(each_bit) = true;
    //     }
    //     random_shuffle(temp.begin(), temp.end());
    //     temp_good = *each_good;
    //     for (int each_bit = 0; each_bit < temp.size(); ++each_bit) {
    //         temp_good.utility.at(each_bit) = temp.at(each_bit);
    //     }
    //     temp_good.price = cost_evaluation(temp_good.utility);
    //     if (temp_good.price < each_good->price) {
    //         *each_good = temp_good;
    //     }
    // }

    for (vector<Good>::iterator each_good = this->goods.begin(); each_good != this->goods.end(); ++each_good) {
        int temp_price;
        int random_index = rand() % gate_way_number;
        each_good->utility.at(random_index) = ~each_good->utility.at(random_index);
        temp_price = cost_evaluation(each_good->utility);
        if (temp_price < each_good->price) {
            each_good->price = temp_price;
        }
        else {
            each_good->utility.at(random_index) = ~each_good->utility.at(random_index);
        }
    }
}