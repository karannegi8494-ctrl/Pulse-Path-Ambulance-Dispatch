#include "EmergencyDispatch.h"

#include <cstdlib>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {

class FakeAmbulanceProvider : public AmbulanceProvider {
public:
    explicit FakeAmbulanceProvider(const std::vector<AmbulanceSnapshot>& ambulances)
        : ambulances_(ambulances) {}

    std::vector<AmbulanceSnapshot> getAmbulances() const {
        return ambulances_;
    }

private:
    std::vector<AmbulanceSnapshot> ambulances_;
};

class FakeRoutingService : public RoutingService {
public:
    void addLocation(const std::string& location) {
        locations_.push_back(location);
    }

    void addRoute(const std::string& start, const std::string& destination,
                  bool reachable, int travelTime,
                  const std::vector<std::string>& route) {
        routes_[key(start, destination)] = RouteEstimate{reachable, travelTime, route};
    }

    bool isValidLocation(const std::string& location) const {
        for (std::vector<std::string>::const_iterator it = locations_.begin();
             it != locations_.end(); ++it) {
            if (*it == location) {
                return true;
            }
        }
        return false;
    }

    RouteEstimate findShortestRoute(const std::string& start,
                                    const std::string& destination) const {
        const std::map<std::string, RouteEstimate>::const_iterator it =
            routes_.find(key(start, destination));
        if (it == routes_.end()) {
            return RouteEstimate{false, -1, std::vector<std::string>()};
        }
        return it->second;
    }

private:
    static std::string key(const std::string& start, const std::string& destination) {
        return start + "->" + destination;
    }

    std::vector<std::string> locations_;
    std::map<std::string, RouteEstimate> routes_;
};

