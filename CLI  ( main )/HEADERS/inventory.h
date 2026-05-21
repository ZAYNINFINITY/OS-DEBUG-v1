#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>
using namespace std;

#define INSPECTOR 1
#define LOGS 2
#define DEADLOCK 3
#define MEMORY 4
#define PRIORITY 5
#define COMPACTOR 6
#define TERMINATOR 7
#define RR 8

class Inventory {
private:
    struct Tool {
        int id;
        string name;
        bool unlocked;
    };
    Tool toolkit[8];

public:
    Inventory();
    void reset();
    bool has(int id);
    void unlock(int id);
    void unlockQuiet(int id);
    bool isKnown(int id);
    string getName(int id);
    void show();
};

#endif

