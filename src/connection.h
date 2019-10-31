#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <time.h>

using namespace std;

extern int gateway_number;
extern int fog_server_number;
extern int edge_server_number;

void connect();

// class Server {
//     public:
//         Server();

//         double longitude;
//         double latitude;
// };

// class Client {
//     public:
//         Client();

//         double longitude;
//         double latitude;
//         vector<int> priority;
// };

// vector<Server> servers;
// vector<Client> clients;
// vector<vector<long double> > distance_table;

#endif