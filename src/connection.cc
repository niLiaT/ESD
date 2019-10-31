#include "connection.h"

using namespace std;

int gateway_number;
int fog_server_number;
int edge_server_number;

void connect() {
    gateway_number = 40;
    fog_server_number = 40;
    edge_server_number = 40;
}

// void input() {
//     std::ifstream infile("dataset/taichung.txt");

//     double longitude, latitude;
//     int counter = 0;

//     while(infile >> longitude >> latitude) {
//         servers[counter].longitude = longitude;
//         servers[counter].latitude = latitude;
//         counter++;
//     }

//     counter = 0;

//     infile.close();
//     infile.clear();

//     infile.open("dataset/population_taichung.txt");
//     while(infile >> longitude >> latitude) {
//         clients[counter].longitude = longitude;
//         clients[counter].latitude = latitude;
//         counter++;
//     }
// }

// Server::Server() {
//     longitude = 0.0;
//     latitude = 0.0;
// }

// Client::Client() {
//     longitude = 0.0;
//     latitude = 0.0;
// }

// void initialize(int server_quantity, int client_quantity) {
//     servers.resize(server_quantity);
//     clients.resize(client_quantity);

//     input();

//     distance_table.resize(server_quantity, vector<long double>(client_quantity));

//     for (int each_server = 0; each_server < server_quantity; ++each_server) {
//         for (int each_client = 0; each_client < client_quantity; ++each_client) {
//             distance_table[each_server][each_client] = \
//             distance(servers[each_server].latitude, servers[each_server].longitude, clients[each_client].latitude, clients[each_client].longitude);
//         }
//     }

// }

// void connection(vector<bool> solution) {
     

// }

// int main() {
//     initialize(320, 1500);

//     srand(time(NULL));

//     vector<bool> sol(320, 0);

//     for (int i = 0; i < 320; i++) {
//         sol[i] = rand() % 2;
//     }

//     connection(sol);

// }