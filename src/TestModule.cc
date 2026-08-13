#include "TestModule.h"
#include "TestMsg_m.h"

Define_Module(TestModule);

void TestModule::initialize()
{
    EV << "TestModule initialized" << endl;
}

void TestModule::handleMessage(cMessage *msg)
{
    if (dynamic_cast<TestMsg*>(msg)) {
        TestMsg *testMsg = check_and_cast<TestMsg*>(msg);
        EV << "Received TestMsg: " << testMsg->getTest() << endl;
    }
    delete msg;
}
