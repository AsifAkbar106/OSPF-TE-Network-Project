#ifndef MINIMAL_TEST_H
#define MINIMAL_TEST_H

#include <omnetpp.h>

class MinimalTest : public cSimpleModule
{
  public:
    MinimalTest();
    virtual ~MinimalTest();
    
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

#endif // MINIMAL_TEST_H
