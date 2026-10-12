#include <iostream>
using namespace std;

int main() {
    int n, patientLocation;

    cout << "Enter patient location ID: ";
    cin >> patientLocation;

    cout << "Enter number of hospitals: ";
    cin >> n;

    string name[50];
    int location[50], time[50], available[50];

    for (int i = 0; i < n; i++) {
        cout << "\nEnter hospital name: ";
        cin >> name[i];

        cout << "Enter hospital location ID: ";
        cin >> location[i];

        cout << "Enter travel time (minutes): ";
        cin >> time[i];

        cout << "Is hospital available? (1=Yes, 0=No): ";
        cin >> available[i];
    }

    int minTime = 9999;
    int selected = -1;

    for (int i = 0; i < n; i++) {
        if (available[i] == 1 && time[i] >= 0 &&
            time[i] < minTime) {
            minTime = time[i];
            selected = i;
        }
    }

    cout << "\n--- Hospital Selection Result ---\n";

    if (selected == -1) {
        cout << "No suitable hospital available.\n";
    } else {
        cout << "Patient Location: "
             << patientLocation << endl;

        cout << "Selected Hospital: "
             << name[selected] << endl;

        cout << "Hospital Location ID: "
             << location[selected] << endl;

        cout << "Estimated Travel Time: "
             << minTime << " minutes\n";
    }

    return 0;
}