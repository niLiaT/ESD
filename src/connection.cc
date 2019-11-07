#include "connection.h"

using namespace std;

int gateway_number;
int fog_device_number;
int edge_device_number;
int agv_number;

vector<Gate_way> gate_ways;
vector<Fog_device> fog_devices;
vector<Edge_device> edge_devices;
vector<Agv> agvs;

Gate_way::Gate_way() {
    x = 0;
    y = 0;
}

Fog_device::Fog_device() {
    x = 0;
    y = 0;
}

Edge_device::Edge_device() {
    x = 0;
    y = 0;
}

Agv::Agv() {
    x = 0;
    y = 0;
}

void input() {
    std::ifstream infile("dataset/3_15_70_500.txt");

    double x, y;
    int counter = 0;
    gate_ways.resize(gateway_number);
    fog_devices.resize(fog_device_number);
    edge_devices.resize(edge_device_number);
    agvs.resize(agv_number);

    while(infile >> x >> y) {
        if (counter < gateway_number) {
            gate_ways[counter].x = x;
            gate_ways[counter].y = y;
        }
        else if ((counter - gateway_number) < fog_device_number) {
            fog_devices[counter - gateway_number].x = x;
            fog_devices[counter - gateway_number].y = y;
        }
        else if ((counter - gateway_number - fog_device_number) < edge_device_number) {
            edge_devices[counter - gateway_number - fog_device_number].x = x;
            edge_devices[counter - gateway_number - fog_device_number].y = y;
        }
        else {
            agvs[counter - gateway_number - fog_device_number - edge_device_number].x = x;
            agvs[counter - gateway_number - fog_device_number - edge_device_number].y = y;
        }
        counter++;
    }

    infile.close();
    infile.clear();
}

void build() {
    gateway_number = 3;
    fog_device_number = 15;
    edge_device_number = 70;
    agv_number = 500;
    
    input();
}

void connect(vector<bool> solution) {
    for (vector<Agv>::iterator each_agv = agvs.begin(); each_agv != agvs.end(); ++each_agv) {
        
    }
}