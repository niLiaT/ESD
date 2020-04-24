#include "connection.h"

#define fiber_cost 50
#define gate_way_cost 200
#define edge_cost 80
#define fog_cost 100
#define agv_data_rate 1000
#define edge_data_rate 4000
#define fog_data_rate 7000
#define edge_range 75
#define edge_capacity 20
#define fog_capacity 10
#define gate_way_capacity 10
#define agv_data_length 5000
#define edge_data_length 10000
#define fog_data_length 15000
#define edge_demand 3000
#define fog_demand 30000
#define gate_way_demand 300000
#define edge_delay 80
#define fog_delay 20
#define gate_delay 15
#define penalty_constant 100000

using namespace std;

int gate_way_number;
int fog_device_number;
int edge_device_number;
int agv_number;

struct penalty_cost {
    int link;
    int demand;
    int latency;
    int cover;
    int capacity;
};

vector<Gate_way> gate_ways;
vector<Fog_device> fog_devices;
vector<Edge_device> edge_devices;
vector<Agv> agvs;

Gate_way::Gate_way() {
    x = 0;
    y = 0;
    status = false;
    demand = 0;
}

Fog_device::Fog_device() {
    x = 0;
    y = 0;
    status = false;
    link = NULL;
    demand = 0;
    distance = 0;
}

Edge_device::Edge_device() {
    x = 0;
    y = 0;
    status = false;
    link = NULL;
    demand = 0;
    distance = 0;
}

Agv::Agv() {
    x = 0;
    y = 0;
    link = NULL;
    demand = (rand() % 151) + 50;
    distance  = 0;
}

