#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include "Ambulance.h"
#include "graph.h"
using namespace std;

static const double INF = std::numeric_limits<double>::infinity();

int Graph::addLocation(const string& name) {
    auto it = idOf.find(name);
    if (it != idOf.end()) return it->second;   // already exists

    int id = static_cast<int>(names.size());
    names.push_back(name);
    adj.emplace_back();
    idOf[name] = id;
    return id;
}

bool Graph::hasLocation(const string& name) const {
    return idOf.find(name) != idOf.end();
}

int Graph::size() const {
    return static_cast<int>(names.size());
}

bool Graph::addRoad(const string& from, const string& to,
                    double weight, bool bidirectional) {
    if (weight < 0) return false;                       // Dijkstra needs non-negative weights
    if (!hasLocation(from) || !hasLocation(to)) return false;

    int u = idOf.at(from);
    int v = idOf.at(to);
    adj[u].push_back({v, weight});
    if (bidirectional) adj[v].push_back({u, weight});
    return true;
}

void Graph::dijkstra(int src, vector<double>& dist, vector<int>& parent) const {
    int n = size();
    dist.assign(n, INF);
    parent.assign(n, -1);
    vector<bool> done(n, false);
    dist[src] = 0;
 
    for (int i = 0; i < n; ++i) {
        int u = -1;
        for (int j = 0; j < n; ++j)
            if (!done[j] && (u == -1 || dist[j] < dist[u])) u = j;
        if (u == -1 || dist[u] == INF) break;
        done[u] = true;
 
        for (const Edge& e : adj[u]) {
            if (dist[u] + e.weight < dist[e.to]) {
                dist[e.to] = dist[u] + e.weight;
                parent[e.to] = u;
            }
        }
    }
}
PathResult Graph::shortestPath(const string& from, const string& to) const {
    PathResult result;
    if (!hasLocation(from) || !hasLocation(to)) return result;

    int src = idOf.at(from);
    int dst = idOf.at(to);

    vector<double> dist;
    vector<int> parent;
    dijkstra(src, dist, parent);

    if (dist[dst] == INF) return result;                // unreachable

    result.reachable = true;
    result.distance = dist[dst];
    for (int v = dst; v != -1; v = parent[v]) {
        result.path.push_back(names[v]);
    }
    reverse(result.path.begin(), result.path.end());
    return result;
}

vector<double> Graph::distancesFrom(const string& from) const {
    if (!hasLocation(from)) return {};

    vector<double> dist;
    vector<int> parent;
    dijkstra(idOf.at(from), dist, parent);
    return dist;                                        // INF means unreachable
}

void Graph::printGraph() const {
    for (int u = 0; u < size(); ++u) {
        cout << names[u] << " -> ";
        if (adj[u].empty()) cout << "(no roads)";
        for (size_t i = 0; i < adj[u].size(); ++i) {
            cout << names[adj[u][i].to] << "(" << adj[u][i].weight << ")";
            if (i + 1 < adj[u].size()) cout << ", ";
        }
        cout << "\n";
    }
}