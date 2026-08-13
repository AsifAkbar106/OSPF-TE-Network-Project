#include "OSPFTE_Router.h"
#include <algorithm>
#include <cmath>
#include<omnetpp.h>
using namespace omnetpp;
Define_Module(OSPFTE_Router);

OSPFTE_Router::OSPFTE_Router()
{
    ted = nullptr;
    cbr = nullptr;
}

OSPFTE_Router::~OSPFTE_Router()
{
    if (ted) {
        delete ted;
    }
    if (cbr) {
        delete cbr;
    }
}

void OSPFTE_Router::initialize()
{
    // Initialize OSPF-TE specific components
    routerId = par("routerId").stringValue();
    if (routerId.empty()) {
        routerId = getFullName();
    }
    
    // Initialize TED
    ted = new TrafficEngineeringDatabase();
    
    // Initialize CBR (Constraint-Based Routing)
    cbr = new ConstraintBasedRouting();
    
    // Schedule TED updates
    scheduleAt(simTime() + par("tedUpdateInterval"), new cMessage("TED_UPDATE"));
    
    // Schedule hello messages
    scheduleAt(simTime() + par("helloInterval"), new cMessage("HELLO_TIMER"));
    
    EV << "OSPF-TE Router " << routerId << " initialized" << endl;
}

void OSPFTE_Router::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (strcmp(msg->getName(), "TED_UPDATE") == 0) {
            updateTrafficEngineeringDatabase();
            scheduleAt(simTime() + par("tedUpdateInterval"), msg);
        }
        else if (strcmp(msg->getName(), "HELLO_TIMER") == 0) {
            sendHelloMessages();
            scheduleAt(simTime() + par("helloInterval"), msg);
        }
        else {
            delete msg;
        }
    }
    else {
        // Handle incoming OSPF-TE messages
        if (strcmp(msg->getName(), "OSPF-TE Hello") == 0) {
            handleHelloMessage(msg);
        }
        else if (strcmp(msg->getName(), "OSPF-TE LSA") == 0) {
            handleLSA(msg);
        }
        else {
            // Forward other messages
            forwardMessage(msg);
        }
    }
}

void OSPFTE_Router::updateTrafficEngineeringDatabase()
{
    // Simulate updating link metrics based on current network conditions
    EV << "Updating TED for router " << routerId << endl;
    
    // Simulate metric calculation for all links
    for (int i = 0; i < gateSize("out"); i++) {
        double bandwidth = getLinkBandwidth(i);
        double delay = getLinkDelay(i);
        double load = getLinkLoad(i);
        
        // OSPF-TE metric calculation
        double metric = calculateOSPF_TEMetric(bandwidth, delay, load);
        
        // Update TED
        ted->updateLink("destination_" + std::to_string(i), bandwidth, delay, load, metric);
    }
    
    // Recalculate routes using CBR
    recalculateRoutes();
    
    EV << "TED updated for router " << routerId << endl;
}

double OSPFTE_Router::calculateOSPF_TEMetric(double bandwidth, double delay, double load)
{
    // Normalize values (higher bandwidth = lower cost, lower delay = lower cost, lower load = lower cost)
    double bandwidthCost = 1.0 / (bandwidth + 1.0); // Avoid division by zero
    double delayCost = delay / 1000.0; // Convert to milliseconds
    double loadCost = load / 100.0; // Convert to percentage
    
    // Weighted combination
    double bwWeight = par("bandwidthWeight");
    double delayWeight = par("delayWeight");
    double loadWeight = par("loadWeight");
    
    double totalCost = bwWeight * bandwidthCost + delayWeight * delayCost + loadWeight * loadCost;
    
    return std::max(1.0, totalCost); // Minimum cost of 1
}

void OSPFTE_Router::recalculateRoutes()
{
    // Simulate route recalculation using CBR
    EV << "Recalculating routes using CBR for router " << routerId << endl;
    
    // Get all known destinations from TED
    std::vector<std::string> destinations = ted->getAllDestinations();
    
    for (auto& dest : destinations) {
        // Simulate finding optimal path
        EV << "Calculating optimal path to " << dest << endl;
    }
}

