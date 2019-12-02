#include "tsp.h"

using namespace std;

vector<City> cities;

void input() {
    std::ifstream infile("dataset/eil51.txt");
    double x, y;
    int counter = 0, number;

    cities.resize(51);

    while(infile >> number >> x >> y) {
        if (counter < 51) {
            cities[counter].x = x;
            cities[counter].y = y;
        }
        counter++;
    }

    infile.close();
    infile.clear();
}

City::City() {
    x = 0;
    y = 0;
}

inline double calc_distance(int x1, int y1, int x2, int y2) {
    return (sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)));
}

double cost_evaluation(vector<int> &route) {
    double distance = 0;

    for (int each_city = 0; each_city < route.size() + 1; ++each_city) {
        distance += calc_distance(cities[route[each_city]].x, cities[route[each_city]].y, cities[route[(each_city + 1) % route.size()]].x, cities[route[(each_city + 1) % route.size()]].y);
    }

    return distance;
}