#ifndef RIP_ROUTER_H
#define RIP_ROUTER_H

#include <omnetpp.h>
using namespace omnetpp;
class RIPRoutingTable;

class RIP_Router : public cSimpleModule
{
  protected:
    std::string routerId;
    RIPRoutingTable *ripTable;
    
  public:
    RIP_Router();
    virtual ~RIP_Router();
    
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
    
    // RIP specific methods
    void sendRIPUpdates();
    void handleRIPUpdate(cMessage *update);
    void updateRoutingTable();
    void forwardMessage(cMessage *msg);
};

// RIP Route structure
struct RIPRoute {
    std::string destination;
    std::string nextHop;
    int metric;
    double timestamp;
};

// RIP Routing Table
class RIPRoutingTable
{
  private:
    std::vector<RIPRoute> routes;
    
  public:
    void addRoute(RIPRoute route);
    RIPRoute* findRoute(std::string destination);
    std::vector<RIPRoute> getAllRoutes();
    void removeRoute(std::string destination);
};

#endif // RIP_ROUTER_H
