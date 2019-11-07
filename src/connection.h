#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <time.h>

using namespace std;

extern int gateway_number;
extern int fog_device_number;
extern int edge_device_number;
extern int agv_number;

class Gate_way {
    public:
        Gate_way();

        int x;
        int y;
};

class Fog_device {
    public:
        Fog_device();

        int x;
        int y;
};

class Edge_device {
    public:
        Edge_device();
        
        int x;
        int y;
};

class Agv {
    public:
        Agv();

        int x;
        int y;
};

extern vector<Gate_way> gate_ways;
extern vector<Fog_device> fog_devices;
extern vector<Edge_device> edge_devices;
extern vector<Agv> agvs;
// vector<vector<long double> > distance_table;

void input();
void build();
void connect(vector<bool>);

#endif