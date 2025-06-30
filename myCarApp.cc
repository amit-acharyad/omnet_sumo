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

#include "myCarApp.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"


Define_Module(MyCarApp);

void MyCarApp::initialize(int stage) {
    BaseApplLayer::initialize(stage);

    if (stage == 0) {
        EV << "INFO: MyCarApp initialized at stage 0 on Car: " << getParentModule()->getFullName() << "\n";
        TraCIMobility* mobility = check_and_cast<TraCIMobility*>(getParentModule()->getSubmodule("veinsmobility"));
        traciMobility=mobility;


        if (mobility) {
                    EV << "INFO: Successfully obtained TraCIMobility module: " << mobility->getFullName()<< "\n";
                } else {
                    EV << "WARNING: TraCIMobility module not found!\n";
                }

    }

    if (stage == 1) {
        checkStatusTimer = new cMessage("check-status");
        scheduleAt(simTime() + 1, checkStatusTimer);
    }
}

void MyCarApp::handleSelfMsg(cMessage* msg) {
    if (msg == checkStatusTimer) {
   int id = traciMobility->getId();
     double speed = traciMobility->getSpeed();
     EV << "Time: " << simTime()
         << " | Car: " << getParentModule()->getFullName()
      << " | Id: (" << id
      << " | Speed: " << speed << " m/s\n";

        scheduleAt(simTime() + 1, checkStatusTimer);  // repeat every 1s
    }
}


void MyCarApp::handleLowerMsg(cMessage* msg) {
    // Process messages from lower layers (e.g., MAC, PHY)
    EV << "MyCarApp received a message: " << msg->getName() << endl;
    delete msg;
}
int MyCarApp::numInitStages() const {
    return BaseApplLayer::numInitStages(); // or just return 1;
}
