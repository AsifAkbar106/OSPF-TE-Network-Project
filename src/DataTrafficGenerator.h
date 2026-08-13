#ifndef DATATRAFFICGENERATOR_H_
#define DATATRAFFICGENERATOR_H_

#include <omnetpp.h>

using namespace omnetpp;

class DataTrafficGenerator : public cSimpleModule
{
private:
    // Parameters
    int packetLength;
    double sendInterval;
    double startTime;
    double stopTime;
    double bitrate;
    double burstInterval;
    
    // Self-messages for scheduling
    cMessage *sendTimer;
    cMessage *burstTimer;
    
    // State
    bool inBurst;
    
    // Statistics
    long packetsSent;
    long bytesGenerated;
    
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
    
    void generateDataPacket();
    void scheduleNextPacket();
    void startBurst();
    void endBurst();
};

Define_Module(DataTrafficGenerator);

#endif /* DATATRAFFICGENERATOR_H_ */
