#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <time.h>
#include <float.h>

using namespace std;

extern int gate_way_number;
extern int fog_device_number;
extern int edge_device_number;
extern int agv_number;

class Gate_way {
public:
    Gate_way();

    int x;
    int y;
    bool status;
    int link_quantity;
    int demand;

    void reset() {
        link_quantity = 0;
        demand = 0;
    }
};

class Fog_device {
public:
    Fog_device();

    int x;
    int y;
    bool status;
    Gate_way *link;
    int link_quantity;
    int demand;
    double distance;

    void reset() {
        link_quantity = 0;
        link = NULL;
        demand = 0;
    }
};

class Edge_device {
public:
    Edge_device();
    
    int x;
    int y;
    bool status;
    Fog_device *link;
    int link_quantity;
    int demand;
    double distance;

    void reset() {
        link_quantity = 0;
        link = NULL;
        demand = 0;
    }
};

class Agv {
public:
    Agv();

    int x;
    int y;
    int demand;
    Edge_device *link;
    double distance;

    void reset() {
        link = NULL;
    }
};

extern vector<Gate_way> gate_ways;
extern vector<Fog_device> fog_devices;
extern vector<Edge_device> edge_devices;
extern vector<Agv> agvs;

void input(char *);
void connect(vector<bool>);
double cost_evaluation(vector<bool>);
void find_loading(vector<bool>);

#endif