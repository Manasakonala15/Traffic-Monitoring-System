#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <map>

using namespace std;

// --- CONSTANTS ---
const double GAP = 0.2;         // 0.2m longitudinal gap
const double LAT_GAP = 0.4;     // 0.4m lateral gap
const double STRIP_WIDTH = 0.25; // 25cm resolution

// ==========================================
// 1. VEHICLE HIERARCHY
// ==========================================
class Vehicle {
protected:
    string name;
    double length, width;
public:
    Vehicle(string n, double l, double w) : name(n), length(l), width(w) {}
    double getL() const { return length; }
    double getW() const { return width; }
    string getName() const { return name; }
    virtual ~Vehicle() {}
};

// Types
class StdCycle : public Vehicle { public: StdCycle() : Vehicle("Cycle", 1.6, 0.6) {} };
class Scooter : public Vehicle  { public: Scooter()  : Vehicle("Scooter", 1.8, 0.7) {} };
class Bike : public Vehicle     { public: Bike()     : Vehicle("Bike", 2.0, 0.8) {} };
class Auto : public Vehicle      { public: Auto()      : Vehicle("Auto", 2.6, 1.3) {} };
class GoodsAuto : public Vehicle { public: GoodsAuto() : Vehicle("Goods Auto", 3.0, 1.4) {} };
class Hatchback : public Vehicle { public: Hatchback() : Vehicle("Hatchback", 3.7, 1.7) {} };
class Sedan : public Vehicle     { public: Sedan()     : Vehicle("Sedan", 4.5, 1.8) {} };
class SUV : public Vehicle       { public: SUV()       : Vehicle("SUV", 4.8, 2.0) {} };
class MiniBus : public Vehicle { public: MiniBus() : Vehicle("Mini Bus", 6.0, 2.2) {} };
class CityBus : public Vehicle { public: CityBus() : Vehicle("City Bus", 12.0, 2.55) {} };
class MiniTruck : public Vehicle { public: MiniTruck() : Vehicle("Mini Truck", 3.8, 1.5) {} };
class MedTruck : public Vehicle  { public: MedTruck()  : Vehicle("Truck", 6.0, 2.1) {} };
class Lorry : public Vehicle     { public: Lorry()     : Vehicle("Lorry", 10.0, 2.5) {} };
class Tanker : public Vehicle    { public: Tanker()    : Vehicle("Tanker", 11.0, 2.5) {} };
class Container : public Vehicle { public: Container() : Vehicle("Container", 16.0, 2.6) {} };

struct QueuedVehicle {
    Vehicle* v;
    int xStrip;  
    double pos;  
    double effectiveArea; 
};

// ==========================================
// 2. JUNCTION CLASS (Logic Core)
// ==========================================
class Junction {
private:
    string name;
    double length;
    double width;
    double totalArea;
    double currentLoad;
    vector<double> strips; 
    vector<QueuedVehicle> vehicles;

public:
    Junction(string n, double len, double wid) {
        name = n;
        length = len;
        width = wid;
        totalArea = length * width;
        currentLoad = 0;
        
        int numStrips = (int)floor(width / STRIP_WIDTH);
        strips.assign(numStrips, 0.0);
    }

    string getName() const { return name; }

    double getDensity() {
        if (totalArea == 0) return 0;
        return (currentLoad / totalArea) * 100.0;
    }

    void printOneLineStatus() {
        double d = getDensity();
        string status;
        if(d > 90) status = "[GRIDLOCK ALERT]";
        else if(d > 70) status = "[HEAVY RUSH]";
        else if(d > 50) status = "[MODERATE RUSH]";
        else status = "[TRAFFIC CLEAR]";
        
        cout << ">> Current Density: " << fixed << setprecision(1) << d << "% " << status << endl;
    }

