#ifndef SIMPLEHOST_H
#define SIMPLEHOST_H

#include <omnetpp.h>

using namespace omnetpp;

class SimpleHost : public cSimpleModule
{
  protected:
    std::string hostId;
    
  public:
    SimpleHost();
    virtual ~SimpleHost();
    
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
};

#endif // SIMPLEHOST_H
