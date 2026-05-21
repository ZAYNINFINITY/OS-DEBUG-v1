#include <iostream>     // for input and output
#include <string>       // for using string
#include <cstdlib>      // for srand()
#include <ctime>        // for time()
#include <limits>       // for numeric limits

// header files of project
#include "display.h"
#include "gamestate.h"
#include "inventory.h"
#include "userprofile.h"
#include "leaderboard.h"
#include "history.h"
#include "cases.h"
#include "caselab.h"

using namespace std;

// global pointers for game state and inventory
GameState* g_state = nullptr;
Inventory* g_inventory = nullptr;

// function to show command usage/help
static void showUsage(const char* exeName) {
    cout << "OS Fault Detective CLI\n";
    cout << "Usage: " << exeName << " [--fast] [--help]\n";
    cout << "  --fast   Disable animation delays for demo/testing\n";
    cout << "  --help   Show this help message\n";
}

// function to safely take integer input from user
static int readInt(string prompt, int lo, int hi) {

    // loop until valid input comes
    while (true) {
        cout << prompt;
        cout.flush();

        int v;

        // check if input is valid and in range
        if ((cin >> v) && v >= lo && v <= hi) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return v;
        }

        // clear invalid input
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // warning message
        Display::flashWarning("Enter a number from " + to_string(lo) + " to " + to_string(hi));
    }
}

// function to show starting splash screen
static void showSplash() {

    // show game title
    Display::showTitle();

    int w = 56;

    // project information box
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

    // pause screen for 800ms
    Display::pause(800);
}

// function for selecting difficulty
static Difficulty pickDifficulty() {

    Display::clear();

    // difficulty menu
    cout << "\n  ========================================\n";
    cout << "  SELECT DIFFICULTY\n";
    cout << "  ========================================\n";

    cout << "  [1] TRAINEE   4 min  hints on   score x1.0\n";
    cout << "  [2] ENGINEER  3 min  hints off  score x1.2\n";
    cout << "  [3] SENIOR    2 min  hints off  score x1.5\n";

    cout << "  ========================================\n\n";

    // take choice from user
    int c = readInt("  > Select [1-3]: ", 1, 3);

    // return selected difficulty
    if (c == 1) return Difficulty::Trainee;
    if (c == 3) return Difficulty::Senior;

    return Difficulty::Engineer;
}

// function to run full campaign mode
static void runCampaign() {

    // select difficulty
    Difficulty diff = pickDifficulty();

    // get current user
    UserProfile* user = g_state->getUser();

    // start new game
    g_state->newGame(diff, user);

    // reset inventory
    g_inventory->reset();

    // begin campaign
    g_state->beginCampaign();

    Display::clear();

    // campaign starting message
    cout << "\n  CAMPAIGN STARTED\n";
    cout << "  Player: " << user->getName() << "\n";

    // show difficulty text
    cout << "  Difficulty: "
         << (diff == Difficulty::Trainee ? "TRAINEE"
         : diff == Difficulty::Senior ? "SENIOR"
         : "ENGINEER") << "\n";

    cout << "  Cases: 8 (must clear each to advance)\n\n";

    Display::waitEnter();

    // loop through all 8 cases
    for (int n = 1; n <= 8; n++) {

        // run selected case
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

        // check if player lost
        if (g_state->isCrashed()) {

            Display::clear();

            // glitch effect
            Display::glitch("  CAMPAIGN FAILED");

            cout << "  Stability reached 0%. Campaign aborted.\n\n";

            Display::waitEnter();

            return;
        }
    }

    // finish campaign
    g_state->endCampaign();

    Display::clear();

    // final results
    cout << "\n  CAMPAIGN COMPLETE\n";
    cout << "  Final Score: " << g_state->getTotalScore() << "\n";
    cout << "  Stability: " << g_state->getStability() << "%\n\n";

    // add score to leaderboard
    Leaderboard lb;

    lb.addEntry(
        user->getName(),
        g_state->getTotalScore(),
        user->getCasesSolved(),
        user->getAccuracy()
    );

    // save history
    History::appendRun(
        user->getName(),
        "Campaign",
        g_state->getTotalScore(),
        g_state->getStability()
    );

    // save user profile
    user->save();

    Display::waitEnter();
}

