#include "Orchestrator.h"
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <jsoncpp/json/json.h>
#include "myCarApp.h"

// Register the module with OMNeT++
Define_Module(Orchestrator);

void Orchestrator::initialize() {
    // Get parameters from omnetpp.ini
    port = par("port");

    // Create a self-message for periodically checking the socket
    socket_check_event = new cMessage("socketCheck");

    EV << "🚀 Orchestrator initializing on port " << port << std::endl;

    // The socket server must run in a separate thread to avoid blocking
    // the main OMNeT++ simulation kernel with the blocking `accept()` call.
    server_thread = std::thread(&Orchestrator::startTcpServer, this);
    server_thread.detach(); // Allow the thread to run independently

    // Schedule the first event to start checking for client data
    scheduleAt(simTime() + 1.0, socket_check_event);
}

void Orchestrator::handleMessage(cMessage *msg) {
    // We only expect our self-message.
    if (msg == socket_check_event) {
        EV << "🔍 Checking for client data... client_connected=" << client_connected << ", client_socket=" << client_socket << endl;

        // Only process data if a client has successfully connected
        if (client_connected) {
            handleClientData();
            // Example of two-way communication:
            // Periodically send a command back to the ns-3 client.
            if (simTime() > 5.0 && fmod(simTime().dbl(), 10.0) < 0.5) {
                 sendCommandToClient("TYPE:NFV_COMMAND|DATA:Migrate_VNF_to_Node_B\n");
            }
        } else {
            EV << "⏳ Client not yet connected, waiting..." << endl;
        }
        // Reschedule the check for the next interval
        scheduleAt(simTime() + 0.5, socket_check_event);
    } else {
        // Should not happen in this simple model
        EV_WARN << "Received unexpected message: " << msg->getName() << std::endl;
        delete msg;
    }
}

