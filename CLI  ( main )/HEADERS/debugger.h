#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <string>
using namespace std;

struct PCB {
    int pid;
    string name;
    string state;
    int priority;
    int burstTime;
    int arrivalTime;
    int waitTime;
    vector<int> heldResources;
    vector<int> wantedResources;
};

class ResourceGraph {
private:
    vector<PCB> processes;
    map<int, int> resourceOwner;
    map<int, vector<int>> resourceWaits;
public:
    void addProcess(PCB p);
    void allocateResource(int pid, int res);
    void requestResource(int pid, int res);
    bool hasDeadlock();
    void showGraph();
    void showProcessTable();
    vector<PCB> getProcesses();
};

class PageTable {
private:
    map<int, int> pidToPages;
    int totalPages;
    int pageSize;
public:
    PageTable(int pages, int psize);
    void allocate(int pid, int pages);
    void free(int pid);
    int getUsage(int pid);
    bool hasLeak(int pid, int allowed);
    void showMap();
    int getFreePages();
};

class ProcessQueue {
private:
    queue<PCB> readyQueue;
    vector<PCB> allProcesses;
public:
    void addProcess(PCB p);
    void showQueue();
    vector<PCB> getAll();
    PCB getNext();
};

class ProcessTree {
private:
    struct Node {
        PCB proc;
        vector<Node> children;
    };
    vector<Node> roots;
public:
    void addProcess(PCB p, int parentPid);
    void showTree();
    int countZombies();
};

#endif

