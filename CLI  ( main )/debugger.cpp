#include "debugger.h"
#include <iostream>
#include <iomanip>
using namespace std;

void ResourceGraph::addProcess(PCB p) {
    processes.push_back(p);
}

void ResourceGraph::allocateResource(int pid, int res) {
    resourceOwner[res] = pid;
}

void ResourceGraph::requestResource(int pid, int res) {
    resourceWaits[pid].push_back(res);
}

bool ResourceGraph::hasDeadlock() {
    for (size_t i = 0; i < processes.size(); i++) {
        for (int wanted : processes[i].wantedResources) {
            if (resourceOwner.count(wanted)) {
                int holder = resourceOwner[wanted];
                for (size_t j = 0; j < processes.size(); j++) {
                    if (processes[j].pid == holder) {
                        for (int hwanted : processes[j].wantedResources) {
                            if (resourceOwner.count(hwanted)) {
                                int hholder = resourceOwner[hwanted];
                                for (size_t k = 0; k < processes.size(); k++) {
                                    if (processes[k].pid == hholder) {
                                        for (int kwanted : processes[k].wantedResources) {
                                            if (resourceOwner.count(kwanted) && resourceOwner[kwanted] == processes[i].pid) {
                                                return true;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

void ResourceGraph::showGraph() {
    cout << "\n  RESOURCE ALLOCATION GRAPH\n";
    cout << "  -------------------------\n";
    for (size_t i = 0; i < processes.size(); i++) {
        cout << "  P" << processes[i].pid << " (" << processes[i].name << ")\n";
        for (int h : processes[i].heldResources) {
            cout << "    holds -> [Res-" << h << "]\n";
        }
        for (int w : processes[i].wantedResources) {
            cout << "    wants -> [Res-" << w << "]\n";
        }
    }
    cout << "\n";
}

void ResourceGraph::showProcessTable() {
    cout << "\n  +-----+---------+----------+-----------+\n";
    cout << "  | PID |  STATE  |  HOLDS   |   WANTS   |\n";
    cout << "  +-----+---------+----------+-----------+\n";
    for (size_t i = 0; i < processes.size(); i++) {
        string holds = "";
        for (int h : processes[i].heldResources) holds += "R" + to_string(h) + " ";
        string wants = "";
        for (int w : processes[i].wantedResources) wants += "R" + to_string(w) + " ";
        cout << "  | P" << processes[i].pid << "  | " << setw(7) << processes[i].state;
        cout << " | " << setw(8) << holds << "| " << setw(9) << wants << "|\n";
    }
    cout << "  +-----+---------+----------+-----------+\n\n";
}

vector<PCB> ResourceGraph::getProcesses() {
    return processes;
}

PageTable::PageTable(int pages, int psize) {
    totalPages = pages;
    pageSize = psize;
}

void PageTable::allocate(int pid, int pages) {
    pidToPages[pid] += pages;
}

void PageTable::free(int pid) {
    pidToPages[pid] = 0;
}

int PageTable::getUsage(int pid) {
    if (pidToPages.count(pid)) return pidToPages[pid];
    return 0;
}

bool PageTable::hasLeak(int pid, int allowed) {
    return getUsage(pid) > allowed;
}

void PageTable::showMap() {
    int used = 0;
    for (auto& p : pidToPages) used += p.second;
    cout << "\n  MEMORY MAP (" << totalPages * pageSize << " MB total)\n\n  [";
    for (int i = 0; i < totalPages; i++) {
        if (i < used) cout << "##";
        else cout << "..";
    }
    cout << "]\n\n";
    for (auto& p : pidToPages) {
        cout << "  P" << p.first << " uses " << p.second * pageSize << " MB (" << p.second << " pages)\n";
    }
    cout << "\n";
}

int PageTable::getFreePages() {
    int used = 0;
    for (auto& p : pidToPages) used += p.second;
    return totalPages - used;
}

void ProcessQueue::addProcess(PCB p) {
    allProcesses.push_back(p);
    readyQueue.push(p);
}

void ProcessQueue::showQueue() {
    cout << "\n  READY QUEUE:\n";
    queue<PCB> temp = readyQueue;
    while (!temp.empty()) {
        PCB p = temp.front();
        temp.pop();
        cout << "  P" << p.pid << " (pri=" << p.priority << ", burst=" << p.burstTime << ")\n";
    }
    cout << "\n";
}

vector<PCB> ProcessQueue::getAll() {
    return allProcesses;
}

PCB ProcessQueue::getNext() {
    PCB p = readyQueue.front();
    readyQueue.pop();
    return p;
}

void ProcessTree::addProcess(PCB p, int parentPid) {
    Node n;
    n.proc = p;
    if (parentPid == 0) {
        roots.push_back(n);
    }
}

void ProcessTree::showTree() {
    cout << "\n  PROCESS TREE:\n";
    for (size_t i = 0; i < roots.size(); i++) {
        cout << "  " << roots[i].proc.name << " (PID " << roots[i].proc.pid << ")\n";
    }
    cout << "\n";
}

int ProcessTree::countZombies() {
    int count = 0;
    for (size_t i = 0; i < roots.size(); i++) {
        if (roots[i].proc.state == "ZOMBIE") count++;
    }
    return count;
}

