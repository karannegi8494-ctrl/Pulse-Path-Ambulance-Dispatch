#ifndef AMBULANCE_H
#define AMBULANCE_H

#include <string>
#include <vector>

struct Ambulance {
    int id;
    std::string name;
    std::string currentLocation;
    bool available;
};

class AmbulanceManager {
private:
    std::vector<Ambulance> ambulances;
public:
    void addAmbulance(int id, const std::string& name,
                      const std::string& currentLocation, bool available);
    void showAmbulances() const;
    void updateStatus(int id, bool status);
    bool dispatchAmbulance(int id);
    const std::vector<Ambulance>& getAmbulances() const;
};

#endif