void expect(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

void testFiltersBusyAmbulances() {
    FakeRoutingService routing;
    routing.addLocation("A"); routing.addLocation("B"); routing.addLocation("E");
    routing.addRoute("A", "E", true, 2, std::vector<std::string>{"A", "E"});
    routing.addRoute("B", "E", true, 8, std::vector<std::string>{"B", "E"});
    FakeAmbulanceProvider provider(std::vector<AmbulanceSnapshot>{
        AmbulanceSnapshot{20, "A", false}, AmbulanceSnapshot{21, "B", true}});

    EmergencyDispatch dispatch(provider, routing);
    const DispatchResult result = dispatch.planDispatch("E");
    expect(result.hasDispatch() && result.ambulanceId == 21 && result.estimatedTravelTime == 8,
           "busy ambulances must not be selected");
}

void testSelectsMinimumTravelTimeAndRoute() {
    FakeRoutingService routing;
    routing.addLocation("A"); routing.addLocation("B"); routing.addLocation("E");
    routing.addRoute("A", "E", true, 10, std::vector<std::string>{"A", "X", "E"});
    routing.addRoute("B", "E", true, 4, std::vector<std::string>{"B", "E"});
    FakeAmbulanceProvider provider(std::vector<AmbulanceSnapshot>{
        AmbulanceSnapshot{2, "A", true}, AmbulanceSnapshot{9, "B", true}});

    EmergencyDispatch dispatch(provider, routing);
    const DispatchResult result = dispatch.planDispatch("E");
    expect(result.hasDispatch() && result.ambulanceId == 9 && result.estimatedTravelTime == 4,
           "smallest route time must win");
    expect(result.route.size() == 2 && result.route[0] == "B" && result.route[1] == "E",
           "selected route must be returned");
}

void testSkipsUnreachableAmbulances() {
    FakeRoutingService routing;
    routing.addLocation("A"); routing.addLocation("B"); routing.addLocation("E");
    routing.addRoute("A", "E", false, -1, std::vector<std::string>());
    routing.addRoute("B", "E", true, 6, std::vector<std::string>{"B", "E"});
    FakeAmbulanceProvider provider(std::vector<AmbulanceSnapshot>{
        AmbulanceSnapshot{1, "A", true}, AmbulanceSnapshot{2, "B", true}});

    EmergencyDispatch dispatch(provider, routing);
    const DispatchResult result = dispatch.planDispatch("E");
    expect(result.hasDispatch() && result.ambulanceId == 2,
           "unreachable ambulances must be skipped");
}

void testRejectsInvalidEmergencyLocation() {
    FakeRoutingService routing;
    routing.addLocation("A");
    FakeAmbulanceProvider provider(std::vector<AmbulanceSnapshot>{AmbulanceSnapshot{1, "A", true}});

    EmergencyDispatch dispatch(provider, routing);
    const DispatchResult result = dispatch.planDispatch("missing");
    expect(result.status == DispatchStatus::InvalidEmergencyLocation && !result.hasDispatch(),
           "invalid emergency locations must fail explicitly");
}

void testSkipsAmbulanceAtInvalidLocation() {
    FakeRoutingService routing;
    routing.addLocation("E");
    FakeAmbulanceProvider provider(
        std::vector<AmbulanceSnapshot>{AmbulanceSnapshot{1, "unknown", true}});

    EmergencyDispatch dispatch(provider, routing);
    const DispatchResult result = dispatch.planDispatch("E");
    expect(result.status == DispatchStatus::NoReachableAmbulances,
           "ambulances with invalid locations must not be dispatched");
}

void testReportsNoAvailableAmbulance() {
    FakeRoutingService routing;
    routing.addLocation("A"); routing.addLocation("E");
    FakeAmbulanceProvider provider(std::vector<AmbulanceSnapshot>{AmbulanceSnapshot{1, "A", false}});

    EmergencyDispatch dispatch(provider, routing);
    const DispatchResult result = dispatch.planDispatch("E");
    expect(result.status == DispatchStatus::NoAvailableAmbulances,
           "all-busy fleet must report no availability");
}

void testUsesLowestIdForEqualTravelTimes() {
    FakeRoutingService routing;
    routing.addLocation("A"); routing.addLocation("B"); routing.addLocation("E");
    routing.addRoute("A", "E", true, 5, std::vector<std::string>{"A", "E"});
    routing.addRoute("B", "E", true, 5, std::vector<std::string>{"B", "E"});
    FakeAmbulanceProvider provider(std::vector<AmbulanceSnapshot>{
        AmbulanceSnapshot{14, "A", true}, AmbulanceSnapshot{3, "B", true}});

    EmergencyDispatch dispatch(provider, routing);
    const DispatchResult result = dispatch.planDispatch("E");
    expect(result.hasDispatch() && result.ambulanceId == 3,
           "equal travel times must use the lowest ambulance ID");
}

void testReportsNoReachableAmbulance() {
    FakeRoutingService routing;
    routing.addLocation("A"); routing.addLocation("E");
    routing.addRoute("A", "E", false, -1, std::vector<std::string>());
    FakeAmbulanceProvider provider(std::vector<AmbulanceSnapshot>{AmbulanceSnapshot{1, "A", true}});

    EmergencyDispatch dispatch(provider, routing);
    const DispatchResult result = dispatch.planDispatch("E");
    expect(result.status == DispatchStatus::NoReachableAmbulances,
           "reachable failure must be distinguished from no availability");
}

}

int main() {
    try {
        testFiltersBusyAmbulances();
        testSelectsMinimumTravelTimeAndRoute();
        testSkipsUnreachableAmbulances();
        testRejectsInvalidEmergencyLocation();
        testSkipsAmbulanceAtInvalidLocation();
        testReportsNoAvailableAmbulance();
        testUsesLowestIdForEqualTravelTimes();
        testReportsNoReachableAmbulance();
        std::cout << "All EmergencyDispatch tests passed.\n";
        return EXIT_SUCCESS;
    } catch (const std::exception& error) {
        std::cerr << "EmergencyDispatch test failed: " << error.what() << '\n';
        return EXIT_FAILURE;
    }
}
