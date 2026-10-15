#ifndef AMBULANCE_H
#define AMBULANCE_H

#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct Ambulance {
    int id;
    string name;
    bool available;
};

class AmbulanceManager {
private:
    vector<Ambulance> ambulances;

    int getID()const;
    string getLocation()const;
    bool isAvailale()const;

    void setLocation(string loaction);
    void setAvailability(bool status);
public:
    void addAmbulance(int id, string name, bool available);
    void showAmbulances();
    void updateStatus(int id, bool status);
    void dispatchAmbulance();
};

#endif