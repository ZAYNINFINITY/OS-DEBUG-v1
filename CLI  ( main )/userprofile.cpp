#include "userprofile.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <algorithm>
using namespace std;

struct StoredProfile {
    string username;
    int totalScore;
    int casesSolved;
    int bestStability;
    int wrongMoves;
    int totalMoves;
    int caseScores[8];
    string caseEndings[8];
};

static vector<StoredProfile> loadStoredProfiles();
static void saveStoredProfiles(const vector<StoredProfile>& profiles);

UserProfile::UserProfile(string name) {
    username = name;
    totalScore = 0;
    casesSolved = 0;
    accuracyRate = 0.0;
    bestStability = 100;
    totalPlayTime = 0;
    wrongMoves = 0;
    totalMoves = 0;
    for (int i = 0; i < 8; i++) {
        caseScores[i] = 0;
        caseEndings[i] = "-";
    }
}

string UserProfile::getName() {
    return username;
}

int UserProfile::getTotalScore() {
    return totalScore;
}

int UserProfile::getCasesSolved() {
    return casesSolved;
}

float UserProfile::getAccuracy() {
    if (totalMoves == 0) return 0.0;
    return ((float)(totalMoves - wrongMoves) / totalMoves) * 100.0;
}

int UserProfile::getBestStability() {
    return bestStability;
}

void UserProfile::addCaseResult(int caseId, int score, string ending, int stability, int wrongs) {
    int idx = caseId - 1;
    if (idx < 0 || idx > 7) return;
    caseScores[idx] = score;
    caseEndings[idx] = ending;
    totalScore += score;
    casesSolved++;
    wrongMoves += wrongs;
    totalMoves += 10;
    if (stability < bestStability) bestStability = stability;
}

void UserProfile::save() {
    vector<StoredProfile> profiles = loadStoredProfiles();
    StoredProfile current;
    current.username = username;
    current.totalScore = totalScore;
    current.casesSolved = casesSolved;
    current.bestStability = bestStability;
    current.wrongMoves = wrongMoves;
    current.totalMoves = totalMoves;
    for (int i = 0; i < 8; i++) {
        current.caseScores[i] = caseScores[i];
        current.caseEndings[i] = caseEndings[i];
    }

    bool found = false;
    for (size_t i = 0; i < profiles.size(); i++) {
        if (profiles[i].username == username) {
            profiles[i] = current;
            found = true;
            break;
        }
    }

    if (!found) {
        profiles.push_back(current);
    }

    saveStoredProfiles(profiles);
}

UserProfile UserProfile::load(string name) {
    UserProfile profile(name);
    vector<StoredProfile> profiles = loadStoredProfiles();

    for (size_t i = 0; i < profiles.size(); i++) {
        if (profiles[i].username == name) {
            profile.totalScore = profiles[i].totalScore;
            profile.casesSolved = profiles[i].casesSolved;
            profile.bestStability = profiles[i].bestStability;
            profile.wrongMoves = profiles[i].wrongMoves;
            profile.totalMoves = profiles[i].totalMoves;
            for (int j = 0; j < 8; j++) {
                profile.caseScores[j] = profiles[i].caseScores[j];
                profile.caseEndings[j] = profiles[i].caseEndings[j];
            }
            break;
        }
    }

    return profile;
}

vector<string> UserProfile::getAllUsernames() {
    vector<string> users;
    ifstream file("profiles.txt");
    if (!file.is_open()) return users;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string user;
        getline(ss, user, '|');
        users.push_back(user);
    }

    sort(users.begin(), users.end());
    users.erase(unique(users.begin(), users.end()), users.end());
    return users;
}

