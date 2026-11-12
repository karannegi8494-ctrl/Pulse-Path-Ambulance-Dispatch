#include "Ambulance.h"

#include <iostream>

void AmbulanceManager::addAmbulance(
    int id, const std::string& name, const std::string& currentLocation, bool available) {
    if (id <= 0 || name.empty() || currentLocation.empty()) {
        std::cout << "Invalid ambulance details.\n";
        return;
    }

    for (const Ambulance& ambulance : ambulances) {
        if (ambulance.id == id) {
            std::cout << "Ambulance ID already exists.\n";
            return;
        }
    }

    ambulances.push_back({id, name, currentLocation, available});

    std::cout << "Ambulance added successfully!\n";
}

void AmbulanceManager::showAmbulances() const {
    std::cout << "\n===== AMBULANCE LIST =====\n";

    if (ambulances.empty()) {
        std::cout << "No ambulances registered.\n";
        return;
    }

    for (const auto& a : ambulances) {
        std::cout << "ID: " << a.id
                  << " | Name: " << a.name
                  << " | Location: " << a.currentLocation
                  << " | Status: " << (a.available ? "Available" : "Busy") << '\n';
    }
}

void AmbulanceManager::updateStatus(int id, bool status) {
    for (auto& a : ambulances) {
        if (a.id == id) {
            a.available = status;
            std::cout << "Ambulance status updated!\n";
            return;
        }
    }

    std::cout << "Ambulance not found!\n";
}

bool AmbulanceManager::dispatchAmbulance(int id) {
    for (auto& a : ambulances) {
        if (a.id == id && a.available) {
            a.available = false;
            return true;
        }
    }

    return false;
}

const std::vector<Ambulance>& AmbulanceManager::getAmbulances() const {
    return ambulances;
}
