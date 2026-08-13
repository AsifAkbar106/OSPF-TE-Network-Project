#include "RIP_Router.h"

Define_Module(RIP_Router);

RIP_Router::RIP_Router()
{
    // Initialize member variables
}

RIP_Router::~RIP_Router()
{
    // Cleanup if needed
}

void RIP_Router::initialize()
{
    // Initialize RIP specific components
    routerId = getFullName();
    
    // Initialize RIP routing table
    ripTable = new RIPRoutingTable();
    
    // Schedule periodic updates
    scheduleAt(simTime() + par("updateInterval"), new cMessage("RIP_UPDATE"));
    
    EV << "RIP Router " << routerId << " initialized" << endl;
}

void RIP_Router::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (strcmp(msg->getName(), "RIP_UPDATE") == 0) {
            sendRIPUpdates();
            scheduleAt(simTime() + par("updateInterval"), msg);
        }
        else {
            delete msg;
        }
    }
    else {
        // Handle incoming RIP messages
        if (strcmp(msg->getName(), "RIP Update") == 0) {
            handleRIPUpdate(msg);
        }
        else {
            // Forward other messages
            forwardMessage(msg);
        }
    }
}

void RIP_Router::sendRIPUpdates()
{
    // Send RIP updates to all neighbors
    for (int i = 0; i < gateSize("out"); i++) {
        cMessage *update = new cMessage("RIP Update");
        update->addPar("routerId");
        update->addPar("timestamp");
        
        update->par("routerId").setStringValue(routerId.c_str());
        update->par("timestamp").setDoubleValue(simTime().dbl());
        
        // Simplified RIP update - no complex route arrays
        
        send(update, "out", i);
    }
    
    EV << "RIP Router " << routerId << " sent updates" << endl;
}

void RIP_Router::handleRIPUpdate(cMessage *update)
{
    EV << "RIP Router " << routerId << " received update from " << update->par("routerId").stringValue() << endl;
    
    // Simplified RIP processing
    EV << "Processing RIP update from " << update->par("routerId").stringValue() << endl;
    
    // Update routing table
    updateRoutingTable();
    
    delete update;
}

// Simplified RIP route processing removed

void RIP_Router::updateRoutingTable()
{
    // Simulate routing table update
    EV << "Updating RIP routing table for router " << routerId << endl;
}

void RIP_Router::forwardMessage(cMessage *msg)
{
    // Simple message forwarding based on RIP routing
    // In a real implementation, this would use the routing table
    int outGate = intuniform(0, gateSize("out") - 1);
    send(msg, "out", outGate);
}

void RIP_Router::finish()
{
    if (ripTable) {
        delete ripTable;
    }
}