// function for selecting individual chapters
static void chapterSelect() {

    Difficulty diff = pickDifficulty();

    UserProfile* user = g_state->getUser();

    // start new game
    g_state->newGame(diff, user);

    // reset inventory
    g_inventory->reset();

    while (true) {

        Display::clear();

        // chapter selection menu
        cout << "\n  ========================================\n";
        cout << "  CHAPTER SELECT\n";
        cout << "  ========================================\n";

        cout << "  [1] Deadlock        [5] Syscalls\n";
        cout << "  [2] Starvation      [6] Sync\n";
        cout << "  [3] Memory          [7] Lifecycle\n";
        cout << "  [4] Root Cause      [8] Pipeline\n";

        cout << "  [9] Back to Menu\n";

        cout << "  ========================================\n\n";

        // take user choice
        int c = readInt("  > Select [1-9]: ", 1, 9);

        // go back
        if (c == 9) return;

        // unlock tools/items quietly
        g_inventory->unlockQuiet(DEADLOCK);
        g_inventory->unlockQuiet(TERMINATOR);
        g_inventory->unlockQuiet(PRIORITY);
        g_inventory->unlockQuiet(MEMORY);
        g_inventory->unlockQuiet(COMPACTOR);
        g_inventory->unlockQuiet(RR);

        // run selected case
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

// investigation mode menu
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

    // open selected mode
    if (c == 1) runCampaign();
    if (c == 2) chapterSelect();
}

// algorithm lab menu
static void algorithmLab() {

    while (true) {

        // show scheduling algorithms menu
        CaseLab::showMenu();

        int c = readInt("  > Select [1-6]: ", 1, 6);

        // return to menu
        if (c == 6) return;

        // run selected algorithm
        switch (c) {

            case 1: CaseLab::runFCFS(); break;
            case 2: CaseLab::runSJF(); break;
            case 3: CaseLab::runRoundRobin(); break;
            case 4: CaseLab::runPriority(); break;
            case 5: CaseLab::compareAll(); break;
        }
    }
}

// function to show user profile
static void showProfile() {

    UserProfile* user = g_state->getUser();

    if (user) {

        // display user stats
        user->showStats();

        // show user ranking
        Leaderboard lb;
        lb.showUserRank(user->getName());
    }

    Display::waitEnter();
}

// function to show leaderboard
static void showLeaderboard() {

    Leaderboard lb;

    // display leaderboard
    lb.show();

    Display::waitEnter();
}

// function to display main menu
static int mainMenu() {

    UserProfile* user = g_state->getUser();

    // get user information
    string name = user ? user->getName() : "Guest";
    int score = user ? user->getTotalScore() : 0;
    int cases = user ? user->getCasesSolved() : 0;

    Display::clear();

    // show menu header
    Display::showMenuHeader(name, score, cases);

    // menu options
    cout << "  [1] INVESTIGATION  - Solve OS incidents\n";
    cout << "  [2] ALGORITHM LAB  - Study scheduling\n";
    cout << "  [3] MY PROFILE     - View stats\n";
    cout << "  [4] LEADERBOARD    - Global rankings\n";
    cout << "  [5] EXIT\n";

    cout << "  ========================================\n\n";

    // return user choice
    return readInt("  > Choice: ", 1, 5);
}

// login function
static UserProfile* login() {

    // get all saved usernames
    vector<string> users = UserProfile::getAllUsernames();

    Display::clear();

    cout << "\n  ========================================\n";
    cout << "  USER LOGIN\n";
    cout << "  ========================================\n";

    // if no users exist
    if (users.empty()) {

        cout << "  No users found. Create one.\n";
        cout << "  Enter username: ";

        string name;
        getline(cin, name);

        // default name if empty
        if (name.empty()) name = "Student";

        // create and save new user
        UserProfile newUser(name);
        newUser.save();

        return new UserProfile(name);
    }

    // display all existing users
    cout << "  Existing users:\n";

    for (size_t i = 0; i < users.size(); i++) {
        cout << "  [" << (i+1) << "] " << users[i] << "\n";
    }

    // option to create new user
    cout << "  [" << (users.size()+1) << "] Create new user\n";

    cout << "  ========================================\n\n";

    // take user choice
    int c = readInt("  > Choice: ", 1, (int)users.size() + 1);

    // load existing user
    if (c <= (int)users.size()) {
        return new UserProfile(UserProfile::load(users[c-1]));
    }

    // create new user
    cout << "  Enter username: ";

    string name;
    getline(cin, name);

    if (name.empty()) name = "Student";

    UserProfile newUser(name);
    newUser.save();

    return new UserProfile(name);
}

// main function
int main(int argc, char** argv) {

    // check command line arguments
    for (int i = 1; i < argc; i++) {

        string arg = argv[i];

        // show help menu
        if (arg == "--help" || arg == "-h") {
            showUsage(argv[0]);
            return 0;
        }

        // enable fast mode
        if (arg == "--fast") {
            Display::setFastMode(true);
        }
    }

    // random seed initialization
    srand((unsigned)time(NULL));

    // create game objects
    g_state = new GameState();
    g_inventory = new Inventory();

    // show starting screen
    showSplash();

    // login user
    UserProfile* user = login();

    // create default game
    g_state->newGame(Difficulty::Engineer, user);

    bool running = true;

    // main game loop
    while (running) {

        // handle menu choices
        switch (mainMenu()) {

            case 1: investigationMenu(); break;
            case 2: algorithmLab(); break;
            case 3: showProfile(); break;
            case 4: showLeaderboard(); break;

            // exit game
            case 5: running = false; break;
        }
    }

    Display::clear();

    // goodbye message
    cout << "\n  Session ended. Goodbye, "
         << user->getName() << ".\n\n";

    // free memory
    delete g_state;
    delete g_inventory;
    delete user;

    return 0;
}