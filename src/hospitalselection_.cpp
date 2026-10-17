
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <climits>
#include <functional>
using namespace std;

class Graph {
private:
    int V;
    vector<vector<pair<int, int>>> adj;

public:
    Graph(int vertices) {
        V = vertices;
        adj.resize(V);
    }

    void addRoad(int u, int v, int time) {
        adj[u].push_back({v, time});
        adj[v].push_back({u, time});
    }

    vector<int> dijkstra(int source) {
        vector<int> dist(V, INT_MAX);

        priority_queue<
            pair<int, int>,
            vector<pair<int, int>>,
            greater<pair<int, int>>
        > pq;

        dist[source] = 0;
        pq.push({0, source});

        while (!pq.empty()) {
            int time = pq.top().first;
            int node = pq.top().second;
            pq.pop();

            if (time > dist[node])
                continue;

            for (auto road : adj[node]) {
                int nextNode = road.first;
                int roadTime = road.second;

                if (dist[node] != INT_MAX &&
                    dist[node] + roadTime < dist[nextNode]) {
                    dist[nextNode] = dist[node] + roadTime;
                    pq.push({dist[nextNode], nextNode});
                }
            }
        }

        return dist;
    }
};

struct Hospital {
    int id;
    string name;
    int location;
    bool available;
};

class HospitalSelection {
private:
    vector<Hospital> hospitals;

public:
    void addHospital(int id, string name, int location,
                     bool available) {
        hospitals.push_back({id, name, location, available});
    }

    int selectHospital(Graph &g, int patientLocation) {
        vector<int> dist = g.dijkstra(patientLocation);

        int selected = -1;
        int minTime = INT_MAX;

        for (int i = 0; i < hospitals.size(); i++) {
            int loc = hospitals[i].location;

            if (hospitals[i].available &&
                loc >= 0 && loc < dist.size() &&
                dist[loc] < minTime) {
                minTime = dist[loc];
                selected = i;
            }
        }

        cout << "\n--- Hospital Selection Result ---\n";

        if (selected == -1) {
            cout << "No suitable hospital available.\n";
            return -1;
        }

        cout << "Patient Location: "
             << patientLocation << endl;

        cout << "Selected Hospital: "
             << hospitals[selected].name << endl;

        cout << "Hospital ID: "
             << hospitals[selected].id << endl;

        cout << "Hospital Location: "
             << hospitals[selected].location << endl;

        cout << "Estimated Travel Time: "
             << minTime << " minutes\n";

        return hospitals[selected].id;
    }
};

int main() {
    // Demo graph: replace with your team's Graph module.
    Graph g(6);

    g.addRoad(0, 1, 4);
    g.addRoad(0, 2, 2);
    g.addRoad(1, 2, 1);
    g.addRoad(1, 3, 5);
    g.addRoad(2, 3, 8);
    g.addRoad(2, 4, 10);
    g.addRoad(3, 4, 2);
    g.addRoad(3, 5, 6);
    g.addRoad(4, 5, 3);

    HospitalSelection hs;

    hs.addHospital(101, "City Hospital", 3, true);
    hs.addHospital(102, "AIIMS Hospital", 5, true);
    hs.addHospital(103, "Apollo Hospital", 4, false);

    int patientLocation;

    cout << "Enter patient location ID (0-5): ";
    cin >> patientLocation;

    if (patientLocation < 0 || patientLocation >= 6) {
        cout << "Invalid patient location!\n";
        return 0;
    }

    // Emergency Dispatch can call this function.
    int hospitalID = hs.selectHospital(g, patientLocation);

    if (hospitalID != -1) {
        cout << "\nHospital ID sent to Emergency Dispatch: "
             << hospitalID << endl;
    }

    return 0;
}