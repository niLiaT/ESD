#ifndef SE_H_
#define SE_H_

#include <iostream>
#include <vector>
#include "connection.h"
#include <algorithm>
#include <tuple>

using namespace std;

class Good {
public:
    Good();
    Good(int);
    vector<bool> utility; //Solution form of a good
    double price; //Fitness value of a good
    int id_bits_number; //Numbers of identity bits
};

class Searcher {
public:
    Searcher();
    Searcher(int);
    vector<bool> investment; //Solution held by a searcher
    vector<bool> candidate_investment; //Temporary investment
    double profit; //Fitness value of a searcher's investment
    double candidate_profit; //Fitness value of a temporary investment
    Good invest(Good); //A function for investing a good
};

class Region {
public:
    Region(int, int, int);
    int invested_times; //t_a
    int univested_times; //t_b
    vector<Searcher> searchers; //Searchers who invest in the region
    vector<Searcher> candidate_searchers; //Temporary searchers of the region
    vector<Good> goods; //Goods of the region
    vector<Good> candidate_goods; //Temporary goods of the region
    // vector<bool> id_bits; //Identity bits
    pair<int, int> id_bits_range; // The range of identity bits number of the region
    vector<Good>::iterator best_good; //Best good of a region
    double mu, nu, rho;
    double expected_value; //E
    void reset_id_bits(); //Reset identity bits to insure a good still stay in the region after 
};

void se(int, int, int, int, int, int);
vector<Region> initialization(int, int, int, int);
void resource_arrangement(vector<Region> &);
void vision_search(vector<Region> &, int);
Good marketing_research(vector<Region> &);

#endif