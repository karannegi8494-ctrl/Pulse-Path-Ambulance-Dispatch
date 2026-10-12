#ifndef EMERGENCY_DISPATCH_H
#define EMERGENCY_DISPATCH_H

#include <string>
#include <vector>

// This module deliberately depends on interfaces instead of owning ambulance
// records or routing. AmbulanceProvider is implemented by the ambulance
// management module, and RoutingService is implemented by the graph module.
struct AmbulanceSnapshot {
    int id;
    std::string currentLocation;
    bool available;
};

class AmbulanceProvider {
public:
    virtual ~AmbulanceProvider() {}
    virtual std::vector<AmbulanceSnapshot> getAmbulances() const = 0;
};

struct RouteEstimate {
    bool reachable;
    int travelTime;
    std::vector<std::string> route;
};

class RoutingService {
public:
    virtual ~RoutingService() {}
    virtual bool isValidLocation(const std::string& location) const = 0;
    virtual RouteEstimate findShortestRoute(const std::string& start,
                                            const std::string& destination) const = 0;
};

enum class DispatchStatus {
    Success,
    InvalidEmergencyLocation,
    NoAvailableAmbulances,
    NoReachableAmbulances
};

struct DispatchResult {
    DispatchStatus status;
    int ambulanceId;
    int estimatedTravelTime;
    std::vector<std::string> route;

    bool hasDispatch() const;
};

class EmergencyDispatch {
public:
    EmergencyDispatch(const AmbulanceProvider& ambulanceProvider,
                      const RoutingService& routingService);

    // Selects the available ambulance with the lowest travel time. Equal
    // travel times are resolved by choosing the numerically lowest ID.
    DispatchResult planDispatch(const std::string& emergencyLocation) const;

private:
    const AmbulanceProvider& ambulanceProvider_;
    const RoutingService& routingService_;
};

#endif
