#include "se.h"

vector<bool> decimal_to_binary(int, int);

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
    double id_bits_number = ceil(log2(regions.size())); //Identity bits length

    for (int each_region = 0; each_region < regions.size(); ++each_region) {
        //Divide the market by the identity bits in binary
        (regions[each_region].id_bits).resize(id_bits_number);
        regions[each_region].id_bits = decimal_to_binary(each_region, id_bits_number); //Change its order into binary as its identity bits

        //Allocate the goods into each regions by set the identity bits of each good
        for (vector<Good>::iterator each_good = regions[each_region].goods.begin(); each_good != regions[each_region].goods.end(); ++each_good) {
            for (int each_bit = 0; each_bit < id_bits_number; ++each_bit) {
                each_good->utility[each_bit] = regions[each_region].id_bits[each_bit];
            }
        }
    }
}

void vision_search(vector<Region> &regions, int player_quantity) {
    int random_index = 0;
    double investment_record = 0; //Mu
    double average_profit = 0; //Nu
    double best_price = 0; //Rho
    double total_price = 0;
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

        //Reset the identity bits
        for (vector<Good>::iterator each_good = each_region->goods.begin(); each_good != each_region->goods.end(); ++each_good) {
            for (int index = 0; index < each_region->id_bits.size(); ++index) {
                each_good->utility.at(index) = each_region->id_bits.at(index);
            }
        }
    }

    //Calculate expected value
    for (vector<Region>::iterator each_region = regions.begin(); each_region != regions.end(); ++each_region) {
        //Calculate mu, formula (2)
        investment_record = each_region->invested_times / each_region->univested_times;

        //Calculate nu, formula (3)
        average_profit = 0;
        for (vector<Searcher>::iterator each_searcher = each_region->searchers.begin(); each_searcher != each_region->searchers.end(); ++each_searcher) {
            average_profit += each_searcher->profit;
        }
        average_profit /= each_region->searchers.size();
    
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
                if (challenger->expected_value < defending_champion->expected_value) {
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
    // int length = rand() % good.utility.size(), random_index;
    int size = good.utility.size();

    this->candidate_investment = this->investment;

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

vector<bool> decimal_to_binary(int decimal_number, int length) {
    vector<bool> binary_number(length, false);
    int i = 0;
    while (decimal_number > 0) {
        binary_number[i] = decimal_number % 2;
        decimal_number /= 2;
        i++;
    }

    reverse(binary_number.begin(), binary_number.end());

    return binary_number;
}