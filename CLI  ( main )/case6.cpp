#include "cases.h"
#include "display.h"
#include "hud.h"
#include "gamestate.h"
#include "case_runner.h"
#include <iostream>
using namespace std;

extern GameState* g_state;

void case6_sync() {
    Display::clear();
    Display::divider();
    Display::typewriter("  [ INCIDENT REPORT -- LOCKDOWN ]", 45000);
    Display::typewriter("  [ NOVACORP FINANCIAL LEDGER   ]", 45000);
    Display::divider();
    Display::pause(800);
    Display::typewriter("  The shared payment ledger is showing wrong totals.", 55000);
    Display::typewriter("  Two threads update balance simultaneously.", 55000);
    Display::typewriter("  One update is lost.", 65000);
    Display::pause(900);
    Display::typewriter("  This is a race condition.", 75000);
    Display::divider();
    Display::waitEnter();

    case_runner::Context ctx = case_runner::begin(6, "CASE 6: LOCKDOWN");
    int efficiency = 0;
    bool s1 = false, s2 = false, s3 = false;

    while (!s1) {
        if (case_runner::should_abort(ctx)) {
            CaseScore cs = g_state->endCase(false, false, efficiency);
            Display::divider();
            Display::typewriter("  CASE FAILED", 65000);
            if (g_state->isCrashed()) cout << "  Stability reached 0%.\n";
            else cout << "  Timer expired during synchronization challenge.\n";
            cout << "  Score: " << cs.points << "   Efficiency: " << efficiency << "%\n";
            Display::waitEnter();
            return;
        }
        HUD::render("CASE 6: LOCKDOWN", 0, 3, g_state->getStability(), case_runner::time_left(ctx));
        cout << "\n  Stage 1: 2-thread mutual exclusion solution?\n";
        cout << "  [1] Peterson   [2] Random sleep   [3] Disable thread\n";
        cout << "\n  > Choice: ";
        cout.flush();
        int c; cin >> c; cin.ignore();
        if (c == 1) {
            Display::typewriter("  Peterson's Solution. Correct.", 50000);
            s1 = true; efficiency += 20;
        } else {
            Display::glitch("  Wrong. Peterson guarantees mutual exclusion.");
            g_state->wrongMove();
        }
    }

    while (!s2) {
        if (case_runner::should_abort(ctx)) {
            CaseScore cs = g_state->endCase(false, false, efficiency);
            Display::divider();
            Display::typewriter("  CASE FAILED", 65000);
            if (g_state->isCrashed()) cout << "  Stability reached 0%.\n";
            else cout << "  Timer expired during synchronization challenge.\n";
            cout << "  Score: " << cs.points << "   Efficiency: " << efficiency << "%\n";
            Display::waitEnter();
            return;
        }
        HUD::render("CASE 6: LOCKDOWN", 1, 3, g_state->getStability(), case_runner::time_left(ctx));
        cout << "\n  Stage 2: Production primitive for critical section?\n";
        cout << "  [1] Mutex   [2] Busy wait   [3] Remove shared var\n";
        cout << "\n  > Choice: ";
        cout.flush();
        int c; cin >> c; cin.ignore();
        if (c == 1) {
            Display::typewriter("  Mutex applied. Race eliminated.", 50000);
            s2 = true; efficiency += 20;
        } else {
            Display::glitch("  Wrong. Mutex is the production solution.");
            g_state->wrongMove();
        }
    }

    while (!s3) {
        if (case_runner::should_abort(ctx)) {
            CaseScore cs = g_state->endCase(false, false, efficiency);
            Display::divider();
            Display::typewriter("  CASE FAILED", 65000);
            if (g_state->isCrashed()) cout << "  Stability reached 0%.\n";
            else cout << "  Timer expired during synchronization challenge.\n";
            cout << "  Score: " << cs.points << "   Efficiency: " << efficiency << "%\n";
            Display::waitEnter();
            return;
        }
        HUD::render("CASE 6: LOCKDOWN", 2, 3, g_state->getStability(), case_runner::time_left(ctx));
        cout << "\n  Stage 3: Control 5-connection pool?\n";
        cout << "  [1] Counting semaphore   [2] Binary mutex   [3] No primitive\n";
        cout << "\n  > Choice: ";
        cout.flush();
        int c; cin >> c; cin.ignore();
        if (c == 1) {
            Display::typewriter("  Counting semaphore. Correct.", 50000);
            s3 = true; efficiency += 20;
        } else {
            Display::glitch("  Wrong. Need counting semaphore for N slots.");
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
