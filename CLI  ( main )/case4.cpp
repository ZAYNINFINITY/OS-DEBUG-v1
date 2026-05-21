#include "cases.h"
#include "display.h"
#include "hud.h"
#include "inventory.h"
#include "gamestate.h"
#include "debugger.h"
#include "algorithms.h"
#include "case_runner.h"
#include <iostream>
using namespace std;

extern GameState* g_state;
extern Inventory* g_inventory;

void case4_boss() {
    Display::clear();
    Display::divider();
    Display::typewriter("  [ FINAL TRANSMISSION ]", 45000);
    Display::typewriter("  [ NOVACORP -- ALL SYSTEMS ]", 45000);
    Display::divider();
    Display::pause(800);
    Display::typewriter("  You have been here three times.", 60000);
    Display::typewriter("  Three cases. Three fixes.", 60000);
    Display::typewriter("  Tonight -- everything came back.", 80000);
    Display::pause(1000);
    Display::glitch("  The deadlock.");
    Display::glitch("  The starvation.");
    Display::glitch("  The memory leak.");
    Display::pause(900);
    Display::typewriter("  All at once. Because they were never separate.", 60000);
    Display::typewriter("  Fix the root. Or watch NovaCorp fall.", 70000);
    Display::divider();
    Display::waitEnter();

    case_runner::Context ctx = case_runner::begin(4, "CASE 4: ROOT CAUSE BOSS", 4, 60);
    int efficiency = 0;
    bool stage1 = false, stage2 = false, stage3 = false, stage4 = false;

    // Stage 1: Deadlock
    while (!stage1) {
        if (case_runner::should_abort(ctx)) {
            CaseScore cs = g_state->endCase(false, false, efficiency);
            Display::divider();
            Display::typewriter("  CASE FAILED", 65000);
            if (g_state->isCrashed()) cout << "  Stability reached 0%.\n";
            else cout << "  Timer expired during boss encounter.\n";
            cout << "  Score: " << cs.points << "   Efficiency: " << efficiency << "%\n";
            Display::waitEnter();
            return;
        }
        HUD::render("CASE 4: ROOT CAUSE BOSS", 0, 4, g_state->getStability(), case_runner::time_left(ctx));
        cout << "\n  STAGE 1 -- DEADLOCK\n";
        cout << "  [1] Detect deadlock\n";
        cout << "  [2] Fix deadlock\n";
        cout << "\n  > Choice: ";
        cout.flush();
        int c; cin >> c; cin.ignore();
        if (c == 1) {
            cout << "\n  Circular wait: P1->P2->P3->P1 confirmed.\n\n";
            stage1 = true;
            efficiency += 15;
        } else if (c == 2) {
            Display::typewriter("  Detect first.", 45000);
        }
    }

    while (stage1 && !stage2) {
        if (case_runner::should_abort(ctx)) {
            CaseScore cs = g_state->endCase(false, false, efficiency);
            Display::divider();
            Display::typewriter("  CASE FAILED", 65000);
            if (g_state->isCrashed()) cout << "  Stability reached 0%.\n";
            else cout << "  Timer expired during boss encounter.\n";
            cout << "  Score: " << cs.points << "   Efficiency: " << efficiency << "%\n";
            Display::waitEnter();
            return;
        }
        HUD::render("CASE 4: ROOT CAUSE BOSS", 1, 4, g_state->getStability(), case_runner::time_left(ctx));
        cout << "\n  STAGE 2 -- STARVATION\n";
        cout << "  [1] Detect starvation\n";
        cout << "  [2] Apply aging\n";
        cout << "\n  > Choice: ";
        cout.flush();
        int c; cin >> c; cin.ignore();
        if (c == 1) {
            cout << "\n  Low priority process ignored 72h confirmed.\n\n";
            stage2 = true;
            efficiency += 15;
        } else if (c == 2) {
            Display::typewriter("  Detect first.", 45000);
        }
    }

    while (stage2 && !stage3) {
        if (case_runner::should_abort(ctx)) {
            CaseScore cs = g_state->endCase(false, false, efficiency);
            Display::divider();
            Display::typewriter("  CASE FAILED", 65000);
            if (g_state->isCrashed()) cout << "  Stability reached 0%.\n";
            else cout << "  Timer expired during boss encounter.\n";
            cout << "  Score: " << cs.points << "   Efficiency: " << efficiency << "%\n";
            Display::waitEnter();
            return;
        }
        HUD::render("CASE 4: ROOT CAUSE BOSS", 2, 4, g_state->getStability(), case_runner::time_left(ctx));
        cout << "\n  STAGE 3 -- MEMORY LEAK\n";
        cout << "  [1] Scan memory\n";
        cout << "  [2] Compact memory\n";
        cout << "\n  > Choice: ";
        cout.flush();
        int c; cin >> c; cin.ignore();
        if (c == 1) {
            cout << "\n  P4 holding 8x allocation confirmed.\n\n";
            stage3 = true;
            efficiency += 15;
        } else if (c == 2) {
            Display::typewriter("  Scan first.", 45000);
        }
    }

    while (stage3 && !stage4) {
        if (case_runner::should_abort(ctx)) {
            CaseScore cs = g_state->endCase(false, false, efficiency);
            Display::divider();
            Display::typewriter("  CASE FAILED", 65000);
            if (g_state->isCrashed()) cout << "  Stability reached 0%.\n";
            else cout << "  Timer expired during boss encounter.\n";
            cout << "  Score: " << cs.points << "   Efficiency: " << efficiency << "%\n";
            Display::waitEnter();
            return;
        }
        HUD::render("CASE 4: ROOT CAUSE BOSS", 3, 4, g_state->getStability(), case_runner::time_left(ctx));
        cout << "\n  STAGE 4 -- ROOT CAUSE\n";
        cout << "  Which algorithm caused this cascade?\n";
        cout << "  [1] FCFS         [2] SJF\n";
        cout << "  [3] Round Robin  [4] Priority\n";
        cout << "\n  > Choice: ";
        cout.flush();
        int c; cin >> c; cin.ignore();
        if (c == 3) {
            Algorithms::roundRobin(4);
            Display::typewriter("  Round Robin. Correct. Fair for mixed workloads.", 50000);
            efficiency += 35;
            stage4 = true;
        } else if (c == 1 || c == 4) {
            Display::glitch("  That algorithm caused this problem.");
            g_state->wrongMove();
        } else if (c == 2) {
            Algorithms::sjf();
            Display::typewriter("  SJF can still starve. Not ideal.", 50000);
            efficiency += 15;
            stage4 = true;
        }
    }

    if (g_state->getStability() > 80) efficiency += 20;

    CaseScore cs = g_state->endCase(true, efficiency >= 90, efficiency);

    Display::clear();
    Display::divider();
    if (cs.ending == Ending::Perfect) {
        Display::typewriter("  ENDING: THE CLEAN SYSTEM", 70000);
        Display::typewriter("  NovaCorp never crashed again.", 60000);
    } else if (cs.ending == Ending::Good) {
        Display::typewriter("  ENDING: THE SCARRED SYSTEM", 70000);
    } else {
        Display::typewriter("  ENDING: THE FRAGILE PEACE", 70000);
    }
    Display::divider();
    cout << "  FINAL SCORE: " << g_state->getTotalScore() << "\n";
    cout << "  STABILITY: " << g_state->getStability() << "%\n";
    cout << "  EFFICIENCY: " << efficiency << "%\n\n";
    Display::waitEnter();
}
