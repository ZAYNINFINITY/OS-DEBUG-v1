#include "cases.h"
#include "display.h"
#include "hud.h"
#include "gamestate.h"
#include "case_runner.h"
#include <iostream>
using namespace std;

extern GameState* g_state;

void case7_process_lifecycle() {
    Display::clear();
    Display::divider();
    Display::typewriter("  [ ALERT -- THE ORPHAN FOREST ]", 45000);
    Display::typewriter("  [ NOVACORP COMPUTE CLUSTER   ]", 45000);
    Display::divider();
    Display::pause(800);
    Display::typewriter("  Process table is full of Z entries.", 60000);
    Display::pause(800);
    Display::typewriter("  Z means Zombie.", 70000);
    Display::pause(900);
    Display::typewriter("  Processes that finished -- but were never reaped.", 55000);
    Display::divider();
    Display::waitEnter();

    case_runner::Context ctx = case_runner::begin(7, "CASE 7: THE ORPHAN FOREST");
    int efficiency = 0;
    bool s1 = false, s2 = false, s3 = false;

    while (!s1) {
        if (case_runner::should_abort(ctx)) {
            CaseScore cs = g_state->endCase(false, false, efficiency);
            Display::divider();
            Display::typewriter("  CASE FAILED", 65000);
            if (g_state->isCrashed()) cout << "  Stability reached 0%.\n";
            else cout << "  Timer expired during lifecycle challenge.\n";
            cout << "  Score: " << cs.points << "   Efficiency: " << efficiency << "%\n";
            Display::waitEnter();
            return;
        }
        HUD::render("CASE 7: THE ORPHAN FOREST", 0, 3, g_state->getStability(), case_runner::time_left(ctx));
        cout << "\n  Correct process state sequence?\n";
        cout << "  [1] New->Ready->Running->Waiting->Ready->Terminated\n";
        cout << "  [2] Running->New->Zombie->Ready\n";
        cout << "  [3] Waiting->New only\n";
        cout << "\n  > Choice: ";
        cout.flush();
        int c; cin >> c; cin.ignore();
        if (c == 1) {
            Display::typewriter("  Correct. Five-state model.", 50000);
            s1 = true; efficiency += 20;
        } else {
            Display::glitch("  Wrong. Review the standard model.");
            g_state->wrongMove();
        }
    }

    while (!s2) {
        if (case_runner::should_abort(ctx)) {
            CaseScore cs = g_state->endCase(false, false, efficiency);
            Display::divider();
            Display::typewriter("  CASE FAILED", 65000);
            if (g_state->isCrashed()) cout << "  Stability reached 0%.\n";
            else cout << "  Timer expired during lifecycle challenge.\n";
            cout << "  Score: " << cs.points << "   Efficiency: " << efficiency << "%\n";
            Display::waitEnter();
            return;
        }
        HUD::render("CASE 7: THE ORPHAN FOREST", 1, 3, g_state->getStability(), case_runner::time_left(ctx));
        cout << "\n  Child finishes. What must parent do?\n";
        cout << "  [1] Ignore -- child cleans itself\n";
        cout << "  [2] Call wait() or waitpid()\n";
        cout << "  [3] Child calls wait()\n";
        cout << "\n  > Choice: ";
        cout.flush();
        int c; cin >> c; cin.ignore();
        if (c == 2) {
            Display::typewriter("  Correct. Parent must reap the zombie.", 50000);
            s2 = true; efficiency += 20;
        } else {
            Display::glitch("  Wrong. Parent must call wait().");
            g_state->wrongMove();
        }
    }

    while (!s3) {
        if (case_runner::should_abort(ctx)) {
            CaseScore cs = g_state->endCase(false, false, efficiency);
            Display::divider();
            Display::typewriter("  CASE FAILED", 65000);
            if (g_state->isCrashed()) cout << "  Stability reached 0%.\n";
            else cout << "  Timer expired during lifecycle challenge.\n";
            cout << "  Score: " << cs.points << "   Efficiency: " << efficiency << "%\n";
            Display::waitEnter();
            return;
        }
        HUD::render("CASE 7: THE ORPHAN FOREST", 2, 3, g_state->getStability(), case_runner::time_left(ctx));
        cout << "\n  Fix zombie buildup in production?\n";
        cout << "  [1] waitpid() with timeout + kill stale\n";
        cout << "  [2] Spawn unlimited replacements\n";
        cout << "  [3] Disable process table limit\n";
        cout << "\n  > Choice: ";
        cout.flush();
        int c; cin >> c; cin.ignore();
        if (c == 1) {
            Display::typewriter("  Fix applied. Process table cleared.", 50000);
            s3 = true; efficiency += 20;
        } else {
            Display::glitch("  Wrong. That makes it worse.");
            g_state->wrongMove();
        }
    }

    if (g_state->getStability() > 80) efficiency += 20;
    if (s1 && s2 && s3) efficiency += 20;

    CaseScore cs = g_state->endCase(true, efficiency >= 90, efficiency);
    Display::divider();
    cout << "  Score: " << cs.points << "   Efficiency: " << efficiency << "%\n";
    Display::waitEnter();
}
