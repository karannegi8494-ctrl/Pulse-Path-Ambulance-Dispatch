#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <string>
#include <vector>
#include "graph.h"

using namespace std;

struct Hospital {
    int id;
    string name;
    string location;
    bool available;
    int totalBeds;
    int occupiedBeds;
};

class HospitalSelection {
private:
    vector<Hospital> hospitals;

public:
    void addHospital(int id, string name, string location,
                     bool available, int totalBeds, int occupiedBeds);

    void showHospitals() const;

    void selectHospital(const Graph& city,
                        const string& patientLocation) const;
};

#endif