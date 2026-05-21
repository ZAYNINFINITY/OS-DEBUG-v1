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

void case3_memory() {
    Display::clear();
    Display::divider();
    Display::typewriter("  [ INCOMING TRANSMISSION -- Week 2 ]", 45000);
    Display::typewriter("  [ NOVACORP MAIN SERVER           ]", 45000);
    Display::divider();
    Display::pause(800);
    Display::typewriter("  Something is eating our memory.", 65000);
    Display::typewriter("  It started small. Nobody noticed.", 55000);
    Display::typewriter("  Tonight -- memory is at 4%.", 85000);
    Display::typewriter("  Something is still taking more. Find it.", 65000);
    Display::divider();
    Display::waitEnter();

    PageTable mem(10, 64);
    mem.allocate(4, 9);

    case_runner::Context ctx = case_runner::begin(3, "CASE 3: THE MEMORY THIEF");
    bool usedFree = false;
    bool usedMap = false;
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
        HUD::render("CASE 3: THE MEMORY THIEF", 0, 3, g_state->getStability(), case_runner::time_left(ctx));

        cout << "\n  DEBUG COMMANDS:\n";
        cout << "  [1] free     - Show memory usage\n";
        cout << "  [2] pmap     - Show process memory map\n";
        cout << "  [3] diagnose - Submit diagnosis\n";
        cout << "  [4] fix      - Apply fix\n";
        cout << "\n  > Choice: ";
        cout.flush();

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            usedFree = true;
            cout << "\n  Memory: " << mem.getFreePages() * 64 << " MB free of " << 10 * 64 << " MB\n";
            cout << "  P4 is using " << mem.getUsage(4) * 64 << " MB (was assigned 64 MB)\n\n";
            Display::typewriter("  P4 was given 64 MB. Using 576 MB. 9x its allocation.", 50000);
        } else if (choice == 2) {
            usedMap = true;
            mem.showMap();
            Display::typewriter("  P4 is holding 9 of 10 pages. Was assigned 1 page.", 50000);
        } else if (choice == 3) {
            if (!usedFree || !usedMap) {
                Display::typewriter("  Run diagnostics first.", 45000);
                g_state->wrongMove();
                continue;
            }
            cout << "\n  Enter diagnosis (memory/deadlock/starvation): ";
            string diag;
            getline(cin, diag);
            if (diag == "memory") {
                Display::typewriter("  Correct. MEMORY LEAK -- P4 accumulated pages.", 50000);
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
        HUD::render("CASE 3: THE MEMORY THIEF", 1, 3, g_state->getStability(), case_runner::time_left(ctx));

        cout << "\n  FIX OPTIONS:\n";
        cout << "  [1] reclaim compact  - Proper OS fix (best)\n";
        cout << "  [2] restart p4       - Temporary fix\n";
        cout << "  [3] add ram          - Hardware band-aid\n";
        cout << "\n  > Choice: ";
        cout.flush();

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            Display::typewriter("  Reclaiming P4's excess pages...", 50000);
            Display::loadingBar("", 80000);
            mem.free(4);
            mem.allocate(4, 1);
            Display::typewriter("  512 MB -> 64 MB. Memory restored.", 50000);
            efficiency += 40;
            done = true;
        } else if (choice == 2) {
            Display::typewriter("  P4 restarts. Memory freed. But leak is still in code.", 50000);
            efficiency += 20;
            done = true;
        } else if (choice == 3) {
            Display::glitch("  Hardware cannot fix a software memory leak.");
            g_state->wrongMove();
        }
    }

    if (usedFree && usedMap) efficiency += 20;
    if (g_state->getStability() > 80) efficiency += 10;

    CaseScore cs = g_state->endCase(true, efficiency >= 90, efficiency);
    g_inventory->unlock(MEMORY);
    g_inventory->unlock(COMPACTOR);

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
