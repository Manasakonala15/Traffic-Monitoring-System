

# 🚦 Real-Time Traffic Monitoring System

A comprehensive simulation and monitoring tool designed to analyze traffic density, manage vehicle flow at junctions, and prevent gridlocks. This project features a robust **C++ Object-Oriented** backend for core algorithmic logic and a dynamic **HTML/CSS/JavaScript** frontend for real-time visualization.

## 🚀 Features

* **Dynamic Spatial Allocation:** Uses a strip-based algorithm to allocate road space based on precise vehicle dimensions and safety gaps.
* **Object-Oriented Architecture:** Implements a scalable vehicle hierarchy (Polymorphism & Inheritance) to easily introduce new vehicle types.
* **Real-Time Density Tracking:** Calculates junction load percentages dynamically, triggering automated status alerts (Clear, Moderate Rush, Heavy Rush, Gridlock Alert).
* **Interactive Dashboard:** A responsive web GUI that visualizes vehicle placement, tracks active vehicles, and provides a clear, color-coded density monitor.
* **Live Queue Management:** First-In-First-Out (FIFO) vehicle clearance simulating real-world traffic light resolutions.

## 💻 Tech Stack

* **Core Logic / Backend:** C++ (Standard Library)
* **Frontend / Visualization:** HTML5, CSS3, Vanilla JavaScript
* **Design Pattern:** Object-Oriented Programming (OOP)

## 🧠 Core Logic & Algorithms

### 1. The Vehicle Hierarchy (OOP)
The system uses a polymorphic C++ class structure to manage different vehicles. A base `Vehicle` class defines universal properties (name, length, width). Derived classes (e.g., `Hatchback`, `CityBus`, `Container`) inherit from the base class and initialize with specific, real-world dimensions. This makes the system highly extensible.

### 2. Strip-Based Space Allocation Algorithm
Instead of just counting the *number* of cars, the system accurately simulates physical road space:
* The total width of the road is divided into discrete **"strips"** (e.g., 0.25m resolution).
* When a new vehicle arrives, the algorithm calculates the required strips based on the vehicle's width plus a predefined **lateral safety gap**.
* It iterates through the junction's current strip array to find a contiguous block of strips that can accommodate the vehicle's length plus a **longitudinal safety gap**.
* If space is found, the vehicle's footprint is locked into those strips. If not, the system flags a block, preventing impossible overlaps.

### 3. Density Calculation
Traffic density is continuously calculated to monitor the health of the junction:
Density (%) = (Total Effective Area of Active Vehicles / Total Area of Junction) * 100
Depending on this percentage, the system state shifts, updating both the C++ console and the web UI with relevant warnings.

## 📂 Project Structure

* traffic_monitor.cpp - The C++ backend containing the OOP hierarchy, spatial algorithms, and CLI controller.
* index.html - The frontend web application featuring the interactive grid, road visualizer, and live stats.

## 🛠️ How to Run

### Running the C++ Backend (CLI)
1. Ensure you have a C++ compiler installed (like g++).
2. Compile the code:
   g++ traffic_monitor.cpp -o traffic_monitor

3. Execute the program:
   ./traffic_monitor

4. Follow the on-screen prompts to create a junction and add/remove vehicles.

### Running the GUI Dashboard
1. Simply open the index.html file in any modern web browser (Chrome, Firefox, Edge, Safari).
2. Click **"+ Add New Junction"** to configure your road parameters.
3. Click on the created junction card to enter the Controller View.
4. Use the categorized buttons to inject traffic and monitor the visualizer!

## 📸 Dashboard Preview
*(Note: You can add a screenshot of your beautiful web GUI here once uploaded to GitHub!)*

* **Junction Setup:** Define custom lengths and lane widths.
* **Live Road Visualizer:** Watch vehicles visually take up proportional space on the road.
* **Status Panel:** Instantly know if the road is clear (Green) or gridlocked (Red).
