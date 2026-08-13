#include "DataTrafficGenerator.h"

void DataTrafficGenerator::initialize()
{
    // Read parameters from NED file
    packetLength = par("packetLength");
    sendInterval = par("sendInterval").doubleValue();
    startTime = par("startTime").doubleValue();
    stopTime = par("stopTime").doubleValue();
    bitrate = par("bitrate");
    burstInterval = par("burstInterval").doubleValue();
    
    // Initialize statistics
    packetsSent = 0;
    bytesGenerated = 0;
    inBurst = false;
    
    // Create self-messages for scheduling
    sendTimer = new cMessage("sendTimer");
    burstTimer = new cMessage("burstTimer");
    
    // Schedule first burst
    scheduleAt(simTime() + startTime, burstTimer);
    
    EV << "DataTrafficGenerator initialized - bitrate: " << bitrate 
       << " bps, packet length: " << packetLength << " bytes" << endl;
}

void DataTrafficGenerator::handleMessage(cMessage *msg)
{
    if (msg == burstTimer) {
        // Check if we should stop generating traffic
        if (simTime() >= stopTime) {
            EV << "DataTrafficGenerator stopping at time " << simTime() << endl;
            return;
        }
        
        if (!inBurst) {
            startBurst();
        } else {
            endBurst();
        }
    } else if (msg == sendTimer) {
        // Generate data packet during burst
        generateDataPacket();
        
        // Schedule next packet in burst
        if (inBurst) {
            scheduleNextPacket();
        }
    } else if (strcmp(msg->getName(), "retryPacket") == 0) {
        // Retry sending a packet
        generateDataPacket();
        delete msg;
    } else {
        // Handle incoming messages (if any)
        delete msg;
    }
}

void DataTrafficGenerator::generateDataPacket()
{
    // Create a new packet
    cPacket *packet = new cPacket("DataPacket");
    packet->setByteLength(packetLength);
    packet->setTimestamp(simTime());
    
    // Add data-specific information
    packet->addPar("packetType") = "DATA";
    packet->addPar("bitrate") = bitrate;
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
                EV << "Channel busy, scheduling data packet for later at " << finishTime << endl;
                scheduleAt(finishTime + SimTime(1e-9), new cMessage("retryPacket"));
                delete packet;
                return;
            }
        }
        send(packet, "out", 0);
        EV << "Sent data packet #" << packetsSent << " at time " << simTime() << endl;
    } else {
        delete packet;
        EV << "No output gate available, dropping packet" << endl;
    }
}

void DataTrafficGenerator::scheduleNextPacket()
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

void DataTrafficGenerator::startBurst()
{
    inBurst = true;
    EV << "Starting data burst at time " << simTime() << endl;
    
    // Schedule first packet in burst with small delay to avoid scheduling conflicts
    scheduleAt(simTime() + SimTime(1e-9), sendTimer);
    
    // Schedule end of burst (random duration between 1-10 seconds)
    double burstDuration = uniform(1.0, 10.0);
    scheduleAt(simTime() + burstDuration, burstTimer);
}

void DataTrafficGenerator::endBurst()
{
    inBurst = false;
    EV << "Ending data burst at time " << simTime() << endl;
    
    // Cancel any pending send timer
    if (sendTimer->isScheduled()) {
        cancelEvent(sendTimer);
    }
    
    // Schedule next burst
    double nextBurstTime = exponential(burstInterval);
    scheduleAt(simTime() + nextBurstTime, burstTimer);
}

void DataTrafficGenerator::finish()
{
    // Cancel any pending messages
    cancelAndDelete(sendTimer);
    cancelAndDelete(burstTimer);
    
    // Record final statistics
    recordScalar("packets sent", packetsSent);
    recordScalar("bytes generated", bytesGenerated);
    recordScalar("average bitrate", bytesGenerated * 8.0 / simTime().dbl());
    
    EV << "DataTrafficGenerator finished - sent " << packetsSent 
       << " packets, " << bytesGenerated << " bytes" << endl;
}
