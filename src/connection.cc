#include "connection.h"

using namespace std;

int gate_way_number;
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
    status = false;
}

Fog_device::Fog_device() {
    x = 0;
    y = 0;
    status = false;
    link = NULL;
}

Edge_device::Edge_device() {
    x = 0;
    y = 0;
    status = false;
    link = NULL;
}

Agv::Agv() {
    x = 0;
    y = 0;
    link = NULL;
}

void input() {
    std::ifstream infile("dataset/3_15_70_500.txt");

    double x, y;
    int counter = 0;
    gate_ways.resize(gate_way_number);
    fog_devices.resize(fog_device_number);
    edge_devices.resize(edge_device_number);
    agvs.resize(agv_number);

    while(infile >> x >> y) {
        if (counter < gate_way_number) {
            gate_ways[counter].x = x;
            gate_ways[counter].y = y;
        }
        else if ((counter - gate_way_number) < fog_device_number) {
            fog_devices[counter - gate_way_number].x = x;
            fog_devices[counter - gate_way_number].y = y;
        }
        else if ((counter - gate_way_number - fog_device_number) < edge_device_number) {
            edge_devices[counter - gate_way_number - fog_device_number].x = x;
            edge_devices[counter - gate_way_number - fog_device_number].y = y;
        }
        else {
            agvs[counter - gate_way_number - fog_device_number - edge_device_number].x = x;
            agvs[counter - gate_way_number - fog_device_number - edge_device_number].y = y;
        }
        counter++;
    }

    infile.close();
    infile.clear();
}

void build() {
    gate_way_number = 3;
    fog_device_number = 15;
    edge_device_number = 70;
    agv_number = 500;
    
    input();
}

inline double distance(int x1, int y1, int x2, int y2) {
    return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}

void connect(vector<bool> solution) {
    double shortest = DBL_MAX;
    int counter = 0;
    double temp;
    void *layer;
    void *upper_layer;

    for (vector<bool>::iterator each_bit = solution.begin(); each_bit != solution.end(); ++each_bit) {
        if (counter < gate_way_number)
            gate_ways[counter].status = *each_bit;
        else if ((counter - gate_way_number) < fog_device_number)
            fog_devices[counter].status = *each_bit;
        else
            edge_devices[counter].status = *each_bit;
    }

    for (vector<Agv>::iterator each_agv = agvs.begin(); each_agv != agvs.end(); ++each_agv) {
        for (vector<Edge_device>::iterator each_edge_device = edge_devices.begin(); each_edge_device != edge_devices.end(); ++each_edge_device) {
            if ((*each_edge_device).status) {
                if ((temp = distance((*each_agv).x, (*each_agv).y, (*each_edge_device).x, (*each_edge_device).y)) < shortest) {
                    shortest = temp;
                    (*each_agv).link = &(*each_edge_device);
                }
            }
        }
    }

    shortest = DBL_MAX;

    for (vector<Edge_device>::iterator each_edge_device = edge_devices.begin(); each_edge_device != edge_devices.end(); ++each_edge_device) {
        for (vector<Fog_device>::iterator each_fog_device = fog_devices.begin(); each_fog_device != fog_devices.end(); ++each_fog_device) {
            if ((*each_fog_device).status) {
                if ((temp = distance((*each_edge_device).x, (*each_edge_device).y, (*each_fog_device).x, (*each_fog_device).y)) < shortest) {
                    shortest = temp;
                    (*each_edge_device).link = &(*each_fog_device);
                }
            }
        }
    }

    shortest = DBL_MAX;

    for (vector<Fog_device>::iterator each_fog_device = fog_devices.begin(); each_fog_device != fog_devices.end(); ++each_fog_device) {
        for (vector<Gate_way>::iterator each_gate_way = gate_ways.begin(); each_gate_way != gate_ways.end(); ++each_gate_way) {
            if ((*each_gate_way).status) {
                if ((temp = distance((*each_fog_device).x, (*each_fog_device).y, (*each_gate_way).x, (*each_gate_way).y)) < shortest) {
                    shortest = temp;
                    (*each_fog_device).link = &(*each_gate_way);
                }
            }
        }
    }
}