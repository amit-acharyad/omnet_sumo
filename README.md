# SUMO-OMNeT++ Integrated V2X Simulation

This project demonstrates the integration of **SUMO (Simulation of Urban Mobility)** with **OMNeT++** via the **Veins** framework. It enables simulation of connected vehicles (V2X communication), where vehicle mobility is driven by SUMO and application-level logic is handled within OMNeT++.

---

## 🚗 Project Overview

- **Vehicles are controlled in SUMO**, and their mobility is synchronized with OMNeT++ using the **TraCI** interface.
- Custom application modules are used:
  - `MyCarApp`: runs on vehicles and sends mobility data.
  - `MyRSUApp`: runs on RSUs (Road Side Units) and receives vehicle messages.
- RSUs **forward received data over a TCP socket** to an external system.
- The data format sent over the socket is:

  ```
  node[0] 2.4
  node[0] 3.4
  node[1] 4.1
  ...
  ```

- This can be tested using a simple TCP listener like `netcat`:
  
  ```bash
  nc -l 9000
  ```

  Once running, you will see incoming vehicle data printed live in the terminal.

---

## 📡 Communication Flow

```plaintext
SUMO (Vehicle mobility)
      ↓
OMNeT++ (Veins)
      ↓
Vehicle (MyCarApp)
      ↓  wireless
RSU (MyRSUApp)
      ↓  TCP socket
External system (e.g., netcat listener)
```

---

## ✅ Features

- 🔄 Real-time SUMO-OMNeT++ mobility sync via TraCI.
- 📡 Wireless message passing from vehicles to RSUs.
- 🔌 TCP socket-based external forwarding of simulation data.
- 💡 Simple and modular design — easy to extend or integrate with real-world interfaces.

---

## 🧩 Technologies Used

| Tool             | Version   | Purpose                           |
|------------------|-----------|-----------------------------------|
| **OMNeT++**      | 5.6.2     | Network simulation engine         |
| **Veins**        | 5.3.1     | SUMO-OMNeT++ integration          |
| **SUMO**         | 1.18+     | Vehicular mobility simulation     |
| **TraCI**        | –         | Real-time mobility control        |
| **POSIX Sockets**| –         | Socket-based external output      |
| **Netcat (`nc`)**| –         | TCP data listener (for testing)   |

> **Note:** INET is **not used** in this simulation.

---

## 🛠 How to Run

1. **Clone this repository**.
2. **Import the project into your OMNeT++ workspace**.
3. If **Veins has not been built**, you will need to build it first.
4. **Run the simulation** from within OMNeT++.
5. To observe RSU socket output, start a listener in a terminal:

   ```bash
   nc -l 9000
   ```

6. You'll start seeing data like:

   ```
   node[0] 2.4
   node[0] 3.4
   node[1] 1.8
   ...
   ```

---

