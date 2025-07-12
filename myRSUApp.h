
#ifndef __SUMOVEINS_MYRSUAPP_H_
#define __SUMOVEINS_MYRSUAPP_H_

#include <omnetpp.h>
#include "veins/base/modules/BaseApplLayer.h"
#include <sys/socket.h>
#include <thread>
using namespace omnetpp;
using namespace veins;
class MyRSUApp : public BaseApplLayer {
  private:
    int sock_fd = -1;
    std::thread socket_thread;
    bool connected = false;

    void runSocketClient(); // thread function

  public:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void handleLowerMsg(cMessage *msg) override;
    virtual void finish() override;

    void sendDataOverSocket(const std::string& data);
};

#endif
