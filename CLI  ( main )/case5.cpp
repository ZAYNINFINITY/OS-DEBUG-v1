#include "cases.h"
#include "display.h"
#include "hud.h"
#include "gamestate.h"
#include "case_runner.h"
#include <iostream>
using namespace std;

extern GameState* g_state;

void case5_syscalls() {
    Display::clear();
    Display::divider();
    Display::typewriter("  [ INCOMING ALERT -- 02:11 AM ]", 45000);
    Display::typewriter("  [ NOVACORP WORKER CLUSTER     ]", 45000);
    Display::divider();
    Display::pause(800);
    Display::typewriter("  Worker processes launch then vanish.", 55000);
    Display::typewriter("  The manager keeps spawning replacements.", 55000);
    Display::typewriter("  None finish. Process table is filling up.", 60000);
    Display::divider();
    Display::waitEnter();

    case_runner::Context ctx = case_runner::begin(5, "CASE 5: SYSCALL MAZE");
    bool usedStrace = false;
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
        HUD::render("CASE 5: SYSCALL MAZE", 0, 3, g_state->getStability(), case_runner::time_left(ctx));
        cout << "\n  DEBUG COMMANDS:\n";
        cout << "  [1] strace    - Trace syscalls\n";
        cout << "  [2] top       - Check CPU\n";
        cout << "  [3] diagnose  - Submit diagnosis\n";
        cout << "\n  > Choice: ";
        cout.flush();
        int choice; cin >> choice; cin.ignore();

        if (choice == 1) {
            usedStrace = true;
            cout << "\n  fork() = 4211\n";
            cout << "  [pid 4211] execve(\"/usr/bin/worker\", ...) = -1 ENOENT\n";
            cout << "  [pid 4211] exit(1)\n\n";
            Display::typewriter("  exec() failed. File not found at old path.", 50000);
        } else if (choice == 2) {
            Display::typewriter("  CPU normal. Not helpful here.", 45000);
        } else if (choice == 3) {
            if (!usedStrace) {
                Display::typewriter("  Need syscall evidence first.", 45000);
                g_state->wrongMove();
                continue;
            }
            cout << "\n  Enter diagnosis (syscall-path/scheduling/memory): ";
            string diag; getline(cin, diag);
            if (diag == "syscall-path") {
                Display::typewriter("  Correct. Broken exec path after file move.", 50000);
                efficiency += 30;
                break;
            } else {
                Display::glitch("  Wrong diagnosis.");
                g_state->wrongMove();
            }
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
        HUD::render("CASE 5: SYSCALL MAZE", 1, 3, g_state->getStability(), case_runner::time_left(ctx));
        cout << "\n  FIX OPTIONS:\n";
        cout << "  [1] validate-exec-wait  - Fix path + wait (best)\n";
        cout << "  [2] remove-wait         - Zombie risk\n";
        cout << "  [3] retry-forever       - Infinite loop\n";
        cout << "\n  > Choice: ";
        cout.flush();
        int choice; cin >> choice; cin.ignore();
        if (choice == 1) {
            Display::typewriter("  Patch applied. Path corrected. Children complete normally.", 50000);
            efficiency += 40;
            done = true;
        } else if (choice == 2) {
            Display::typewriter("  Removing wait() causes zombies.", 50000);
            efficiency += 20;
            done = true;
        } else if (choice == 3) {
            Display::glitch("  Retrying forever does not fix wrong path.");
            g_state->wrongMove();
        }
    }

    if (usedStrace) efficiency += 20;
    if (g_state->getStability() > 80) efficiency += 10;

    CaseScore cs = g_state->endCase(true, efficiency >= 90, efficiency);
    Display::divider();
    cout << "  Score: " << cs.points << "   Efficiency: " << efficiency << "%\n";
    Display::waitEnter();
}
