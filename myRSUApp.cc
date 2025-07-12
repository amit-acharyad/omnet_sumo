#include "myRSUApp.h"
#include "veins/base/modules/BaseLayer.h"
#include "SpeedMessage_m.h" // Make sure this header is correct and the message is defined
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <thread>
Define_Module(MyRSUApp);
void MyRSUApp::initialize(int stage) {
    BaseApplLayer::initialize(stage);
    if (stage == 0) {
        // Start socket thread
        socket_thread = std::thread(&MyRSUApp::runSocketClient, this);
        socket_thread.detach();
    }
}

void MyRSUApp::runSocketClient() {
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        EV_ERROR << "Socket creation failed\n";
        return;
    }

    struct sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9000); // same port as your external server

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        EV_ERROR << "Invalid address\n";
        return;
    }

    // Connect to server (blocking)
    if (connect(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        EV_ERROR << "Connection Failed\n";
        return;
    }

    connected = true;
    EV << "Connected to external server via raw socket\n";

    // Optionally implement recv loop here if you want to receive data
    // Otherwise, just keep socket open and send data from OMNeT++ thread
}

void MyRSUApp::sendDataOverSocket(const std::string& data) {
    if (!connected) {
        EV_WARN << "Socket not connected yet, dropping data\n";
        return;
    }

    ssize_t sent = ::send(sock_fd, data.c_str(), data.length(), 0);
    if (sent < 0) {
        EV_ERROR << "Socket send error\n";
    } else {
        EV << "Sent Data is "<<data.c_str()<<" of SIZE " << sent << " bytes over raw socket\n";
    }
}

void MyRSUApp::handleLowerMsg(cMessage *msg) {
    // Example: on receiving SpeedMessage, send data over socket
    SpeedMessage* speedMsg = dynamic_cast<SpeedMessage*>(msg);
    if (speedMsg) {
        std::stringstream ss;
        ss << speedMsg->getSenderId() << "," << speedMsg->getSpeed();
        sendDataOverSocket(ss.str());
    }
    delete msg;
}

void MyRSUApp::handleMessage(cMessage *msg) {
    // Example: on receiving SpeedMessage, send data over socket
       SpeedMessage* speedMsg = dynamic_cast<SpeedMessage*>(msg);
       if (speedMsg) {
           std::stringstream ss;
           ss << speedMsg->getSenderId() << "," << speedMsg->getSpeed();
           sendDataOverSocket(ss.str());
       }
       delete msg;
}

void MyRSUApp::finish() {
    if (sock_fd >= 0) {
        close(sock_fd);
    }
    BaseApplLayer::finish();
}

