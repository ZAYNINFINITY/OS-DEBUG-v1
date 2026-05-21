#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>
#include "display.h"
#include "gamestate.h"
#include "inventory.h"
#include "userprofile.h"
#include "leaderboard.h"
#include "history.h"
#include "cases.h"
#include "caselab.h"
using namespace std;

GameState* g_state = nullptr;
Inventory* g_inventory = nullptr;

static void showUsage(const char* exeName) {
    cout << "OS Fault Detective CLI\n";
    cout << "Usage: " << exeName << " [--fast] [--help]\n";
    cout << "  --fast   Disable animation delays for demo/testing\n";
    cout << "  --help   Show this help message\n";
}

static int readInt(string prompt, int lo, int hi) {
    while (true) {
        cout << prompt;
        cout.flush();
        int v;
        if ((cin >> v) && v >= lo && v <= hi) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return v;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        Display::flashWarning("Enter a number from " + to_string(lo) + " to " + to_string(hi));
    }
}

static void showSplash() {
    Display::showTitle();
    int w = 56;
    Display::boxTop(w);
    Display::boxLine("  SEMESTER PROJECT", w);
    Display::boxSeparator(w);
    Display::boxLine("  Student   :  Zain ul Abideen", w);
    Display::boxLine("  ID        :  B24F0357CS083", w);
    Display::boxLine("  Course    :  Operating Systems Lab", w);
    Display::boxLine("  Semester  :  Spring 2026", w);
    Display::boxLine("  Institute :  PAFIAST", w);
    Display::boxSeparator(w);
    Display::boxLine("  Project   :  OS Fault Detective  v2.0", w);
    Display::boxBottom(w);
    Display::pause(800);
}

static Difficulty pickDifficulty() {
    Display::clear();
    cout << "\n  ========================================\n";
    cout << "  SELECT DIFFICULTY\n";
    cout << "  ========================================\n";
    cout << "  [1] TRAINEE   4 min  hints on   score x1.0\n";
    cout << "  [2] ENGINEER  3 min  hints off  score x1.2\n";
    cout << "  [3] SENIOR    2 min  hints off  score x1.5\n";
    cout << "  ========================================\n\n";
    int c = readInt("  > Select [1-3]: ", 1, 3);
    if (c == 1) return Difficulty::Trainee;
    if (c == 3) return Difficulty::Senior;
    return Difficulty::Engineer;
}

static void runCampaign() {
    Difficulty diff = pickDifficulty();
    UserProfile* user = g_state->getUser();
    g_state->newGame(diff, user);
    g_inventory->reset();
    g_state->beginCampaign();

    Display::clear();
    cout << "\n  CAMPAIGN STARTED\n";
    cout << "  Player: " << user->getName() << "\n";
    cout << "  Difficulty: " << (diff == Difficulty::Trainee ? "TRAINEE" : diff == Difficulty::Senior ? "SENIOR" : "ENGINEER") << "\n";
    cout << "  Cases: 8 (must clear each to advance)\n\n";
    Display::waitEnter();

    for (int n = 1; n <= 8; n++) {
        switch (n) {
            case 1: case1_deadlock(); break;
            case 2: case2_starvation(); break;
            case 3: case3_memory(); break;
            case 4: case4_boss(); break;
            case 5: case5_syscalls(); break;
            case 6: case6_sync(); break;
            case 7: case7_process_lifecycle(); break;
            case 8: case8_client_server_pipeline(); break;
        }
        if (g_state->isCrashed()) {
            Display::clear();
            Display::glitch("  CAMPAIGN FAILED");
            cout << "  Stability reached 0%. Campaign aborted.\n\n";
            Display::waitEnter();
            return;
        }
    }

    g_state->endCampaign();
    Display::clear();
    cout << "\n  CAMPAIGN COMPLETE\n";
    cout << "  Final Score: " << g_state->getTotalScore() << "\n";
    cout << "  Stability: " << g_state->getStability() << "%\n\n";

    Leaderboard lb;
    lb.addEntry(user->getName(), g_state->getTotalScore(), user->getCasesSolved(), user->getAccuracy());
    History::appendRun(user->getName(), "Campaign", g_state->getTotalScore(), g_state->getStability());
    user->save();

    Display::waitEnter();
}