    void addVehicle(Vehicle* v) {
        int reqStrips = (int)ceil((v->getW() + LAT_GAP) / STRIP_WIDTH);
        int bestStart = -1;
        double minMax = 99999;
        double effectiveArea = (v->getL() + GAP) * (v->getW() + LAT_GAP);

        for(int i=0; i <= (int)strips.size() - reqStrips; i++) {
            double currMax = 0;
            for(int k=0; k<reqStrips; k++) currMax = max(currMax, strips[i+k]);
            
            if(currMax < minMax) { 
                minMax = currMax; 
                bestStart = i; 
            }
        }

        if(bestStart != -1 && (minMax + v->getL() + GAP) <= length) {
            double startPos = minMax;
            
            for(int k=0; k<reqStrips; k++) strips[bestStart+k] = startPos + v->getL() + GAP;
            
            QueuedVehicle qv = { v, bestStart, startPos, effectiveArea };
            vehicles.push_back(qv);
            currentLoad += effectiveArea;
            
            cout << "\n>> SUCCESS: " << v->getName() << " added." << endl;
            // Removed duplicate status print here
        } else {
            cout << "\n>> BLOCKED! Junction cannot fit another " << v->getName() << ". Clear vehicles!" << endl;
            // Removed duplicate status print here
            delete v; 
        }
    }

    void removeVehicle() {
        if(vehicles.empty()) { cout << "\n>> Road is empty!" << endl; return; }
        
        QueuedVehicle vToRemove = vehicles.front();
        vehicles.erase(vehicles.begin());
        currentLoad = max(0.0, currentLoad - vToRemove.effectiveArea);
        
        cout << "\n>> SIGNAL GREEN: " << vToRemove.v->getName() << " exited." << endl;
        delete vToRemove.v;

        fill(strips.begin(), strips.end(), 0.0);
        
        for(auto& qv : vehicles) {
            int reqStrips = (int)ceil((qv.v->getW() + LAT_GAP) / STRIP_WIDTH);
            double currMax = 0;
            for(int k=0; k<reqStrips; k++) currMax = max(currMax, strips[qv.xStrip+k]);
            
            qv.pos = currMax; 
            for(int k=0; k<reqStrips; k++) strips[qv.xStrip+k] = qv.pos + qv.v->getL() + GAP;
        }
        // Removed duplicate status print here
    }

    void generateReport() {
        cout << "\n========================================" << endl;
        cout << " DETAILED REPORT: " << name << endl;
        cout << "========================================" << endl;
        cout << " Dimensions:   " << width << "m Wide | " << length << "m Long" << endl;
        cout << " Safety Gaps:  " << LAT_GAP << "m Lateral | " << GAP << "m Longitudinal" << endl;
        cout << " Vehicle Count:" << vehicles.size() << endl;
        
        double d = getDensity();
        cout << " DENSITY:      " << fixed << setprecision(1) << d << "%" << endl;
        cout << " STATUS:       ";
        
        if(d > 90) cout << "[GRIDLOCK ALERT]" << endl;
        else if(d > 70) cout << "[HEAVY RUSH]" << endl;
        else if(d > 50) cout << "[MODERATE RUSH]" << endl;
        else cout << "[TRAFFIC CLEAR]" << endl;
        cout << "========================================" << endl;
    }

    void listVehiclesSummary() {
        cout << "\n--- VEHICLE SUMMARY ---" << endl;
        if(vehicles.empty()) {
            cout << "(No vehicles on road)" << endl;
        } else {
            map<string, int> counts;
            for(size_t i = 0; i < vehicles.size(); i++) {
                counts[vehicles[i].v->getName()]++;
            }
            for(auto const& pair : counts) {
                cout << " " << pair.first << ": " << pair.second << endl;
            }
        }
        cout << "-----------------------" << endl;
    }
    
    ~Junction() { for(auto& qv : vehicles) delete qv.v; }
};

