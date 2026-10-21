#include <iostream>
#include "Ambulance.h"
#include "graph.h"
#include "EmergencyDispatch.h"
#include <vector>
#include <limits>
#include "Hospital.h"
using namespace std;

int main() {
    AmbulanceManager manager;
    Graph city;
    AmbulanceManager manager;
    HospitalSelection hospitalManager;
    const char *places[] = {"Central_Hospital", "North_Station", "Market",
                            "City_Park", "Airport", "University", "Old_Town"};
    vector<string> placeNames; // same order as graph ids (used to label distances)
    for (const char *p : places)
    {
        city.addLocation(p);
        placeNames.push_back(p);
    }
    // Sample city map
    city.addRoad("Central_Hospital", "Market", 4);
    city.addRoad("Central_Hospital", "North_Station", 6);
    city.addRoad("Market", "City_Park", 3);
    city.addRoad("Market", "Old_Town", 5);
    city.addRoad("North_Station", "University", 4);
    city.addRoad("City_Park", "University", 6);
    city.addRoad("City_Park", "Airport", 9);
    city.addRoad("Old_Town", "Airport", 7);


    // Sample ambulance data
    manager.addAmbulance(101, "Ambulance_A", true);
    manager.addAmbulance(102, "Ambulance_B", true);
    manager.addAmbulance(103, "Ambulance_C", false);

    int choice;

    do {
        cout << "\n===== PULSEPATH MENU =====\n";
        cout << "1. View Ambulances\n";
        cout << "2. Add Ambulance\n";
        cout << "3. Dispatch Ambulance\n";
        cout << "4. Update Ambulance Status\n";
        cout << "5. View City Map\n";
        cout << "6. Show Distances From a Location\n";
        cout << "7. Add Location to Map\n";
        cout << "8. Add Road to Map\n";
        cout << "9. Add Hospital\n";
        cout << "10. View Hospitals\n";
        cout << "11. Select Best Hospital\n";
        cout << "0. Exit\n";

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                manager.showAmbulances();
                break;

            case 2: {
                int id;
                string name;
                int status;

                cout << "Enter ambulance ID: ";
                cin >> id;

                cout << "Enter ambulance name: ";
                cin >> name;

                cout << "Available? (1 = Yes, 0 = No): ";
                cin >> status;

                if (status != 0 && status != 1) {
                    cout << "Invalid status!\n";
                    break;
                }

                manager.addAmbulance(id, name, status);
                break;
            }

            case 3:
                manager.dispatchAmbulance();
                break;

            case 4: {
                int id, status;

                cout << "Enter ambulance ID: ";
                cin >> id;

                cout << "Enter new status (1 = Available, 0 = Busy): ";
                cin >> status;

                if (status != 0 && status != 1) {
                    cout << "Invalid status!\n";
                    break;
                }

                manager.updateStatus(id, status);
                break;
            }
            
        case 5:
        {
            cout << "\n--- City Map ---\n";
            city.printGraph();
            break;

        case 6:
        {
            string from;
                cout << "Enter starting location: ";
                cin >> from;
 
                if (!city.hasLocation(from)) {
                    cout << "Unknown location!\n";
                    break;
                }
 
                vector<double> dist = city.distancesFrom(from);
                cout << "Travel time from " << from << ":\n";
                for (int i = 0; i < (int)dist.size(); ++i) {
                    cout << "  " << placeNames[i] << ": ";
                    if (dist[i] == numeric_limits<double>::infinity()) cout << "unreachable\n";
                    else cout << dist[i] << "\n";
                }
                break;
            }
 

        case 7:
        {
            string name;
            cout << "Enter new location name (no spaces): ";
            cin >> name;

            if (city.hasLocation(name))
            {
                cout << "Location already exists!\n";
                break;
            }
            city.addLocation(name);
            placeNames.push_back(name);
            cout << "Location added.\n";
            break;
        }

        case 8:
        {
            string from, to;
            double minutes;
            int twoWay;

            cout << "Enter first location: ";
            cin >> from;
            cout << "Enter second location: ";
            cin >> to;
            cout << "Enter travel time (minutes): ";
            cin >> minutes;
            cout << "Two-way road? (1 = Yes, 0 = No): ";
            cin >> twoWay;

            if (twoWay != 0 && twoWay != 1)
            {
                cout << "Invalid choice!\n";
                break;
            }

            if (city.addRoad(from, to, minutes, twoWay == 1))
            {
                cout << "Road added.\n";
            }
            else
            {
                cout << "Could not add road (check locations and time).\n";
            }
            break;
        }
        
case 9: {
    int id, totalBeds, occupiedBeds;
    string name, location;
    int status;

    cout << "Enter Hospital ID: ";
    cin >> id;

    cout << "Enter Hospital Name (no spaces): ";
    cin >> name;

    cout << "Enter Hospital Location (must exist on city map): ";
    cin >> location;

    if (!city.hasLocation(location)) {
        cout << "Location does not exist on city map!\n";
        break;
    }

    cout << "Hospital Available? (1 = Yes, 0 = No): ";
    cin >> status;

    if (status != 0 && status != 1) {
        cout << "Invalid status!\n";
        break;
    }

    cout << "Enter Total Beds: ";
    cin >> totalBeds;

    cout << "Enter Occupied Beds: ";
    cin >> occupiedBeds;

    hospitalManager.addHospital(
        id, name, location, status == 1,
        totalBeds, occupiedBeds
    );
    break;
}

case 10:
    hospitalManager.showHospitals();
    break;

case 11: {
    string patientLocation;

    cout << "Enter Patient Location: ";
    cin >> patientLocation;

    hospitalManager.selectHospital(city, patientLocation);
    break;
}


            case 0:
                cout << "Exiting PulsePath...\n";
                break;

            default:
                cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 0);

    return 0;
}