static void chapterSelect() {
    Difficulty diff = pickDifficulty();
    UserProfile* user = g_state->getUser();
    g_state->newGame(diff, user);
    g_inventory->reset();

    while (true) {
        Display::clear();
        cout << "\n  ========================================\n";
        cout << "  CHAPTER SELECT\n";
        cout << "  ========================================\n";
        cout << "  [1] Deadlock        [5] Syscalls\n";
        cout << "  [2] Starvation      [6] Sync\n";
        cout << "  [3] Memory          [7] Lifecycle\n";
        cout << "  [4] Root Cause      [8] Pipeline\n";
        cout << "  [9] Back to Menu\n";
        cout << "  ========================================\n\n";

        int c = readInt("  > Select [1-9]: ", 1, 9);
        if (c == 9) return;

        g_inventory->unlockQuiet(DEADLOCK);
        g_inventory->unlockQuiet(TERMINATOR);
        g_inventory->unlockQuiet(PRIORITY);
        g_inventory->unlockQuiet(MEMORY);
        g_inventory->unlockQuiet(COMPACTOR);
        g_inventory->unlockQuiet(RR);

        switch (c) {
            case 1: case1_deadlock(); break;
            case 2: case2_starvation(); break;
            case 3: case3_memory(); break;
            case 4: case4_boss(); break;
            case 5: case5_syscalls(); break;
            case 6: case6_sync(); break;
            case 7: case7_process_lifecycle(); break;
            case 8: case8_client_server_pipeline(); break;
        }
    }
}

static void investigationMenu() {
    Display::clear();
    cout << "\n  ========================================\n";
    cout << "  INVESTIGATION MODE\n";
    cout << "  ========================================\n";
    cout << "  [1] Campaign Mode    - All 8 cases in order\n";
    cout << "  [2] Chapter Select   - Practice any case\n";
    cout << "  [3] Back\n";
    cout << "  ========================================\n\n";

    int c = readInt("  > Select [1-3]: ", 1, 3);
    if (c == 1) runCampaign();
    if (c == 2) chapterSelect();
}

static void algorithmLab() {
    while (true) {
        CaseLab::showMenu();
        int c = readInt("  > Select [1-6]: ", 1, 6);
        if (c == 6) return;
        switch (c) {
            case 1: CaseLab::runFCFS(); break;
            case 2: CaseLab::runSJF(); break;
            case 3: CaseLab::runRoundRobin(); break;
            case 4: CaseLab::runPriority(); break;
            case 5: CaseLab::compareAll(); break;
        }
    }
}

static void showProfile() {
    UserProfile* user = g_state->getUser();
    if (user) {
        user->showStats();
        Leaderboard lb;
        lb.showUserRank(user->getName());
    }
    Display::waitEnter();
}

static void showLeaderboard() {
    Leaderboard lb;
    lb.show();
    Display::waitEnter();
}

static int mainMenu() {
    UserProfile* user = g_state->getUser();
    string name = user ? user->getName() : "Guest";
    int score = user ? user->getTotalScore() : 0;
    int cases = user ? user->getCasesSolved() : 0;

    Display::clear();
    Display::showMenuHeader(name, score, cases);
    cout << "  [1] INVESTIGATION  - Solve OS incidents\n";
    cout << "  [2] ALGORITHM LAB  - Study scheduling\n";
    cout << "  [3] MY PROFILE     - View stats\n";
    cout << "  [4] LEADERBOARD    - Global rankings\n";
    cout << "  [5] EXIT\n";
    cout << "  ========================================\n\n";
    return readInt("  > Choice: ", 1, 5);
}

static UserProfile* login() {
    vector<string> users = UserProfile::getAllUsernames();
    Display::clear();
    cout << "\n  ========================================\n";
    cout << "  USER LOGIN\n";
    cout << "  ========================================\n";

    if (users.empty()) {
        cout << "  No users found. Create one.\n";
        cout << "  Enter username: ";
        string name;
        getline(cin, name);
        if (name.empty()) name = "Student";
        UserProfile newUser(name);
        newUser.save();
        return new UserProfile(name);
    }

    cout << "  Existing users:\n";
    for (size_t i = 0; i < users.size(); i++) {
        cout << "  [" << (i+1) << "] " << users[i] << "\n";
    }
    cout << "  [" << (users.size()+1) << "] Create new user\n";
    cout << "  ========================================\n\n";

    int c = readInt("  > Choice: ", 1, (int)users.size() + 1);
    if (c <= (int)users.size()) {
        return new UserProfile(UserProfile::load(users[c-1]));
    }

    cout << "  Enter username: ";
    string name;
    getline(cin, name);
    if (name.empty()) name = "Student";
    UserProfile newUser(name);
    newUser.save();
    return new UserProfile(name);
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            showUsage(argv[0]);
            return 0;
        }
        if (arg == "--fast") {
            Display::setFastMode(true);
        }
    }

    srand((unsigned)time(NULL));
    g_state = new GameState();
    g_inventory = new Inventory();

    showSplash();

    UserProfile* user = login();
    g_state->newGame(Difficulty::Engineer, user);

    bool running = true;
    while (running) {
        switch (mainMenu()) {
            case 1: investigationMenu(); break;
            case 2: algorithmLab(); break;
            case 3: showProfile(); break;
            case 4: showLeaderboard(); break;
            case 5: running = false; break;
        }
    }

    Display::clear();
    cout << "\n  Session ended. Goodbye, " << user->getName() << ".\n\n";

    delete g_state;
    delete g_inventory;
    delete user;
    return 0;
}
