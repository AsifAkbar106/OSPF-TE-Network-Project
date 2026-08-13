#include "VideoTrafficGenerator.h"

void VideoTrafficGenerator::initialize()
{
    // Read parameters from NED file
    packetLength = par("packetLength");
    sendInterval = par("sendInterval").doubleValue();
    startTime = par("startTime").doubleValue();
    stopTime = par("stopTime").doubleValue();
    bitrate = par("bitrate");
    burstSize = par("burstSize");
    
    // Initialize statistics
    packetsSent = 0;
    bytesGenerated = 0;
    
    // Create self-message for scheduling
    sendTimer = new cMessage("sendTimer");
    
    // Schedule first packet
    scheduleAt(simTime() + startTime, sendTimer);
    
    EV << "VideoTrafficGenerator initialized - bitrate: " << bitrate 
       << " bps, packet length: " << packetLength << " bytes" << endl;
}

void VideoTrafficGenerator::handleMessage(cMessage *msg)
{
    if (msg == sendTimer) {
        // Check if we should stop generating traffic
        if (simTime() >= stopTime) {
            EV << "VideoTrafficGenerator stopping at time " << simTime() << endl;
            return;
        }
        
        // Generate a burst of video packets
        for (int i = 0; i < burstSize; i++) {
            generateVideoPacket();
        }
        
        // Schedule next burst
        scheduleNextPacket();
    } else if (strcmp(msg->getName(), "retryPacket") == 0) {
        // Retry sending a packet
        generateVideoPacket();
        delete msg;
    } else {
        // Handle incoming messages (if any)
        delete msg;
    }
}

void VideoTrafficGenerator::generateVideoPacket()
{
    // Create a new packet
    cPacket *packet = new cPacket("VideoPacket");
    packet->setByteLength(packetLength);
    packet->setTimestamp(simTime());
    
    // Add video-specific information
    packet->addPar("packetType") = "VIDEO";
    packet->addPar("bitrate") = bitrate;
    packet->addPar("frameNumber") = packetsSent;
    
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
                EV << "Channel busy, scheduling packet for later at " << finishTime << endl;
                scheduleAt(finishTime + SimTime(1e-9), new cMessage("retryPacket"));
                delete packet;
                return;
            }
        }
        send(packet, "out", 0);
        EV << "Sent video packet #" << packetsSent << " at time " << simTime() << endl;
    } else {
        delete packet;
        EV << "No output gate available, dropping packet" << endl;
    }
}

void VideoTrafficGenerator::scheduleNextPacket()
{
    double nextTime = exponential(sendInterval);
    
    // Ensure we don't schedule in the past or at current time
    if (nextTime <= 0) {
        nextTime = sendInterval * 0.1; // Minimum 10% of send interval
    }
    
    // Only schedule if not already scheduled
    if (!sendTimer->isScheduled()) {
        scheduleAt(simTime() + nextTime, sendTimer);
    }
}

void VideoTrafficGenerator::finish()
{
    // Cancel any pending messages
    cancelAndDelete(sendTimer);
    
    // Record final statistics
    recordScalar("packets sent", packetsSent);
    recordScalar("bytes generated", bytesGenerated);
    recordScalar("average bitrate", bytesGenerated * 8.0 / simTime().dbl());
    
    EV << "VideoTrafficGenerator finished - sent " << packetsSent 
       << " packets, " << bytesGenerated << " bytes" << endl;
}
