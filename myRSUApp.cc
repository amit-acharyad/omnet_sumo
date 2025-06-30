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

#include "myRSUApp.h"
#include "veins/base/modules/BaseLayer.h"
Define_Module(MyRSUApp);

void MyRSUApp::initialize(int stage) {
    BaseApplLayer::initialize(stage);

    if (stage == 0) {
        EV << "MyRSUApp initialized at stage 0 on RSU: " << getParentModule()->getFullName() << endl;
        // Add parameter fetch or timer setup here if needed
    }
}
int MyRSUApp::numInitStages() const {
    return BaseApplLayer::numInitStages(); // or just return 1;
}
void MyRSUApp::handleSelfMsg(cMessage* msg) {
    EV << "MyRSUApp received a message: " << msg->getName() << endl;
    delete msg;
}

void MyRSUApp::handleLowerMsg(cMessage* msg) {
    // Process incoming messages from lower layers
    EV << "MyRSUApp received a message: " << msg->getName() << endl;
    delete msg;
}

