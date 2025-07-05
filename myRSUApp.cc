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
#include "SpeedMessage_m.h" // Make sure this header is correct and the message is defined

Define_Module(MyRSUApp);

void MyRSUApp::initialize(int stage)
{

    EV<<"Initializing the RSUAPP"<<endl;
    BaseApplLayer::initialize(stage); // Call base class initialize
    // Any RSU-specific initialization (e.g., setting up timers, parameters) can go here
    if (stage == 0) {
        EV_INFO << "MyRSUApp initialized on RSU: " << getParentModule()->getFullName() << endl;
    }
}

// This method handles messages coming from lower layers (e.g., network layer)
void MyRSUApp::handleLowerMsg(cMessage* msg)
{
    // Try to cast the incoming message to SpeedMessage
    EV<<"Handle lower message in RSUAPP"<<endl;

    SpeedMessage* speedMsg = dynamic_cast<SpeedMessage*>(msg);
    EV<<"Handle lower message in RSUAPP"<<endl;
    if (speedMsg) {
        // It's a SpeedMessage, process it
        double receivedSpeed = speedMsg->getSpeed();
        const char* sender = speedMsg->getSenderId();
        EV_INFO << "MyRSUApp: Received speed update from " << sender << ": " << receivedSpeed << " m/s" << endl;

        // You can add more complex logic here:
        // - Store the speed in a data structure
        // - Perform analysis (e.g., average speed, detect anomalies)
        // - Trigger further actions based on the received speed

    } else {
        // It's a different type of message from the lower layer
        EV_WARN << "MyRSUApp: Received non-SpeedMessage from lower layer: " << msg->getName() << endl;
    }

    // IMPORTANT: Always delete the message after processing to prevent memory leaks
    delete msg;
}

// This method handles self-messages (messages scheduled by the module itself)
// and potentially other control messages within the application layer.
void MyRSUApp::handleSelfMsg(cMessage* msg)
{

    SpeedMessage* speedMsg = dynamic_cast<SpeedMessage*>(msg);
    EV<<"Handle self message in RSUAPP"<<endl;
    if (speedMsg) {
        // It's a SpeedMessage, process it
        double receivedSpeed = speedMsg->getSpeed();
        const char* sender = speedMsg->getSenderId();
        EV_INFO << "MyRSUApp(HandleselfMessage):: Received speed update from " << sender << ": " << receivedSpeed << " m/s" << endl;

        // You can add more complex logic here:
        // - Store the speed in a data structure
        // - Perform analysis (e.g., average speed, detect anomalies)
        // - Trigger further actions based on the received speed

    }
    delete msg; // Delete self-message after handling
}
void MyRSUApp::handleMessage(cMessage* msg)
{
    // Add logic for self-messages if you plan to use timers or internal events

    SpeedMessage* speedMsg = dynamic_cast<SpeedMessage*>(msg);
    EV<<"Handle  message in RSUAPP"<<endl;
    if (speedMsg) {
        // It's a SpeedMessage, process it
        double receivedSpeed = speedMsg->getSpeed();
        const char* sender = speedMsg->getSenderId();
        EV_INFO << "MyRSUApp(HandleMessage):: Received speed update from " << sender << ": " << receivedSpeed << " m/s" << endl;

        // You can add more complex logic here:
        // - Store the speed in a data structure
        // - Perform analysis (e.g., average speed, detect anomalies)
        // - Trigger further actions based on the received speed

    }    delete msg; // Delete self-message after handling
}


void MyRSUApp::finish()
{
    BaseApplLayer::finish();
    // Any cleanup operations before the simulation ends
}
