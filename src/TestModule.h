#ifndef TEST_MODULE_H
#define TEST_MODULE_H

#include <omnetpp.h>

class TestModule : public cSimpleModule
{
  public:
    TestModule();
    virtual ~TestModule();
    
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

#endif // TEST_MODULE_H
