#include "PerformanceMonitor.h"
#include <fstream>

Define_Module(PerformanceMonitor);

PerformanceMonitor::PerformanceMonitor()
{
    convergenceTime = 0.0;
    totalThroughput = 0.0;

}

PerformanceMonitor::~PerformanceMonitor()
{
    // Cleanup if needed
}

void PerformanceMonitor::initialize()
{
    // Initialize performance metrics
    convergenceTime = 0.0;
    totalThroughput = 0.0;
    averageLatency = 0.0;
    packetLoss = 0.0;
    
    // Initialize statistics
    throughputVector.setName("Throughput");
    latencyVector.setName("Latency");
    packetLossVector.setName("PacketLoss");
    convergenceTimeVector.setName("ConvergenceTime");
    
    // Schedule periodic measurements
    scheduleAt(simTime() + par("measurementInterval"), new cMessage("MEASURE"));
    
    // Open output files
    outputFile.open(par("outputFile").stringValue());
    statisticsFile.open(par("statisticsFile").stringValue());
    
    EV << "Performance Monitor initialized" << endl;
}

void PerformanceMonitor::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (strcmp(msg->getName(), "MEASURE") == 0) {
            measurePerformance();
            scheduleAt(simTime() + par("measurementInterval"), msg);
        }
        else {
            delete msg;
        }
    }
    else {
        // Handle incoming messages
        delete msg;
    }
}

void PerformanceMonitor::measurePerformance()
{
    // Measure throughput
    double currentThroughput = measureThroughput();
    throughputVector.record(currentThroughput);
    totalThroughput += currentThroughput;
    
    // Measure latency
    double currentLatency = measureLatency();
    latencyVector.record(currentLatency);
    averageLatency = (averageLatency + currentLatency) / 2.0;
    
    // Measure packet loss
    double currentPacketLoss = measurePacketLoss();
    packetLossVector.record(currentPacketLoss);
    packetLoss = (packetLoss + currentPacketLoss) / 2.0;
    
    // Measure convergence time
    double currentConvergenceTime = measureConvergenceTime();
    if (currentConvergenceTime > 0) {
        convergenceTimeVector.record(currentConvergenceTime);
        convergenceTime = currentConvergenceTime;
    }
    
    // Log measurements
    EV << "Performance Metrics - Throughput: " << currentThroughput 
       << " Mbps, Latency: " << currentLatency << " ms, Packet Loss: " 
       << currentPacketLoss << "%, Convergence: " << currentConvergenceTime << " s" << endl;
    
    // Write to output file
    if (outputFile.is_open()) {
        outputFile << simTime().dbl() << "," << currentThroughput << "," 
                   << currentLatency << "," << currentPacketLoss << "," 
                   << currentConvergenceTime << std::endl;
    }
}

double PerformanceMonitor::measureThroughput()
{
    // Simulate throughput measurement
    // In a real implementation, this would measure actual network throughput
    double baseThroughput = 100.0; // Base throughput in Mbps
    double variation = uniform(-10.0, 10.0); // Random variation
    return std::max(0.0, baseThroughput + variation);
}

double PerformanceMonitor::measureLatency()
{
    // Simulate latency measurement
    // In a real implementation, this would measure actual round-trip time
    double baseLatency = 5.0; // Base latency in ms
    double variation = uniform(-2.0, 2.0); // Random variation
    return std::max(0.0, baseLatency + variation);
}

double PerformanceMonitor::measurePacketLoss()
{
    // Simulate packet loss measurement
    // In a real implementation, this would measure actual packet loss
    double baseLoss = 0.1; // Base packet loss in %
    double variation = uniform(-0.05, 0.05); // Random variation
    return std::max(0.0, baseLoss + variation);
}

double PerformanceMonitor::measureConvergenceTime()
{
    // Measure routing convergence time
    // This is a simplified implementation
    static double lastConvergenceTime = 0.0;
    static simtime_t lastCheck = 0.0;
    
    simtime_t currentTime = simTime();
    if (currentTime - lastCheck > 10.0) { // Check every 10 seconds
        // Simulate convergence time measurement
        double convergence = uniform(0.1, 2.0); // Random convergence time
        lastConvergenceTime = convergence;
        lastCheck = currentTime;
        return convergence;
    }
    
    return 0.0; // No new convergence measurement
}

void PerformanceMonitor::compareProtocols()
{
    // Compare OSPF-TE vs RIP performance
    EV << "=== Protocol Comparison ===" << endl;
    EV << "OSPF-TE Performance:" << endl;
    EV << "  - Average Throughput: " << totalThroughput << " Mbps" << endl;
    EV << "  - Average Latency: " << averageLatency << " ms" << endl;
    EV << "  - Packet Loss: " << packetLoss << "%" << endl;
    EV << "  - Convergence Time: " << convergenceTime << " s" << endl;
    
    // Write comparison to statistics file
    if (statisticsFile.is_open()) {
        statisticsFile << "OSPF-TE Results:" << std::endl;
        statisticsFile << "Throughput: " << totalThroughput << " Mbps" << std::endl;
        statisticsFile << "Latency: " << averageLatency << " ms" << std::endl;
        statisticsFile << "Packet Loss: " << packetLoss << "%" << std::endl;
        statisticsFile << "Convergence Time: " << convergenceTime << " s" << std::endl;
    }
}

void PerformanceMonitor::finish()
{
    // Final performance comparison
    compareProtocols();
    
    // Close output files
    if (outputFile.is_open()) {
        outputFile.close();
    }
    if (statisticsFile.is_open()) {
        statisticsFile.close();
    }
    
    // Record final statistics
    recordScalar("Final_Throughput_Mbps", totalThroughput);
    recordScalar("Final_Latency_ms", averageLatency);
    recordScalar("Final_PacketLoss_Percent", packetLoss);
    recordScalar("Final_ConvergenceTime_s", convergenceTime);
}
