#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

struct Edge {
    int to;        
    double weight; 
};

struct PathResult {
    bool reachable = false;
    double distance = 0.0;
    vector<string> path; 
};

class Graph {
private:
    vector<vector<Edge>> adj;            
    vector<string> names;                
    unordered_map<string, int> idOf;     

    void dijkstra(int src, vector<double>& dist, vector<int>& parent) const;

public:
    int addLocation(const string& name);
    bool hasLocation(const string& name) const;
    int size() const;

    bool addRoad(const string& from, const string& to,
                 double weight, bool bidirectional = true);

    PathResult shortestPath(const string& from, const string& to) const;
    vector<double> distancesFrom(const string& from) const;

    void printGraph() const;
};

#endif