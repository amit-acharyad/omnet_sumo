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

#ifndef __SUMOVEINS_MYRSUAPP_H_
#define __SUMOVEINS_MYRSUAPP_H_

#include <omnetpp.h>
#include "veins/base/modules/BaseApplLayer.h"
using namespace omnetpp;
using namespace veins;
/*
class MyRSUApp : public BaseApplLayer
{
  protected:
    virtual void initialize(int stage) override;
    virtual int numInitStages() const override;
    virtual void handleSelfMsg(cMessage* msg) override;
     virtual void handleLowerMsg(cMessage* msg) override;
};
*/
class MyRSUApp : public BaseApplLayer {
  public:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage* msg) override;
    virtual void handleSelfMsg(cMessage* msg) override;

    virtual void handleLowerMsg(cMessage *msg) override;

    virtual void finish() override;

  protected:
    // No specific protected members needed for this simple RSU app
};
#endif
