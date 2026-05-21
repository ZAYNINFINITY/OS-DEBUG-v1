#include "inventory.h"
#include <iostream>
using namespace std;

Inventory::Inventory() {
    toolkit[0] = {INSPECTOR, "Process Inspector", true};
    toolkit[1] = {LOGS, "System Logs", true};
    toolkit[2] = {DEADLOCK, "Deadlock Detector", false};
    toolkit[3] = {MEMORY, "Memory Scanner", false};
    toolkit[4] = {PRIORITY, "Priority Reassigner", false};
    toolkit[5] = {COMPACTOR, "Memory Compactor", false};
    toolkit[6] = {TERMINATOR, "Process Terminator", false};
    toolkit[7] = {RR, "Round Robin Scheduler", false};
}

void Inventory::reset() {
    for (int i = 0; i < 8; i++) {
        toolkit[i].unlocked = (toolkit[i].id == INSPECTOR || toolkit[i].id == LOGS);
    }
}

bool Inventory::has(int id) {
    for (int i = 0; i < 8; i++) {
        if (toolkit[i].id == id) return toolkit[i].unlocked;
    }
    return false;
}

void Inventory::unlock(int id) {
    for (int i = 0; i < 8; i++) {
        if (toolkit[i].id == id && !toolkit[i].unlocked) {
            toolkit[i].unlocked = true;
            cout << "\n  >> TOOL UNLOCKED: " << toolkit[i].name << "\n";
            return;
        }
    }
}

void Inventory::unlockQuiet(int id) {
    for (int i = 0; i < 8; i++) {
        if (toolkit[i].id == id) {
            toolkit[i].unlocked = true;
            return;
        }
    }
}

bool Inventory::isKnown(int id) {
    for (int i = 0; i < 8; i++) {
        if (toolkit[i].id == id) return true;
    }
    return false;
}

string Inventory::getName(int id) {
    for (int i = 0; i < 8; i++) {
        if (toolkit[i].id == id) return toolkit[i].name;
    }
    return "Unknown";
}

void Inventory::show() {
    cout << "\n  +==================================================+\n";
    cout << "  |  YOUR TOOLKIT                                    |\n";
    cout << "  +--------------------------------------------------+\n";
    cout << "  |  ID   NAME                   STATUS              |\n";
    cout << "  +--------------------------------------------------+\n";
    for (int i = 0; i < 8; i++) {
        string status = toolkit[i].unlocked ? "READY" : "LOCKED";
        cout << "  |  [" << toolkit[i].id << "]  ";
        cout << toolkit[i].name;
        int pad = 22 - (int)toolkit[i].name.size();
        for (int p = 0; p < pad; p++) cout << " ";
        cout << "[" << status << "]|\n";
    }
    cout << "  +==================================================+\n\n";
}

