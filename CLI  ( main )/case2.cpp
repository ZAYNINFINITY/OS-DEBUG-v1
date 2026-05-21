#include "cases.h"
#include "display.h"
#include "hud.h"
#include "inventory.h"
#include "gamestate.h"
#include "debugger.h"
#include "case_runner.h"
#include <iostream>
using namespace std;

extern GameState* g_state;
extern Inventory* g_inventory;

void case2_starvation() {
    Display::clear();
    Display::divider();
    Display::typewriter("  [ INCOMING TRANSMISSION -- Day 3 ]", 45000);
    Display::typewriter("  [ NOVACORP PROCESSING CENTER     ]", 45000);
    Display::divider();
    Display::pause(800);
    Display::typewriter("  Process DELTA has been waiting 72 hours.", 60000);
    Display::typewriter("  It has never been given CPU time.", 55000);
    Display::typewriter("  The database cleanup assigned to it never ran.", 55000);
    Display::typewriter("  Now the database is corrupting.", 60000);
    Display::divider();
    Display::waitEnter();

    Display::clear();
    Display::typewriter("  FLASHBACK -- three days ago:", 55000);
    Display::pause(800);
    Display::typewriter("  ALPHA arrived. High priority. Runs first.", 50000);
    Display::typewriter("  BETA arrived.  High priority. Runs first.", 50000);
    Display::typewriter("  GAMMA arrived. High priority. Runs first.", 50000);
    Display::typewriter("  DELTA waiting. Low priority. Never chosen.", 65000);
    Display::pause(900);
    Display::glitch("  72 hours later -- DELTA still waiting.");
    Display::divider();
    Display::waitEnter();

    ProcessQueue queue;
    queue.addProcess({1, "ALPHA", "RUNNING", 1, 10, 0, 0, {}, {}});
    queue.addProcess({2, "BETA", "RUNNING", 1, 8, 0, 0, {}, {}});
    queue.addProcess({3, "GAMMA", "RUNNING", 1, 12, 0, 0, {}, {}});
    queue.addProcess({4, "DELTA", "WAITING", 4, 5, 0, 72, {}, {}});

    case_runner::Context ctx = case_runner::begin(2, "CASE 2: THE IGNORED WORKER");
    bool usedPs = false;
    bool usedQueue = false;
    bool done = false;
    int efficiency = 0;

    while (!done) {
        if (case_runner::should_abort(ctx)) {
            CaseScore cs = g_state->endCase(false, false, efficiency);
            Display::divider();
            Display::typewriter("  CASE FAILED", 65000);
            if (g_state->isCrashed()) cout << "  Stability reached 0%.\n";
            else cout << "  Timer expired before diagnosis was completed.\n";
            cout << "  Score: " << cs.points << "   Efficiency: " << efficiency << "%\n";
            Display::waitEnter();
            return;
        }
        HUD::render("CASE 2: THE IGNORED WORKER", 0, 3, g_state->getStability(), case_runner::time_left(ctx));

        cout << "\n  DEBUG COMMANDS:\n";
        cout << "  [1] ps       - Show process states\n";
        cout << "  [2] queue    - Show ready queue\n";
        cout << "  [3] diagnose - Submit diagnosis\n";
        cout << "  [4] fix      - Apply fix\n";
        cout << "\n  > Choice: ";
        cout.flush();

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            usedPs = true;
            cout << "\n  +-------+---------+----------+--------+\n";
            cout << "  | NAME  |  STATE  | PRIORITY | WAITED |\n";
            cout << "  +-------+---------+----------+--------+\n";
            cout << "  | ALPHA | RUNNING | HIGH   1 |   0 hr |\n";
            cout << "  | BETA  | RUNNING | HIGH   1 |   0 hr |\n";
            cout << "  | GAMMA | RUNNING | HIGH   1 |   0 hr |\n";
            cout << "  | DELTA | WAITING | LOW    4 |  72 hr |\n";
            cout << "  +-------+---------+----------+--------+\n\n";
            Display::typewriter("  DELTA: 72 hours. Never chosen. Not blocked -- just ignored.", 50000);
        } else if (choice == 2) {
            usedQueue = true;
            queue.showQueue();
            Display::typewriter("  DELTA is at the back. High priority keeps preempting.", 50000);
        } else if (choice == 3) {
            if (!usedPs || !usedQueue) {
                Display::typewriter("  Run diagnostics first.", 45000);
                g_state->wrongMove();
                continue;
            }
            cout << "\n  Enter diagnosis (starvation/deadlock/memory): ";
            string diag;
            getline(cin, diag);
            if (diag == "starvation") {
                Display::typewriter("  Correct. STARVATION -- low priority never gets CPU.", 50000);
                efficiency += 30;
                break;
            } else {
                Display::glitch("  Wrong diagnosis.");
                g_state->wrongMove();
            }
        } else if (choice == 4) {
            Display::typewriter("  Diagnose first.", 45000);
        }
    }

    done = false;
    while (!done) {
        if (case_runner::should_abort(ctx)) {
            CaseScore cs = g_state->endCase(false, false, efficiency);
            Display::divider();
            Display::typewriter("  CASE FAILED", 65000);
            if (g_state->isCrashed()) cout << "  Stability reached 0%.\n";
            else cout << "  Timer expired before the fix was applied.\n";
            cout << "  Score: " << cs.points << "   Efficiency: " << efficiency << "%\n";
            Display::waitEnter();
            return;
        }
        HUD::render("CASE 2: THE IGNORED WORKER", 1, 3, g_state->getStability(), case_runner::time_left(ctx));

        cout << "\n  FIX OPTIONS:\n";
        cout << "  [1] aging          - Priority rises over wait time (best)\n";
        cout << "  [2] kill delta     - Remove starving process\n";
        cout << "  [3] add memory     - Does not help\n";
        cout << "\n  > Choice: ";
        cout.flush();

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            Display::typewriter("  Applying priority aging...", 50000);
            Display::loadingBar("", 80000);
            Display::typewriter("  DELTA priority: LOW -> MEDIUM -> HIGH", 50000);
            Display::typewriter("  DELTA enters CPU queue. Runs. Finally.", 70000);
            efficiency += 40;
            done = true;
        } else if (choice == 2) {
            Display::typewriter("  Killing DELTA abandons the work. Database cleanup never runs.", 50000);
            efficiency += 20;
            done = true;
        } else if (choice == 3) {
            Display::glitch("  DELTA does not need memory. It needs CPU time.");
            g_state->wrongMove();
        }
    }

    if (usedPs && usedQueue) efficiency += 20;
    if (g_state->getStability() > 80) efficiency += 10;

    CaseScore cs = g_state->endCase(true, efficiency >= 90, efficiency);
    g_inventory->unlock(PRIORITY);
    g_inventory->unlock(RR);

    Display::divider();
    if (cs.ending == Ending::Perfect) {
        Display::typewriter("  PERFECT RESOLUTION", 65000);
    } else if (cs.ending == Ending::Good) {
        Display::typewriter("  GOOD RESOLUTION", 65000);
    } else {
        Display::typewriter("  MESSY RESOLUTION", 65000);
    }
    Display::divider();
    cout << "  Score: " << cs.points << "   Efficiency: " << efficiency << "%\n";
    Display::waitEnter();
}
