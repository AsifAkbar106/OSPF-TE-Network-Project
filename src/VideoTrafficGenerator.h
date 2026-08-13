#ifndef VIDEOTRAFFICGENERATOR_H_
#define VIDEOTRAFFICGENERATOR_H_

#include <omnetpp.h>

using namespace omnetpp;

class VideoTrafficGenerator : public cSimpleModule
{
private:
    // Parameters
    int packetLength;
    double sendInterval;
    double startTime;
    double stopTime;
    double bitrate;
    int burstSize;
    
    // Self-messages for scheduling
    cMessage *sendTimer;
    
    // Statistics
    long packetsSent;
    long bytesGenerated;
    
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
    
    void generateVideoPacket();
    void scheduleNextPacket();
};

Define_Module(VideoTrafficGenerator);

#endif /* VIDEOTRAFFICGENERATOR_H_ */