void Orchestrator::startTcpServer() {
    struct sockaddr_in server_addr;
    int opt = 1;
    int addrlen = sizeof(server_addr);

    // 1. Create the server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        EV_ERROR << "FATAL: Socket creation failed: " << strerror(errno) << std::endl;
        return;
    }

    // 2. Set socket options to allow reusing the address
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        EV_ERROR << "FATAL: setsockopt failed: " << strerror(errno) << std::endl;
        close(server_fd); // Ensure server_fd is closed on error
        return;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // Listen on any network interface
    server_addr.sin_port = htons(port);

    // 3. Bind the socket to the specified port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        EV_ERROR << "FATAL: Bind failed on port " << port << ": " << strerror(errno) << std::endl;
        close(server_fd); // Ensure server_fd is closed on error
        return;
    }

    // 4. Listen for incoming connections (allow a small backlog)
    if (listen(server_fd, 5) < 0) { // Increased backlog queue for more robustness
        EV_ERROR << "FATAL: Listen failed: " << strerror(errno) << std::endl;
        close(server_fd); // Ensure server_fd is closed on error
        return;
    }

    EV << "👂 Server listening... Waiting for clients to connect." << std::endl;

    // --- MODIFICATION START ---
    // Loop indefinitely to accept multiple client connections sequentially.
    // This thread will block on `accept()` until a client connects.
    // When a client disconnects, `handleClientData` will close `client_socket`
    // and set `client_connected = false`, allowing this loop to accept a new one.
    while (true) {
        // Only attempt to accept a new connection if no client is currently connected.
        // This prevents overwriting an active client_socket if the main thread is still processing it.
        if (!client_connected) {
            EV << "DEBUG: Server thread is ready to accept a new connection..." << std::endl;
            // `accept` is a blocking call. This thread will pause here until a client attempts to connect.
            if ((client_socket = accept(server_fd, (struct sockaddr *)&server_addr, (socklen_t*)&addrlen)) < 0) {
                // If accept fails, log the error and try again after a short delay.
                // This can happen if the server_fd is closed unexpectedly or other transient network issues.
                EV_ERROR << "FATAL: Accept failed: " << strerror(errno) << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Sleep to prevent busy-looping on error
                continue; // Go back to the start of the while loop to try accepting again
            }

            // A client has successfully connected.
            EV << "✅ Client connected! Client socket FD: " << client_socket << std::endl;
            client_connected = true; // Mark that a client is now connected
        } else {
            // If a client is already connected, this thread will just wait a bit
            // before checking `client_connected` again. This is to avoid a busy-wait
            // while the main OMNeT++ thread is handling the current client.
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    // --- MODIFICATION END ---
    // Note: This loop is infinite, so the code below it will not be reached unless the loop is broken.
    // If you need to clean up server_fd on simulation end, you'd need a way to signal this thread to stop.
    // For now, it will run until the OMNeT++ process itself terminates.
}

void Orchestrator::handleClientData() {
    char buffer[1024];
    ssize_t bytesRead = recv(client_socket, buffer, sizeof(buffer) - 1, 0);


    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        std::string msg(buffer);

        // Print received data
        EV << "📥 DATA RECEIVED from ns-3 client!" << endl;
        EV << "📊 Data content: " << buffer << endl;
        std::cout << "📥 DATA RECEIVED from ns-3 client!" << std::endl;
        std::cout << "📊 Data: " << buffer << std::endl;

        // === ADD THIS BLOCK ===
        Json::Value root;

        Json::CharReaderBuilder reader;
        std::string errs;
        std::istringstream s(msg);
        if (Json::parseFromStream(reader, s, &root, &errs)) {
            if (root.isMember("request") && root["request"].asString() == "position" && root.isMember("node")) {

                int nodeId = root["node"].asInt();
//                cModule *systemModule = getSystemModule();
//                if (systemModule) {
//                    std::cout << "System module children:" << endl;
//                    for (cModule::SubmoduleIterator it(systemModule); !it.end(); ++it) {
//                        std::cout << "  - " << (*it)->getFullPath() << endl; // Option 1: Dereference with *
//                        // OR
//                         EV << "  - " << (*it)->getFullPath() << endl; // Option 2: Use arrow operator
//                    }
//                }
                // Build dynamic path: e.g., "Circle.node[1]"
                std::string path = "Circle.node[" + std::to_string(nodeId) + "]";

                // Get pointer to the module
                cModule* vehicleMod = getModuleByPath(path.c_str());
                std::cout<<"Vehicle mode"<<vehicleMod<<endl;
                   if (vehicleMod) {
                       cModule* applModule = vehicleMod->getSubmodule("appl"); // <--- **VERIFY THIS NAME!**
                       if(applModule){
                           MyCarApp *carApp = dynamic_cast<MyCarApp*>(applModule);
                           if(carApp){
                               Coord pos = carApp->getCurrentPosition();

                                                     Json::Value resp;
                                                     resp["node"] = nodeId;
                                                     resp["x"] = pos.x;
                                                     resp["y"] = pos.y;
                                                     resp["z"] = pos.z;

                                                     Json::StreamWriterBuilder builder;
                                                     std::string response = Json::writeString(builder, resp) + "\n";
                                                     ::send(client_socket, response.c_str(), response.length(), 0);


                                                     EV << "📤 Sent position to ns-3: " << response << endl;
                                                     std::cout << "📤 Sent position to ns-3: " << response << std::endl;
                           }else{
                               EV<<"Could not dynamic cast car app"<<endl;
                               std::cout<<"Could not dynamic cast car app"<<endl;
                           }

                       }else{
                           EV<<"Got no application layer"<<endl;
                           std::cout<<"Got no application layer"<<endl;
                       }

                   }
                   else{
                       EV<<"No vehicle found Error"<<endl;
                   }

            }
        }
        // === END BLOCK ===

        // Optionally, send ACK for other data
        std::string ack = "ACK: Data received\n";
        ::send(client_socket, ack.c_str(), ack.length(), 0);

    } else if (bytesRead == 0) {
        EV << "ℹ️ ns-3 client disconnected." << endl;
        std::cout << "ℹ️ ns-3 client disconnected." << std::endl;
        close(client_socket);
        client_socket = -1;
        client_connected = false;
    } else {
        // bytesRead < 0 - check errno
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            EV << "❌ recv() error: " << strerror(errno) << endl;
            std::cout << "❌ recv() error: " << strerror(errno) << std::endl;
        } else {
            // No data available right now (normal for non-blocking)
            EV << "🔍 No data available (EAGAIN/EWOULDBLOCK)" << endl;
        }
    }
}

void Orchestrator::sendCommandToClient(const std::string& command) {
    if (!client_connected) return;

    EV << "📤 Sending to ns-3: " << command;
    // Use ::send to call the global POSIX socket function, not the OMNeT++ one.
    ::send(client_socket, command.c_str(), command.length(), 0);
}

void Orchestrator::finish() {
    EV << "🏁 Simulation finished. Closing sockets." << std::endl;

    // Clean up the scheduled event
    cancelAndDelete(socket_check_event);

    // Close sockets
    if (client_socket >= 0) {
        close(client_socket);
    }
    if (server_fd >= 0) {
        close(server_fd);
    }
}
