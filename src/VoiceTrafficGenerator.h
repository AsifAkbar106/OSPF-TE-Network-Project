#ifndef VOICETRAFFICGENERATOR_H_
#define VOICETRAFFICGENERATOR_H_

#include <omnetpp.h>

using namespace omnetpp;

class VoiceTrafficGenerator : public cSimpleModule
{
private:
    // Parameters
    int packetLength;
    double sendInterval;
    double startTime;
    double stopTime;
    double bitrate;
    double jitterTolerance;
    
    // Self-messages for scheduling
    cMessage *sendTimer;
    
    // Statistics
    long packetsSent;
    long bytesGenerated;
    
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
    
    void generateVoicePacket();
    void scheduleNextPacket();
};

Define_Module(VoiceTrafficGenerator);

#endif /* VOICETRAFFICGENERATOR_H_ */
