#include "EmergencyDispatch.h"

namespace {

DispatchResult failure(DispatchStatus status) {
    DispatchResult result;
    result.status = status;
    result.ambulanceId = -1;
    result.estimatedTravelTime = -1;
    return result;
}

}

bool DispatchResult::hasDispatch() const {
    return status == DispatchStatus::Success;
}

EmergencyDispatch::EmergencyDispatch(const AmbulanceProvider& ambulanceProvider,
                                     const RoutingService& routingService)
    : ambulanceProvider_(ambulanceProvider), routingService_(routingService) {}

DispatchResult EmergencyDispatch::planDispatch(const std::string& emergencyLocation) const {
    if (emergencyLocation.empty() || !routingService_.isValidLocation(emergencyLocation)) {
        return failure(DispatchStatus::InvalidEmergencyLocation);
    }

    const std::vector<AmbulanceSnapshot> ambulances = ambulanceProvider_.getAmbulances();
    bool hasAvailableAmbulance = false;
    bool hasReachableAmbulance = false;
    DispatchResult best = failure(DispatchStatus::NoReachableAmbulances);

    for (std::vector<AmbulanceSnapshot>::const_iterator it = ambulances.begin();
         it != ambulances.end();
         ++it) {
        if (!it->available) {
            continue;
        }

        hasAvailableAmbulance = true;
        if (it->currentLocation.empty() || !routingService_.isValidLocation(it->currentLocation)) {
            continue;
        }

        const RouteEstimate estimate =
            routingService_.findShortestRoute(it->currentLocation, emergencyLocation);
        if (!estimate.reachable || estimate.travelTime < 0 || estimate.route.empty()) {
            continue;
        }

        const bool isBetterTime = !hasReachableAmbulance ||
                                  estimate.travelTime < best.estimatedTravelTime;
        const bool winsTie = hasReachableAmbulance &&
                             estimate.travelTime == best.estimatedTravelTime &&
                             it->id < best.ambulanceId;

        if (isBetterTime || winsTie) {
            best.status = DispatchStatus::Success;
            best.ambulanceId = it->id;
            best.estimatedTravelTime = estimate.travelTime;
            best.route = estimate.route;
            hasReachableAmbulance = true;
        }
    }

    if (!hasAvailableAmbulance) {
        return failure(DispatchStatus::NoAvailableAmbulances);
    }

    return best;
}
