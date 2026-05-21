#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <string>
#include <vector>
using namespace std;

struct LeaderboardEntry {
    string username;
    int totalScore;
    int casesSolved;
    float accuracy;
    string rank;
    string date;
};

class Leaderboard {
public:
    void addEntry(string user, int score, int cases, float acc);
    void show();
    void showUserRank(string username);
    static string getRank(int score, int cases);
private:
    vector<LeaderboardEntry> load();
    void save(vector<LeaderboardEntry> entries);
};

#endif

