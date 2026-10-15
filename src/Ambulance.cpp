#include "Ambulance.h"

void AmbulanceManager::addAmbulance(
    int id, string name, bool available) {

    ambulances.push_back({id, name, available});

    cout << "Ambulance added successfully!\n";
}

void AmbulanceManager::showAmbulances() {
    cout << "\n===== AMBULANCE LIST =====\n";

    if (ambulances.empty()) {
        cout << "No ambulances registered.\n";
        return;
    }

    for (const auto& a : ambulances) {
        cout << "ID: " << a.id
             << " | Name: " << a.name
             << " | Status: ";

        if (a.available)
            cout << "Available";
        else
            cout << "Busy";

        cout << endl;
    }
}

void AmbulanceManager::updateStatus(int id, bool status) {
    for (auto& a : ambulances) {
        if (a.id == id) {
            a.available = status;
            cout << "Ambulance status updated!\n";
            return;
        }
    }

    cout << "Ambulance not found!\n";
}

void AmbulanceManager::dispatchAmbulance() {
    for (auto& a : ambulances) {
        if (a.available) {
            cout << "\nEmergency dispatched!\n";
            cout << "Ambulance: " << a.name << endl;

            a.available = false;

            cout << "Status: Busy\n";
            return;
        }
    }

    cout << "No ambulance available!\n";
}