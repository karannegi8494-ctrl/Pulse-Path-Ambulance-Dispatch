#include "Ambulance.h"

Ambulance::Ambulance(int id, string location) {
    ambulanceID = id;
    currentLocation = location;
    available = true;
}

int Ambulance::getID() const {
    return ambulanceID;
}

string Ambulance::getLocation() const {
    return currentLocation;
}

bool Ambulance::isAvailable() const {
    return available;
}

void Ambulance::setLocation(string location) {
    currentLocation = location;
}

void Ambulance::setAvailability(bool status) {
    available = status;
}