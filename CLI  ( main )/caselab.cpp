#include "caselab.h"
#include "algorithms.h"
#include "display.h"
#include <iostream>
using namespace std;

void CaseLab::showMenu() {
    cout << "\n  ========================================\n";
    cout << "  ALGORITHM LAB\n";
    cout << "  ========================================\n";
    cout << "  [1] FCFS - First Come First Served\n";
    cout << "  [2] SJF - Shortest Job First\n";
    cout << "  [3] Round Robin (quantum=4)\n";
    cout << "  [4] Priority Scheduling\n";
    cout << "  [5] Compare All Algorithms\n";
    cout << "  [6] Back to Main Menu\n";
    cout << "  ========================================\n\n";
}

void CaseLab::runFCFS() {
    Algorithms::fcfs();
    Display::waitEnter();
}

void CaseLab::runSJF() {
    Algorithms::sjf();
    Display::waitEnter();
}

void CaseLab::runRoundRobin() {
    Algorithms::roundRobin(4);
    Display::waitEnter();
}

void CaseLab::runPriority() {
    Algorithms::priority();
    Display::waitEnter();
}

void CaseLab::compareAll() {
    Algorithms::fcfs();
    Algorithms::sjf();
    Algorithms::roundRobin(4);
    Algorithms::priority();

    int w = 56;
    Display::boxTop(w);
    Display::boxLine("  SUMMARY", w);
    Display::boxSeparator(w);
    Display::boxLine("  ALGORITHM     FAIR   STARVES   PREEMPTS", w);
    Display::boxSeparator(w);
    Display::boxLine("  FCFS          No     No        No", w);
    Display::boxLine("  SJF           No     Yes       No", w);
    Display::boxLine("  Round Robin   Yes    No        Yes", w);
    Display::boxLine("  Priority      No     Yes       No", w);
    Display::boxSeparator(w);
    Display::boxLine("  Best for mixed workloads: Round Robin", w);
    Display::boxBottom(w);
    Display::waitEnter();
}

