#include "MinimalTest.h"

Define_Module(MinimalTest);

void MinimalTest::initialize()
{
    EV << "MinimalTest initialized" << endl;
}

void MinimalTest::handleMessage(cMessage *msg)
{
    EV << "MinimalTest received message: " << msg->getName() << endl;
    delete msg;
}