// ==========================================
// 3. MAIN
// ==========================================
int main() {
    vector<Junction*> cityMap;
    int mainChoice;

    do {
        cout << "\n=== JUNCTION TRAFFIC MONITORING SYSTEM ===" << endl;
        cout << "1. Create New Junction" << endl;
        cout << "2. Select Junction to Monitor" << endl;
        cout << "3. Exit Application" << endl;
        cout << "Choice: "; cin >> mainChoice;

        if (mainChoice == 1) {
            string name; double l, w;
            cout << "\n[SETUP] Enter Junction Name: "; cin.ignore(); getline(cin, name);
            cout << "Length (meters): "; cin >> l;
            cout << "Width (meters): "; cin >> w;
            cityMap.push_back(new Junction(name, l, w));
            cout << ">> Junction '" << name << "' created." << endl;
        }
        else if (mainChoice == 2) {
            if(cityMap.empty()) { cout << ">> No junctions found. Create one first!" << endl; continue; }
            
            cout << "\nSelect Junction:" << endl;
            for(size_t i=0; i<cityMap.size(); i++) {
                cout << i+1 << ". " << cityMap[i]->getName() << endl;
            }
            int jIdx; cout << "Input ID: "; cin >> jIdx;

            if(jIdx > 0 && jIdx <= (int)cityMap.size()) {
                Junction* currentJ = cityMap[jIdx-1];
                int subChoice;
                do {
                    cout << endl;
                    currentJ->printOneLineStatus(); 
                    
                    cout << "\n[CONTROLLER: " << currentJ->getName() << "]" << endl;
                    cout << "1. Add Vehicle" << endl;
                    cout << "2. Pass Vehicle (Green Light)" << endl;
                    cout << "3. List Vehicles (Summary)" << endl;
                    cout << "4. View Full Report" << endl;
                    cout << "5. Back" << endl;
                    cout << "Action: "; cin >> subChoice;

                    if(subChoice == 1) {
                        int cat, type;
                        cout << "\n[CATEGORY] 1.Cycle 2.Bike 3.Auto 4.Car 5.Bus 6.Truck 7.Heavy: ";
                        cin >> cat;
                        
                        Vehicle* v = nullptr;
                        if (cat == 1) v = new StdCycle();
                        else if (cat == 2) {
                            cout << "  Type: 1.Scooter 2.Bike? "; cin >> type;
                            if(type==1) v = new Scooter(); else v = new Bike();
                        }
                        else if (cat == 3) {
                            cout << "  Type: 1.Passenger 2.Goods? "; cin >> type;
                            if(type==1) v = new Auto(); else v = new GoodsAuto();
                        }
                        else if (cat == 4) {
                            cout << "  Type: 1.Hatchback 2.Sedan 3.SUV? "; cin >> type;
                            if(type==1) v = new Hatchback(); else if(type==2) v = new Sedan(); else v = new SUV();
                        }
                        else if (cat == 5) {
                            cout << "  Type: 1.Mini 2.City? "; cin >> type;
                            if(type==1) v = new MiniBus(); else v = new CityBus();
                        }
                        else if (cat == 6) {
                            cout << "  Type: 1.Mini 2.Medium? "; cin >> type;
                            if(type==1) v = new MiniTruck(); else v = new MedTruck();
                        }
                        else if (cat == 7) {
                            cout << "  Type: 1.Lorry 2.Tanker 3.Container? "; cin >> type;
                            if(type==1) v = new Lorry(); else if(type==2) v = new Tanker(); else v = new Container();
                        }

                        if(v) currentJ->addVehicle(v);
                    }
                    else if(subChoice == 2) {
                        currentJ->removeVehicle();
                    }
                    else if(subChoice == 3) {
                        currentJ->listVehiclesSummary();
                    }
                    else if(subChoice == 4) {
                        currentJ->generateReport();
                    }

                } while(subChoice != 5);
            }
        }

    } while (mainChoice != 3);

    for(auto j : cityMap) delete j;
    return 0;
}