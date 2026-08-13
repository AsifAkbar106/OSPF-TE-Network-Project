#ifndef OSPFTE_ROUTER_H
#define OSPFTE_ROUTER_H

#include <omnetpp.h>

using namespace omnetpp;

class TrafficEngineeringDatabase;
class ConstraintBasedRouting;

class OSPFTE_Router : public cSimpleModule
{
  protected:
    std::string routerId;
    TrafficEngineeringDatabase *ted;
    ConstraintBasedRouting *cbr;
    
  public:
    OSPFTE_Router();
    virtual ~OSPFTE_Router();
    
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
    
    // OSPF-TE specific methods
    void updateTrafficEngineeringDatabase();
    double calculateOSPF_TEMetric(double bandwidth, double delay, double load);
    void recalculateRoutes();
    void sendHelloMessages();
    void handleHelloMessage(cMessage *hello);
    void handleLSA(cMessage *lsa);
    void forwardMessage(cMessage *msg);
    
    // Link metric measurement methods
    double getLinkBandwidth(int gateIndex);
    double getLinkDelay(int gateIndex);
    double getLinkLoad(int gateIndex);
};

// Traffic Engineering Database
class TrafficEngineeringDatabase
{
  private:
    struct LinkInfo {
        std::string destination;
        double bandwidth;
        double delay;
        double load;
        double metric;
    };
    
    std::vector<LinkInfo> links;
    
  public:
    void updateLink(std::string dest, double bw, double delay, double load, double metric);
    std::vector<std::string> getAllDestinations();
    LinkInfo* getLinkInfo(std::string dest);
};

// Constraint-Based Routing
class ConstraintBasedRouting
{
  public:
    void findOptimalPath(std::string source, std::string destination, 
                        TrafficEngineeringDatabase *ted);
};

#endif // OSPFTE_ROUTER_H
