//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef __SUMOVEINS_MYCARAPP_H_
#define __SUMOVEINS_MYCARAPP_H_
#include "veins/base/modules/BaseApplLayer.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"

#include <omnetpp.h>

using namespace omnetpp;

using namespace veins;
/*
class MyCarApp : public BaseApplLayer {
protected:
        TraCIMobility* traciMobility = nullptr;
        cMessage* checkStatusTimer = nullptr;

        virtual void initialize(int stage) override;
        virtual int numInitStages() const override;

        virtual void handleSelfMsg(cMessage* msg) override;
        virtual void handleLowerMsg(cMessage* msg) override;
};*/
class MyCarApp : public BaseApplLayer { // Corrected base class
protected:
    double sendInterval;
    cMessage* sendSpeedTimer; // Declare the member variable for the self-message

protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void sendSpeedUpdate(); // Your function to send speed messages
    virtual void finish() override;

};

#endif
