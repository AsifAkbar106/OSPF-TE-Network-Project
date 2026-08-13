#ifndef PERFORMANCE_MONITOR_H
#define PERFORMANCE_MONITOR_H

#include <omnetpp.h>
#include <fstream>
using namespace omnetpp;

class PerformanceMonitor : public cSimpleModule
{
  protected:
    // Performance metrics
    double convergenceTime;
    double totalThroughput;
    double averageLatency;
    double packetLoss;
    
    // Statistics vectors
    cOutVector throughputVector;
    cOutVector latencyVector;
    cOutVector packetLossVector;
    cOutVector convergenceTimeVector;
    
    // Output files
    std::ofstream outputFile;
    std::ofstream statisticsFile;
    
  public:
    PerformanceMonitor();
    virtual ~PerformanceMonitor();
    
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
    
    // Performance measurement methods
    void measurePerformance();
    double measureThroughput();
    double measureLatency();
    double measurePacketLoss();
    double measureConvergenceTime();
    
    // Analysis methods
    void compareProtocols();
};

#endif // PERFORMANCE_MONITOR_H
