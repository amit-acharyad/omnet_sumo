# SUMO-OMNeT++ Integrated V2X Simulation

This project demonstrates the integration of **SUMO (Simulation of Urban Mobility)** with **OMNeT++** via the **Veins** framework. It allows the simulation of connected vehicles (V2X communication) where vehicle mobility is managed by SUMO, and network simulation and application logic are handled within OMNeT++.

## 🚗 Project Overview

- Vehicles are spawned and controlled in **SUMO**.
- Real-time vehicle data is imported into **OMNeT++** using **TraCI (Traffic Control Interface)**.
- Custom vehicle and RSU (Road Side Unit) modules are created (`MyCarApp` and `MyRSUApp`) to process V2X messages.
- Basic information such as **vehicle IDs**, **positions**, and **timestamps** are retrieved and displayed in OMNeT++ for each SUMO vehicle.

---

## 🧩 Technologies Used

| Tool        | Version   | Purpose                        |
|-------------|-----------|--------------------------------|
| **OMNeT++** | 6.1+      | Network simulation             |
| **SUMO**    | 1.18+     | Traffic mobility simulation    |
| **Veins**   | 5.3.1     | SUMO-OMNeT++ integration        |
| **INET** *(optional)* | – | For advanced protocol support |
| **TraCI**   | –         | Communication bridge           |

---

