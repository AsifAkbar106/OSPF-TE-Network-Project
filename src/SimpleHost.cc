#include "SimpleHost.h"

using namespace omnetpp;

Define_Module(SimpleHost);

SimpleHost::SimpleHost()
{
    // Constructor
}

SimpleHost::~SimpleHost()
{
    // Destructor
}

void SimpleHost::initialize()
{
    // Initialize host
    hostId = par("hostId").stringValue();
    if (hostId.empty()) {
        hostId = getFullName();
    }
    
    EV << "SimpleHost " << hostId << " initialized" << endl;
}

void SimpleHost::handleMessage(cMessage *msg)
{
    // Simple message handling - just forward to output
    if (msg->isSelfMessage()) {
        delete msg;
        return;
    }
    
    // Forward message to output gate
    if (gateSize("out") > 0) {
        int outGate = intuniform(0, gateSize("out") - 1);
        send(msg, "out", outGate);
    } else {
        delete msg;
    }
}

void SimpleHost::finish()
{
    EV << "SimpleHost " << hostId << " finished" << endl;
}
