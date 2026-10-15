#include <iostream>
#include "Ambulance.h"
using namespace std;

int main() {
    AmbulanceManager manager;

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

            case 0:
                cout << "Exiting PulsePath...\n";
                break;

            default:
                cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 0);

    return 0;
}