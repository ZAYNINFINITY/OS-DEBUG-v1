#include "leaderboard.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <ctime>
using namespace std;

static void sortEntries(vector<LeaderboardEntry>& entries) {
    sort(entries.begin(), entries.end(), [](const LeaderboardEntry& a, const LeaderboardEntry& b) {
        if (a.totalScore != b.totalScore) return a.totalScore > b.totalScore;
        if (a.casesSolved != b.casesSolved) return a.casesSolved > b.casesSolved;
        if (a.accuracy != b.accuracy) return a.accuracy > b.accuracy;
        return a.username < b.username;
    });
}

string Leaderboard::getRank(int score, int cases) {
    if (cases == 8 && score >= 8000) return "OSS Expert";
    if (cases == 8 && score >= 6000) return "Senior Dev";
    if (cases >= 5) return "Engineer";
    if (cases >= 2) return "Trainee";
    return "Intern";
}

vector<LeaderboardEntry> Leaderboard::load() {
    vector<LeaderboardEntry> entries;
    ifstream file("leaderboard.txt");
    if (!file.is_open()) return entries;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        LeaderboardEntry e;
        string scoreStr, casesStr, accStr;
        getline(ss, e.username, '|');
        getline(ss, scoreStr, '|');
        getline(ss, casesStr, '|');
        getline(ss, accStr, '|');
        getline(ss, e.rank, '|');
        getline(ss, e.date, '|');
        e.totalScore = stoi(scoreStr);
        e.casesSolved = stoi(casesStr);
        e.accuracy = stof(accStr);
        entries.push_back(e);
    }
    return entries;
}

void Leaderboard::save(vector<LeaderboardEntry> entries) {
    sortEntries(entries);
    ofstream file("leaderboard.txt", ios::trunc);
    for (auto& e : entries) {
        file << e.username << "|"
             << e.totalScore << "|"
             << e.casesSolved << "|"
             << fixed << setprecision(1) << e.accuracy << "|"
             << e.rank << "|"
             << e.date << "\n";
    }
}

void Leaderboard::addEntry(string user, int score, int cases, float acc) {
    vector<LeaderboardEntry> entries = load();

    bool found = false;
    for (size_t i = 0; i < entries.size(); i++) {
        if (entries[i].username == user) {
            if (score > entries[i].totalScore) {
                entries[i].totalScore = score;
                entries[i].casesSolved = cases;
                entries[i].accuracy = acc;
                entries[i].rank = getRank(score, cases);
                time_t now = time(0);
                tm* ltm = localtime(&now);
                stringstream date;
                date << 1900 + ltm->tm_year << "-"
                     << 1 + ltm->tm_mon << "-"
                     << ltm->tm_mday;
                entries[i].date = date.str();
            }
            found = true;
            break;
        }
    }

    if (!found) {
        LeaderboardEntry e;
        e.username = user;
        e.totalScore = score;
        e.casesSolved = cases;
        e.accuracy = acc;
        e.rank = getRank(score, cases);
        time_t now = time(0);
        tm* ltm = localtime(&now);
        stringstream date;
        date << 1900 + ltm->tm_year << "-"
             << 1 + ltm->tm_mon << "-"
             << ltm->tm_mday;
        e.date = date.str();
        entries.push_back(e);
    }

    sortEntries(entries);
    save(entries);
}

void Leaderboard::show() {
    vector<LeaderboardEntry> entries = load();
    sortEntries(entries);

    cout << "\n  ========================================\n";
    cout << "  GLOBAL LEADERBOARD\n";
    cout << "  ========================================\n";
    cout << "  RANK  USER           SCORE  CASES  ACCURACY  TITLE\n";
    cout << "  ----------------------------------------\n";

    for (size_t i = 0; i < entries.size() && i < 10; i++) {
        cout << "  " << setw(2) << (i+1) << "    "
             << setw(14) << left << entries[i].username
             << setw(7) << entries[i].totalScore
             << setw(6) << entries[i].casesSolved
             << setw(10) << fixed << setprecision(1) << entries[i].accuracy
             << entries[i].rank << "\n";
    }

    if (entries.empty()) {
        cout << "  No entries yet.\n";
    }

    cout << "  ========================================\n\n";
}

void Leaderboard::showUserRank(string username) {
    vector<LeaderboardEntry> entries = load();
    sortEntries(entries);
    for (size_t i = 0; i < entries.size(); i++) {
        if (entries[i].username == username) {
            cout << "  Your Rank: " << (i+1) << " of " << entries.size() << "\n";
            cout << "  Title: " << entries[i].rank << "\n";
            return;
        }
    }
    cout << "  Not ranked yet.\n";
}