void OSPFTE_Router::sendHelloMessages()
{
    // Send hello messages to neighbors
    for (int i = 0; i < gateSize("out"); i++) {
        cMessage *hello = new cMessage("OSPF-TE Hello");
        hello->addPar("routerId");
        hello->addPar("areaId");
        hello->addPar("helloInterval");
        hello->addPar("deadInterval");
        
        hello->par("routerId").setStringValue(routerId.c_str());
        hello->par("areaId").setStringValue(par("ospfAreaId").stringValue());
        hello->par("helloInterval").setDoubleValue(par("helloInterval"));
        hello->par("deadInterval").setDoubleValue(par("deadInterval"));
        
        send(hello, "out", i);
    }
    
    EV << "OSPF-TE Router " << routerId << " sent hello messages" << endl;
}

void OSPFTE_Router::handleHelloMessage(cMessage *hello)
{
    EV << "Received hello from router " << hello->par("routerId").stringValue() << endl;
    
    // Update neighbor information
    // In a real implementation, this would maintain a neighbor table
    
    delete hello;
}

void OSPFTE_Router::handleLSA(cMessage *lsa)
{
    EV << "Received LSA from router " << lsa->par("routerId").stringValue() << endl;
    
    // Update TED with new link state information
    // In a real implementation, this would process the LSA and update TED
    
    delete lsa;
}

void OSPFTE_Router::forwardMessage(cMessage *msg)
{
    // Simple message forwarding
    // In a real implementation, this would use routing table
    int outGate = intuniform(0, gateSize("out") - 1);
    send(msg, "out", outGate);
}

double OSPFTE_Router::getLinkBandwidth(int gateIndex)
{
    // Simulate bandwidth measurement
    // In a real implementation, this would query the actual link
    return 1000.0; // 1 Gbps
}

double OSPFTE_Router::getLinkDelay(int gateIndex)
{
    // Simulate delay measurement
    // In a real implementation, this would measure actual delay
    return 2.0; // 2 ms
}

double OSPFTE_Router::getLinkLoad(int gateIndex)
{
    // Simulate load measurement
    // In a real implementation, this would measure actual utilization
    return 30.0; // 30% utilization
}

void OSPFTE_Router::finish()
{
    if (ted) {
        delete ted;
    }
    if (cbr) {
        delete cbr;
    }
}

// TrafficEngineeringDatabase implementation
void TrafficEngineeringDatabase::updateLink(std::string dest, double bw, double delay, double load, double metric)
{
    // Find existing link or add new one
    for (auto& link : links) {
        if (link.destination == dest) {
            link.bandwidth = bw;
            link.delay = delay;
            link.load = load;
            link.metric = metric;
            return;
        }
    }
    
    // Add new link
    LinkInfo newLink;
    newLink.destination = dest;
    newLink.bandwidth = bw;
    newLink.delay = delay;
    newLink.load = load;
    newLink.metric = metric;
    links.push_back(newLink);
}

std::vector<std::string> TrafficEngineeringDatabase::getAllDestinations()
{
    std::vector<std::string> destinations;
    for (const auto& link : links) {
        destinations.push_back(link.destination);
    }
    return destinations;
}

TrafficEngineeringDatabase::LinkInfo* TrafficEngineeringDatabase::getLinkInfo(std::string dest)
{
    for (auto& link : links) {
        if (link.destination == dest) {
            return &link;
        }
    }
    return nullptr;
}

// ConstraintBasedRouting implementation
void ConstraintBasedRouting::findOptimalPath(std::string source, std::string destination, 
                                           TrafficEngineeringDatabase *ted)
{
    // Simple implementation - in a real system this would use Dijkstra's algorithm
    // or other pathfinding algorithms with constraints
    EV << "Finding optimal path from " << source << " to " << destination << endl;
}
