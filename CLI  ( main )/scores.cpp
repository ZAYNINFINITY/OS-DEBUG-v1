#include "scores.h"
#include "display.h"
#include "gamestate.h"
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

extern GameState* g_state;

namespace scores {

struct Entry {
    string username;
    string name;
    int    score;
    string date;
};

static string today() {
    time_t t = time(NULL);
    tm* lt   = localtime(&t);
    ostringstream s;
    s << (1900 + lt->tm_year) << "-"
      << setw(2) << setfill('0') << (1 + lt->tm_mon) << "-"
      << setw(2) << setfill('0') << lt->tm_mday;
    return s.str();
}

static vector<Entry> load() {
    vector<Entry> v;
    ifstream f("scores.txt");
    if (!f.is_open()) return v;

    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        // Format: username|score|date
        size_t p1 = line.find('|');
        size_t p2 = (p1 == string::npos) ? string::npos : line.find('|', p1 + 1);
        if (p1 == string::npos || p2 == string::npos) continue;

        Entry e;
        e.username = line.substr(0, p1);
        e.name     = e.username;
        e.score    = stoi(line.substr(p1 + 1, p2 - p1 - 1));
        e.date     = line.substr(p2 + 1);
        v.push_back(e);
    }
    return v;
}

static void save(vector<Entry>& v) {
    ofstream f("scores.txt", ios::trunc);
    for (const auto& e : v)
        f << e.username << "|" << e.score << "|" << e.date << "\n";
}

static bool higher_score(const Entry& a, const Entry& b) {
    return a.score > b.score;
}

void maybe_record(int total_score) {
    vector<Entry> v = load();
    sort(v.begin(), v.end(), higher_score);

    bool qualifies = ((int)v.size() < 5) ||
                     (!v.empty() && total_score > v.back().score);

    if (!qualifies) {
        show();
        return;
    }

    cout << "\n";
    Display::divider();
    cout << "  *** NEW HIGH SCORE : " << total_score << " ***\n";
    Display::divider();

    Entry e;
    e.username = g_state->getUser()->getName();
    e.name     = e.username;
    e.score    = total_score;
    e.date     = today();
    v.push_back(e);

    sort(v.begin(), v.end(), higher_score);
    if ((int)v.size() > 5) v.resize(5);
    save(v);
    show();
}

void show() {
    vector<Entry> v = load();
    sort(v.begin(), v.end(), higher_score);

    int w = 56;
    cout << "\n";
    Display::boxTop(w);
    Display::boxLine("  LEADERBOARD  --  TOP 5", w);
    Display::boxSeparator(w);

    if (v.empty()) {
        Display::boxLine("  No scores yet. Complete a campaign.", w);
    } else {
        Display::boxLine("  RANK  USERNAME            SCORE    DATE", w);
        Display::boxSeparator(w);
        for (int i = 0; i < (int)v.size(); i++) {
            string name = v[i].username;
            if ((int)name.size() > 18) name = name.substr(0, 18);
            while ((int)name.size() < 20) name += " ";

            ostringstream row;
            row << "  [" << (i + 1) << "]  "
                << left << name
                << setw(9) << v[i].score
                << v[i].date;
            Display::boxLine(row.str(), w);
        }
    }

    Display::boxBottom(w);
    cout << "\n";
    cout.flush();
}

} // namespace scores
