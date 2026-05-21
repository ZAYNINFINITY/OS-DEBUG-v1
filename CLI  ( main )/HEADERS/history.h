#ifndef HISTORY_H
#define HISTORY_H

#include <string>
using namespace std;

class History {
public:
    static void appendRun(string username, string difficulty, int score, int stability);
    static void showRuns();
};

#endif

