#include "history.h"
#include "display.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <vector>
using namespace std;

struct Run {
    string username;
    string date;
    string difficulty;
    int score;
    int stability;
    int wrongs;
};

static string timestamp() {
    time_t t = time(NULL);
    tm* lt = localtime(&t);
    ostringstream s;
    s << (1900 + lt->tm_year) << "-"
      << setw(2) << setfill('0') << (1 + lt->tm_mon) << "-"
      << setw(2) << setfill('0') << lt->tm_mday << " "
      << setw(2) << setfill('0') << lt->tm_hour << ":"
      << setw(2) << setfill('0') << lt->tm_min;
    return s.str();
}

static vector<Run> loadRuns() {
    vector<Run> runs;
    ifstream f("history.txt");
    if (!f.is_open()) return runs;

    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        Run r;
        getline(ss, r.username, '|');
        getline(ss, r.date, '|');
        getline(ss, r.difficulty, '|');
        ss >> r.score;
        ss.ignore();
        ss >> r.stability;
        ss.ignore();
        ss >> r.wrongs;
        runs.push_back(r);
    }
    return runs;
}

void History::appendRun(string username, string difficulty, int score, int stability) {
    ofstream f("history.txt", ios::app);
    if (!f.is_open()) return;
    f << username << "|" << timestamp() << "|" << difficulty << "|"
      << score << "|" << stability << "|0\n";
}

void History::showRuns() {
    vector<Run> runs = loadRuns();
    int w = 92;

    cout << "\n";
    Display::boxTop(w);
    Display::boxLine("  RUN HISTORY", w);
    Display::boxSeparator(w);

    if (runs.empty()) {
        Display::boxLine("  No runs yet.", w);
    } else {
        Display::boxLine("  USER               DATE              DIFFICULTY   SCORE   STAB", w);
        Display::boxSeparator(w);

        int start = (int)runs.size() > 10 ? (int)runs.size() - 10 : 0;
        for (int i = start; i < (int)runs.size(); i++) {
            ostringstream row;
            row << "  " << left << setw(18) << runs[i].username
                << setw(18) << runs[i].date
                << setw(13) << runs[i].difficulty
                << setw(8) << runs[i].score
                << runs[i].stability;
            Display::boxLine(row.str(), w);
        }
    }

    Display::boxBottom(w);
    cout << "\n";
}

