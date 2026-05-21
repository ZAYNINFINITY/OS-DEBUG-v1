#ifndef CASELAB_H
#define CASELAB_H

#include <string>
using namespace std;

class CaseLab {
public:
    static void showMenu();
    static void runFCFS();
    static void runSJF();
    static void runRoundRobin();
    static void runPriority();
    static void compareAll();
};

#endif

