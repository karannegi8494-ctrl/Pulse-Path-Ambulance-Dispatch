#ifndef AMbULANCE_H
#define AMbULANCE_H
#include<string>
using namespace std;
class Ambulance{
    private:
    int ambulanceID;
    string currentLocation;
    bool available;

    public:
    Ambulance(int id,string location);

    int getID()const;
    string getLocation()const;
    bool isAvailale()const;

    void setLocation(string loaction);
    coid setAvailability(bool status);
};

#endif