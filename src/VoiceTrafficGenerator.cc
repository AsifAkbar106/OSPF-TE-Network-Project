#include "VoiceTrafficGenerator.h"

void VoiceTrafficGenerator::initialize()
{
    // Read parameters from NED file
    packetLength = par("packetLength");
    sendInterval = par("sendInterval").doubleValue();
    startTime = par("startTime").doubleValue();
    stopTime = par("stopTime").doubleValue();
    bitrate = par("bitrate");
    jitterTolerance = par("jitterTolerance");
    
    // Initialize statistics
    packetsSent = 0;
    bytesGenerated = 0;
    
    // Create self-message for scheduling
    sendTimer = new cMessage("sendTimer");
    
    // Schedule first packet
    scheduleAt(simTime() + startTime, sendTimer);
    
    EV << "VoiceTrafficGenerator initialized - bitrate: " << bitrate 
       << " bps, packet length: " << packetLength << " bytes" << endl;
}

void VoiceTrafficGenerator::handleMessage(cMessage *msg)
{
    if (msg == sendTimer) {
        // Check if we should stop generating traffic
        if (simTime() >= stopTime) {
            EV << "VoiceTrafficGenerator stopping at time " << simTime() << endl;
            return;
        }
        
        // Generate voice packet
        generateVoicePacket();
        
        // Schedule next packet
        scheduleNextPacket();
    } else if (strcmp(msg->getName(), "retryPacket") == 0) {
        // Retry sending a packet
        generateVoicePacket();
        delete msg;
    } else {
        // Handle incoming messages (if any)
        delete msg;
    }
}

void VoiceTrafficGenerator::generateVoicePacket()
{
    // Create a new packet
    cPacket *packet = new cPacket("VoicePacket");
    packet->setByteLength(packetLength);
    packet->setTimestamp(simTime());
    
    // Add voice-specific information
    packet->addPar("packetType") = "VOICE";
    packet->addPar("bitrate") = bitrate;
    packet->addPar("jitterTolerance") = jitterTolerance;
    packet->addPar("sequenceNumber") = packetsSent;
    
    // Update statistics
    packetsSent++;
    bytesGenerated += packetLength;
    
    // Send packet if we have output gates
    if (gateSize("out") > 0) {
        cGate *outGate = gate("out", 0);
        // Check if the channel is busy
        cChannel *channel = outGate->getTransmissionChannel();
        if (channel != nullptr) {
            simtime_t finishTime = channel->getTransmissionFinishTime();
            if (finishTime > simTime()) {
                // Channel is busy, schedule packet for later
                EV << "Channel busy, scheduling voice packet for later at " << finishTime << endl;
                scheduleAt(finishTime + SimTime(1e-9), new cMessage("retryPacket"));
                delete packet;
                return;
            }
        }
        send(packet, "out", 0);
        EV << "Sent voice packet #" << packetsSent << " at time " << simTime() << endl;
    } else {
        delete packet;
        EV << "No output gate available, dropping packet" << endl;
    }
}

void VoiceTrafficGenerator::scheduleNextPacket()
{
    // Voice traffic is typically regular with small jitter
    double jitter = uniform(-jitterTolerance/2, jitterTolerance/2);
    double nextTime = sendInterval + jitter;
    
    // Ensure we don't schedule in the past
    if (nextTime <= 0) {
        nextTime = sendInterval * 0.1; // Minimum 10% of send interval
    }
    
    // Only schedule if not already scheduled
    if (!sendTimer->isScheduled()) {
        scheduleAt(simTime() + nextTime, sendTimer);
    }
}

void VoiceTrafficGenerator::finish()
{
    // Cancel any pending messages
    cancelAndDelete(sendTimer);
    
    // Record final statistics
    recordScalar("packets sent", packetsSent);
    recordScalar("bytes generated", bytesGenerated);
    recordScalar("average bitrate", bytesGenerated * 8.0 / simTime().dbl());
    
    EV << "VoiceTrafficGenerator finished - sent " << packetsSent 
       << " packets, " << bytesGenerated << " bytes" << endl;
}