void input(char *dataset) {
    std::ifstream infile(dataset);

    double x, y;
    int counter = 0;
    
    infile >> gate_way_number >> fog_device_number >> edge_device_number >> agv_number;

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

inline double distance(int x1, int y1, int x2, int y2) {
    return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}

void connect(vector<bool> device_status) {
    double shortest = DBL_MAX;
    int counter = 0;
    double temp;

    for (vector<bool>::iterator each_bit = device_status.begin(); each_bit != device_status.end(); ++each_bit) {
        if (counter < gate_way_number) {
            gate_ways[counter].status = *each_bit;
            counter++;
        }
        else if ((counter - gate_way_number) < fog_device_number) {
            fog_devices[counter - gate_way_number].status = *each_bit;
            counter++;
        }
        else {
            edge_devices[counter - gate_way_number - fog_device_number].status = *each_bit;
            counter++;
        }
    }

    for (vector<Agv>::iterator each_agv = agvs.begin(); each_agv != agvs.end(); ++each_agv) {
        (*each_agv).reset();
    }
    for (vector<Edge_device>::iterator each_edge_device = edge_devices.begin(); each_edge_device != edge_devices.end(); ++each_edge_device) {
        (*each_edge_device).reset();
    }
    for (vector<Fog_device>::iterator each_fog_device = fog_devices.begin(); each_fog_device != fog_devices.end(); ++each_fog_device) {
        (*each_fog_device).reset();
    }
    for (vector<Gate_way>::iterator each_gate_way = gate_ways.begin(); each_gate_way != gate_ways.end(); ++each_gate_way) {
        (*each_gate_way).reset();
    }

    for (vector<Agv>::iterator each_agv = agvs.begin(); each_agv != agvs.end(); ++each_agv) {
        shortest = DBL_MAX;
        for (vector<Edge_device>::iterator each_edge_device = edge_devices.begin(); each_edge_device != edge_devices.end(); ++each_edge_device) {
            if ((*each_edge_device).status) {
                if ((temp = distance((*each_agv).x, (*each_agv).y, (*each_edge_device).x, (*each_edge_device).y)) < shortest) {
                    shortest = temp;
                    if ((*each_agv).link != NULL) {
                        (*(*each_agv).link).link_quantity--;
                        (*(*each_agv).link).demand -= (*each_agv).demand;
                        (*each_agv).link = NULL;
                    }
                    (*each_agv).link = &(*each_edge_device);
                    (*each_agv).distance = shortest;
                    (*each_edge_device).link_quantity++;
                    (*each_edge_device).demand += (*each_agv).demand;
                }
            }
        }
    }

    for (vector<Edge_device>::iterator each_edge_device = edge_devices.begin(); each_edge_device != edge_devices.end(); ++each_edge_device) {
        shortest = DBL_MAX;
        if ((*each_edge_device).status) {
            for (vector<Fog_device>::iterator each_fog_device = fog_devices.begin(); each_fog_device != fog_devices.end(); ++each_fog_device) {
                if ((*each_fog_device).status) {
                    if ((temp = distance((*each_edge_device).x, (*each_edge_device).y, (*each_fog_device).x, (*each_fog_device).y)) < shortest) {
                        shortest = temp;
                        if ((*each_edge_device).link != NULL) {
                            (*(*each_edge_device).link).link_quantity--;
                            (*(*each_edge_device).link).demand -= (*each_edge_device).demand;
                            (*each_edge_device).link = NULL;
                        }
                        (*each_edge_device).link = &(*each_fog_device);
                        (*each_edge_device).distance = shortest;
                        (*each_fog_device).link_quantity++;
                        (*each_fog_device).demand += (*each_edge_device).demand;
                    }
                }
            }
        }
    }

    for (vector<Fog_device>::iterator each_fog_device = fog_devices.begin(); each_fog_device != fog_devices.end(); ++each_fog_device) {
        shortest = DBL_MAX;
        if ((*each_fog_device).status) {
            for (vector<Gate_way>::iterator each_gate_way = gate_ways.begin(); each_gate_way != gate_ways.end(); ++each_gate_way) {
                if ((*each_gate_way).status) {
                    if ((temp = distance((*each_fog_device).x, (*each_fog_device).y, (*each_gate_way).x, (*each_gate_way).y)) < shortest) {
                        shortest = temp;
                        if ((*each_fog_device).link != NULL) {
                            (*(*each_fog_device).link).link_quantity--;
                            (*(*each_fog_device).link).demand -= (*each_fog_device).demand;
                            (*each_fog_device).link = NULL;
                        }
                        (*each_fog_device).link = &(*each_gate_way);
                        (*each_fog_device).distance = shortest;
                        (*each_gate_way).link_quantity++;
                        (*each_gate_way).demand += (*each_fog_device).demand;
                    }
                }
            }
        }
    }
}

struct penalty_cost penalty_evaluation() {
    struct penalty_cost penalty;

    int covered_avgs = 0;
    int covered_edges = 0;
    int covered_fogs = 0;
    int user_request = 0;

    penalty.link = 0;
    penalty.demand = 0;
    penalty.latency = 0;
    penalty.cover = 0;
    penalty.capacity = 0;
 
    for (vector<Agv>::iterator each_agv = agvs.begin(); each_agv != agvs.end(); ++each_agv) {
        if ((*each_agv).link == NULL)
            penalty.link++;
        else {
            covered_avgs++;
            if (agv_data_length / agv_data_rate > edge_delay)
                penalty.latency++;
            if (distance((*each_agv).x, (*each_agv).y, (*each_agv).link->x, (*each_agv).link->y) > edge_range / 2)
                penalty.cover++;
        }
    }
    for (vector<Edge_device>::iterator each_edge_device = edge_devices.begin(); each_edge_device != edge_devices.end(); ++each_edge_device) {
        if ((*each_edge_device).status) {
            if ((*each_edge_device).demand > edge_demand) {
                penalty.demand++;
            }
            if ((*each_edge_device).link == NULL)
                penalty.link++;
            else {
                covered_edges++;
                if ((*each_edge_device).link_quantity > edge_capacity) {
                    penalty.capacity++;
                }
                if (edge_data_length / edge_data_rate > fog_delay)
                    penalty.latency++;
            }
        }
    }
    for (vector<Fog_device>::iterator each_fog_device = fog_devices.begin(); each_fog_device != fog_devices.end(); ++each_fog_device) {
        if ((*each_fog_device).status) {
            if ((*each_fog_device).demand > fog_demand) {
                penalty.demand++;
            }
            if ((*each_fog_device).link == NULL)
                penalty.link++;
            else {
                covered_fogs++;
                if ((*each_fog_device).link_quantity > fog_capacity) {
                    penalty.capacity++;
                }
                if (fog_data_length / fog_data_rate > gate_delay)
                    penalty.latency++;
            }
        }
    }
    for (vector<Gate_way>::iterator each_gate_way = gate_ways.begin(); each_gate_way != gate_ways.end(); ++each_gate_way) {
        if ((*each_gate_way).status) {
            if ((*each_gate_way).link_quantity > gate_way_capacity) {
                penalty.capacity++;
            }
        }
    }

    return penalty;
}

double cost_evaluation(vector<bool> solution) {
    struct penalty_cost extra_cost;
    double cost;
    // int counter;
    int fiber_length = 0;
    int edge_quantity = 0;
    int fog_quantity = 0;
    int gate_way_quantity = 0;

    connect(solution);
    extra_cost = penalty_evaluation();

    for (vector<Agv>::iterator each_agv = agvs.begin(); each_agv != agvs.end(); ++each_agv) {
        if ((*each_agv).link != NULL)
            fiber_length += (*each_agv).distance;
    }
    for (vector<Edge_device>::iterator each_edge_device = edge_devices.begin(); each_edge_device != edge_devices.end(); ++each_edge_device) {
        if ((*each_edge_device).link != NULL)
            fiber_length += (*each_edge_device).distance;
        if ((*each_edge_device).status)
            edge_quantity++;
    }
    for (vector<Fog_device>::iterator each_fog_device = fog_devices.begin(); each_fog_device != fog_devices.end(); ++each_fog_device) {
        if ((*each_fog_device).link != NULL)
            fiber_length += (*each_fog_device).distance;
        if ((*each_fog_device).status)
            fog_quantity++;
    }
    for (vector<Gate_way>::iterator each_gate_way = gate_ways.begin(); each_gate_way != gate_ways.end(); ++each_gate_way) {
        if ((*each_gate_way).status)
            gate_way_quantity++;
    }

    cost = fiber_cost * fiber_length + gate_way_cost * gate_way_quantity + fog_cost * fog_quantity + edge_cost * edge_quantity\
            + penalty_constant * (extra_cost.link + extra_cost.demand + extra_cost.latency + extra_cost.cover + extra_cost.capacity);

    return cost;
}

void find_loading(vector<bool> solution) {
    connect(solution);

}