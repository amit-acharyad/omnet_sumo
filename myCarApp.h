#ifndef __SUMOVEINS_MYCARAPP_H_
#define __SUMOVEINS_MYCARAPP_H_
#include "veins/base/modules/BaseApplLayer.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"

#include <omnetpp.h>

using namespace omnetpp;

using namespace veins;

class MyCarApp : public BaseApplLayer { // Corrected base class
protected:
    double sendInterval;
    cMessage* sendSpeedTimer; // Declare the member variable for the self-message

protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void sendSpeedUpdate(); // Your function to send speed messages
    virtual void finish() override;
public:
    virtual Coord getCurrentPosition();

};

#endif
