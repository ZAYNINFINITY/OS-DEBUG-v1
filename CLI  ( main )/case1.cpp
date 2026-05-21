#include "cases.h"
#include "display.h"
#include "hud.h"
#include "inventory.h"
#include "gamestate.h"
#include "debugger.h"
#include "case_engine.h"
#include "case_runner.h"
#include <iostream>
#include <ctime>
using namespace std;

extern GameState* g_state;
extern Inventory* g_inventory;

void case1_deadlock() {
    Display::clear();
    Display::divider();
    Display::typewriter("  [ INCOMING TRANSMISSION -- 03:47 AM ]", 45000);
    Display::typewriter("  [ NOVACORP SERVER ROOM B           ]", 45000);
    Display::divider();
    Display::pause(800);
    Display::typewriter("  Three processes went silent an hour ago.", 50000);
    Display::typewriter("  Nothing is finishing. CPU is idle.", 55000);
    Display::typewriter("  But nobody released anything.", 60000);
    Display::typewriter("  We lose $4,000 every minute. You have 3 minutes.", 50000);
    Display::divider();
    Display::waitEnter();

    Display::clear();
    Display::typewriter("  FLASHBACK -- six hours ago:", 55000);
    Display::pause(800);
    Display::typewriter("  P1 grabbed Res-A, asked for Res-B.", 50000);
    Display::typewriter("  P2 grabbed Res-B, asked for Res-C.", 50000);
    Display::typewriter("  P3 grabbed Res-C, asked for Res-A.", 50000);
    Display::pause(800);
    Display::glitch("  Nobody got what they needed. Still waiting.");
    Display::divider();
    Display::waitEnter();

    ResourceGraph graph;
    PCB p1 = {1, "Alpha", "WAITING", 2, 10, 0, 0, {1}, {2}};
    PCB p2 = {2, "Beta", "WAITING", 2, 10, 0, 0, {2}, {3}};
    PCB p3 = {3, "Gamma", "WAITING", 2, 10, 0, 0, {3}, {1}};
    graph.addProcess(p1);
    graph.addProcess(p2);
    graph.addProcess(p3);
    graph.allocateResource(1, 1);
    graph.allocateResource(2, 2);
    graph.allocateResource(3, 3);
    graph.requestResource(1, 2);
    graph.requestResource(2, 3);
    graph.requestResource(3, 1);

    CaseEngine engine(g_state, 1, "CASE 1: THE FROZEN CITY");
    engine.setCorrectDiagnosis("deadlock");
    engine.setCorrectFix("terminate p3");

    bool usedPs = false;
    bool usedResources = false;
    bool done = false;
    int efficiency = 0;

    case_runner::Context ctx = case_runner::begin(1, "CASE 1: THE FROZEN CITY");

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
        HUD::render("CASE 1: THE FROZEN CITY", 0, 3, g_state->getStability(), case_runner::time_left(ctx));

        cout << "\n  DEBUG COMMANDS:\n";
        cout << "  [1] ps          - Show process table\n";
        cout << "  [2] resources   - Show resource graph\n";
        cout << "  [3] analyze     - Detect circular wait\n";
        cout << "  [4] diagnose    - Submit diagnosis\n";
        cout << "  [5] fix         - Apply fix\n";
        cout << "\n  > Choice: ";
        cout.flush();

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            usedPs = true;
            graph.showProcessTable();
            Display::typewriter("  All three processes waiting. Nobody running.", 50000);
        } else if (choice == 2) {
            usedResources = true;
            graph.showGraph();
            Display::typewriter("  Each process holds one resource, wants another.", 50000);
        } else if (choice == 3) {
            if (graph.hasDeadlock()) {
                Display::typewriter("  CIRCULAR WAIT DETECTED: P1 -> P2 -> P3 -> P1", 50000);
                Display::typewriter("  All 4 Coffman conditions met.", 50000);
            }
        } else if (choice == 4) {
            if (!usedPs || !usedResources) {
                Display::typewriter("  Run diagnostics first.", 45000);
                g_state->wrongMove();
                continue;
            }
            cout << "\n  Enter diagnosis (deadlock/starvation/memory): ";
            string diag;
            getline(cin, diag);
            if (diag == "deadlock") {
                Display::typewriter("  Correct. DEADLOCK -- circular wait.", 50000);
                efficiency += 30;
                break;
            } else {
                Display::glitch("  Wrong diagnosis.");
                g_state->wrongMove();
            }
        } else if (choice == 5) {
            Display::typewriter("  Diagnose first.", 45000);
        }
    }

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
        HUD::render("CASE 1: THE FROZEN CITY", 1, 3, g_state->getStability(), case_runner::time_left(ctx));

        cout << "\n  FIX OPTIONS:\n";
        cout << "  [1] terminate p3  - Break the chain (best)\n";
        cout << "  [2] restart all   - Nuclear option\n";
        cout << "  [3] add memory    - Does not help\n";
        cout << "\n  > Choice: ";
        cout.flush();

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            Display::typewriter("  Terminating P3...", 50000);
            Display::loadingBar("", 80000);
            Display::typewriter("  P3 releases Res-C. P2 receives Res-C. P2 completes.", 50000);
            Display::typewriter("  P2 releases Res-B. P1 receives Res-B. P1 completes.", 50000);
            Display::typewriter("  The circle is broken. All processes running.", 65000);
            efficiency += 40;
            done = true;
        } else if (choice == 2) {
            Display::typewriter("  All processes restarted. Data lost. Fix works but messy.", 50000);
            efficiency += 20;
            done = true;
        } else if (choice == 3) {
            Display::glitch("  More memory cannot break a circular wait.");
            g_state->wrongMove();
        }
    }

    if (usedPs && usedResources) efficiency += 20;
    if (g_state->getStability() > 80) efficiency += 10;

    CaseScore cs = g_state->endCase(true, efficiency >= 90, efficiency);
    g_inventory->unlock(DEADLOCK);
    g_inventory->unlock(TERMINATOR);

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
