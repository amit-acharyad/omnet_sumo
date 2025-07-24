
#include "myCarApp.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"
#include "SpeedMessage_m.h"

Define_Module(MyCarApp);

void MyCarApp::initialize(int stage)
{
    EV<<"Initializing the my carapp"<<endl;
    BaseApplLayer::initialize(stage); // Call base class initialize

    if (stage == 0) {
        // Get parameters from NED file
        sendInterval = par("sendInterval").doubleValue();

        // Create and schedule the first self-message to send speed
        // It's good practice to declare this as a member variable if you need to cancel it later
        sendSpeedTimer = new cMessage("sendSpeedMsg");
        scheduleAt(simTime() + sendInterval, sendSpeedTimer);
    }
}

void MyCarApp::handleMessage(cMessage *msg)
{
    // Check if it's a self-message (scheduled by this module)
    if (msg->isSelfMessage()) {
        if (msg == sendSpeedTimer) { // Using the member variable for comparison
            sendSpeedUpdate();  // Function to send speed message

            // Reschedule the next self-message
            scheduleAt(simTime() + sendInterval, sendSpeedTimer);
        } else {
            // Handle other types of self-messages if you introduce them
            EV_WARN << "MyCarApp: Unhandled self-message: " << msg->getName() << endl;
            delete msg; // Delete unknown self-messages
        }
    }
    // Check if it's a message from the lower layer
    else if (msg->getArrivalGate() != nullptr &&
             strcmp(msg->getArrivalGate()->getBaseName(), "lowerLayerIn") == 0)
    {
        // For a car, you might not expect many application-level messages from lower layers.
        // If you do, you'd add specific handling here (e.g., for acknowledgements, or RSU commands)
        EV_INFO << "MyCarApp: Received message from lower layer: " << msg->getName() << endl;
        delete msg; // Don't forget to free the memory
    }
    // Handle other types of messages (e.g., control messages from OMNeT++ core)
    else {
        EV_WARN << "MyCarApp: Unhandled message: " << msg->getName() << endl;
        delete msg;
    }
}

void MyCarApp::sendSpeedUpdate()
{
    // Get the host vehicle's mobility module
    TraCIMobility* mobility = check_and_cast<TraCIMobility*>(getParentModule()->getSubmodule("veinsmobility"));

    if (!mobility) {
        EV_ERROR << "MyCarApp: Could not find mobility module!" << endl;
        return;
    }

    // Get the current speed
    double currentSpeed = mobility->getSpeed();
    const char* xStr = getParentModule()->getDisplayString().getTagArg("p", 0);
    const char* yStr = getParentModule()->getDisplayString().getTagArg("p", 1);
    Coord pos(atof(xStr), atof(yStr), 0);

    EV_INFO << "OMNeT++ 5.x vehicle position: (" << pos.x << ", " << pos.y << ")" << endl;
    Coord currentPosition=mobility->getCurrentPosition();
    EV<<"Position from TRACI X : "<<currentPosition.x<<"Y: "<<currentPosition.y<<"Z: "<<currentPosition.z<<endl;


    // Create a new SpeedMessage
    SpeedMessage* speedMsg = new SpeedMessage("speedMessage");
    speedMsg->setSpeed(currentSpeed);
    // Set sender ID (e.g., vehicle ID or module full name)
    speedMsg->setSenderId(getParentModule()->getFullName());
    speedMsg->setChannelNumber(0);
    // Set the recipient to the broadcast address for the network layer
    // Optionally set sender's network layer address if needed by your network protocol


    EV_INFO << "MyCarApp: Broadcasting speed update: " << currentSpeed
            << " m/s from " << speedMsg->getSenderId()<<"IN channel"<<speedMsg->getChannelNumber() << endl;

    // Send the message to the lower layer (network layer) with 0 delay for immediate sending
    sendDelayedDown(speedMsg,0);
}

// Removed handleLowerMsg as it was conflicting and likely not needed for a car's receiving logic.
// If your car app needs to receive specific application messages from lower layers,
// implement that logic within handleMessage's "lowerLayerIn" branch.
Coord MyCarApp:: getCurrentPosition(){
    const char* xStr = getParentModule()->getDisplayString().getTagArg("p", 0);
    const char* yStr = getParentModule()->getDisplayString().getTagArg("p", 1);
    Coord pos(atof(xStr), atof(yStr), 0);

    EV_INFO << "OMNeT++ 5.x vehicle position: (" << pos.x << ", " << pos.y << ")" << endl;
    return pos;
}
void MyCarApp::finish()
{
    BaseApplLayer::finish();
    // Clean up the self-message if it's still scheduled
    if (sendSpeedTimer && sendSpeedTimer->isScheduled()) {
        cancelAndDelete(sendSpeedTimer);
    }
}
