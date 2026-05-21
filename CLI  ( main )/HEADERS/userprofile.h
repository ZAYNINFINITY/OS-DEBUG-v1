#ifndef USERPROFILE_H
#define USERPROFILE_H

#include <string>
#include <vector>
using namespace std;

class UserProfile {
private:
    string username;
    int totalScore;
    int casesSolved;
    float accuracyRate;
    int caseScores[8];
    string caseEndings[8];
    int bestStability;
    long totalPlayTime;
    int wrongMoves;
    int totalMoves;

public:
    UserProfile(string name);
    string getName();
    int getTotalScore();
    int getCasesSolved();
    float getAccuracy();
    int getBestStability();
    void addCaseResult(int caseId, int score, string ending, int stability, int wrongs);
    void save();
    static UserProfile load(string name);
    static vector<string> getAllUsernames();
    void showStats();
    int getCaseScore(int id);
    string getCaseEnding(int id);
};

#endif

