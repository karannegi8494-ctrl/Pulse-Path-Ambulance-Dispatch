#include <iostream>
#include "hospital.h"

using namespace std;

void HospitalSelection::addHospital(
    int id, string name, string location,
    bool available, int totalBeds, int occupiedBeds) {

    if (id <= 0 || name.empty() || location.empty() ||
        totalBeds < 0 || occupiedBeds < 0 ||
        occupiedBeds > totalBeds) {
        cout << "Invalid hospital details!\n";
        return;
    }

    for (const auto& h : hospitals) {
        if (h.id == id) {
            cout << "Hospital ID already exists!\n";
            return;
        }
    }

    hospitals.push_back({
        id, name, location, available, totalBeds, occupiedBeds
    });

    cout << "Hospital added successfully!\n";
}

void HospitalSelection::showHospitals() const {
    cout << "\n===== HOSPITAL LIST =====\n";

    if (hospitals.empty()) {
        cout << "No hospitals registered.\n";
        return;
    }

    for (const auto& h : hospitals) {
        cout << "ID: " << h.id
             << " | Name: " << h.name
             << " | Location: " << h.location
             << " | Status: "
             << (h.available ? "Available" : "Unavailable")
             << " | Beds available: "
             << h.totalBeds - h.occupiedBeds
             << "/" << h.totalBeds << '\n';
    }
}

void HospitalSelection::selectHospital(
    const Graph& city, const string& patientLocation) const {

    if (!city.hasLocation(patientLocation)) {
        cout << "Unknown patient location!\n";
        return;
    }

    const Hospital* selected = nullptr;
    PathResult bestRoute;

    for (const auto& h : hospitals) {
        if (!h.available || h.occupiedBeds >= h.totalBeds)
            continue;

        if (!city.hasLocation(h.location))
            continue;

        PathResult route =
            city.shortestPath(patientLocation, h.location);

        if (!route.reachable)
            continue;

        if (selected == nullptr ||
            route.distance < bestRoute.distance) {
            selected = &h;
            bestRoute = route;
        }
    }

    cout << "\n===== HOSPITAL SELECTION RESULT =====\n";

    if (selected == nullptr) {
        cout << "No suitable hospital with available beds found!\n";
        return;
    }

    cout << "Patient location: " << patientLocation << '\n';
    cout << "Selected hospital: " << selected->name << '\n';
    cout << "Hospital ID: " << selected->id << '\n';
    cout << "Hospital location: " << selected->location << '\n';
    cout << "Available beds: "
         << selected->totalBeds - selected->occupiedBeds << '\n';
    cout << "Estimated travel time: "
         << bestRoute.distance << " minutes\n";

    cout << "Route: ";
    for (size_t i = 0; i < bestRoute.path.size(); ++i) {
        cout << bestRoute.path[i];

        if (i + 1 < bestRoute.path.size())
            cout << " -> ";
    }

    cout << '\n';
}