void UserProfile::showStats() {
    cout << "\n  ========================================\n";
    cout << "  USER PROFILE: " << username << "\n";
    cout << "  ========================================\n";
    cout << "  Total Score:    " << totalScore << "\n";
    cout << "  Cases Solved:   " << casesSolved << "/8\n";
    cout << "  Accuracy:       " << fixed << setprecision(1) << getAccuracy() << "%\n";
    cout << "  Best Stability: " << bestStability << "%\n";
    cout << "  ----------------------------------------\n";
    cout << "  CASE PROGRESS:\n";
    for (int i = 0; i < 8; i++) {
        cout << "  Case " << (i+1) << ": " << setw(10) << caseEndings[i];
        if (caseScores[i] > 0) cout << "  (" << caseScores[i] << " pts)";
        cout << "\n";
    }
    cout << "  ========================================\n\n";
}

int UserProfile::getCaseScore(int id) {
    if (id < 1 || id > 8) return 0;
    return caseScores[id-1];
}

string UserProfile::getCaseEnding(int id) {
    if (id < 1 || id > 8) return "-";
    return caseEndings[id-1];
}

static vector<StoredProfile> loadStoredProfiles() {
    vector<StoredProfile> profiles;
    ifstream file("profiles.txt");
    if (!file.is_open()) return profiles;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string user, total, solved, acc, stab, scores, ends, wrongs, moves;

        getline(ss, user, '|');
        getline(ss, total, '|');
        getline(ss, solved, '|');
        getline(ss, acc, '|');
        getline(ss, stab, '|');
        getline(ss, scores, '|');
        getline(ss, ends, '|');
        getline(ss, wrongs, '|');
        getline(ss, moves, '|');

        StoredProfile stored;
        stored.username = user;
        stored.totalScore = total.empty() ? 0 : stoi(total);
        stored.casesSolved = solved.empty() ? 0 : stoi(solved);
        stored.bestStability = stab.empty() ? 100 : stoi(stab);
        stored.wrongMoves = wrongs.empty() ? 0 : stoi(wrongs);
        stored.totalMoves = moves.empty() ? 0 : stoi(moves);

        for (int i = 0; i < 8; i++) {
            stored.caseScores[i] = 0;
            stored.caseEndings[i] = "-";
        }

        stringstream scoreStream(scores);
        string scoreVal;
        int idx = 0;
        while (getline(scoreStream, scoreVal, ',') && idx < 8) {
            stored.caseScores[idx++] = scoreVal.empty() ? 0 : stoi(scoreVal);
        }

        stringstream endStream(ends);
        string endVal;
        idx = 0;
        while (getline(endStream, endVal, ',') && idx < 8) {
            stored.caseEndings[idx++] = endVal.empty() ? "-" : endVal;
        }

        bool replaced = false;
        for (size_t i = 0; i < profiles.size(); i++) {
            if (profiles[i].username == stored.username) {
                profiles[i] = stored;
                replaced = true;
                break;
            }
        }
        if (!replaced) {
            profiles.push_back(stored);
        }
    }

    return profiles;
}

static void saveStoredProfiles(const vector<StoredProfile>& profiles) {
    ofstream file("profiles.txt", ios::trunc);
    if (!file.is_open()) return;

    for (size_t i = 0; i < profiles.size(); i++) {
        const StoredProfile& stored = profiles[i];
        float accuracy = 0.0f;
        if (stored.totalMoves > 0) {
            accuracy = ((float)(stored.totalMoves - stored.wrongMoves) / stored.totalMoves) * 100.0f;
        }

        file << stored.username << "|"
             << stored.totalScore << "|"
             << stored.casesSolved << "|"
             << fixed << setprecision(1) << accuracy << "|"
             << stored.bestStability << "|";

        for (int j = 0; j < 8; j++) {
            file << stored.caseScores[j];
            if (j < 7) file << ",";
        }
        file << "|";

        for (int j = 0; j < 8; j++) {
            file << stored.caseEndings[j];
            if (j < 7) file << ",";
        }
        file << "|"
             << stored.wrongMoves << "|"
             << stored.totalMoves << "\n";
    }
}

