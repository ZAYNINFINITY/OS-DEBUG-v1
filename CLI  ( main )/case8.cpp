#include "cases.h"
#include "display.h"
#include "hud.h"
#include "gamestate.h"
#include "case_runner.h"
#include <iostream>
using namespace std;

extern GameState* g_state;

void case8_client_server_pipeline() {
    Display::clear();
    Display::divider();
    Display::typewriter("  [ FINAL ALERT -- BROKEN PIPELINE ]", 45000);
    Display::typewriter("  [ NOVACORP API GATEWAY           ]", 45000);
    Display::divider();
    Display::pause(800);
    Display::typewriter("  Clients are timing out.", 65000);
    Display::pause(800);
    Display::typewriter("  The API gateway queues requests.", 55000);
    Display::typewriter("  The processing pipeline is not draining.", 55000);
    Display::pause(900);
    Display::typewriter("  Fix the flow. Restore the API.", 65000);
    Display::divider();
    Display::waitEnter();

    case_runner::Context ctx = case_runner::begin(8, "CASE 8: BROKEN PIPELINE");
    int efficiency = 0;
    bool s1 = false, s2 = false, s3 = false;

    while (!s1) {
        if (case_runner::should_abort(ctx)) {
            CaseScore cs = g_state->endCase(false, false, efficiency);
            Display::divider();
            Display::typewriter("  CASE FAILED", 65000);
            if (g_state->isCrashed()) cout << "  Stability reached 0%.\n";
            else cout << "  Timer expired during pipeline recovery.\n";
            cout << "  Score: " << cs.points << "   Efficiency: " << efficiency << "%\n";
            Display::waitEnter();
            return;
        }
        HUD::render("CASE 8: BROKEN PIPELINE", 0, 3, g_state->getStability(), case_runner::time_left(ctx));
        cout << "\n  In client-server, who initiates?\n";
        cout << "  [1] Server pushes    [2] Client requests\n";
        cout << "  [3] Third-party broker\n";
        cout << "\n  > Choice: ";
        cout.flush();
        int c; cin >> c; cin.ignore();
        if (c == 2) {
            Display::typewriter("  Correct. Client sends request, server responds.", 50000);
            s1 = true; efficiency += 20;
        } else {
            Display::glitch("  Wrong. Client initiates in request-response.");
            g_state->wrongMove();
        }
    }

    while (!s2) {
        if (case_runner::should_abort(ctx)) {
            CaseScore cs = g_state->endCase(false, false, efficiency);
            Display::divider();
            Display::typewriter("  CASE FAILED", 65000);
            if (g_state->isCrashed()) cout << "  Stability reached 0%.\n";
            else cout << "  Timer expired during pipeline recovery.\n";
            cout << "  Score: " << cs.points << "   Efficiency: " << efficiency << "%\n";
            Display::waitEnter();
            return;
        }
        HUD::render("CASE 8: BROKEN PIPELINE", 1, 3, g_state->getStability(), case_runner::time_left(ctx));
        cout << "\n  What does UNIX pipe do?\n";
        cout << "  [1] stdout(A) -> stdin(B)\n";
        cout << "  [2] Copies files\n";
        cout << "  [3] CPU scheduling\n";
        cout << "\n  > Choice: ";
        cout.flush();
        int c; cin >> c; cin.ignore();
        if (c == 1) {
            Display::typewriter("  Correct. Pipe connects stdout to stdin.", 50000);
            s2 = true; efficiency += 20;
        } else {
            Display::glitch("  Wrong. Pipe is stdout->stdin chaining.");
            g_state->wrongMove();
        }
    }

    while (!s3) {
        if (case_runner::should_abort(ctx)) {
            CaseScore cs = g_state->endCase(false, false, efficiency);
            Display::divider();
            Display::typewriter("  CASE FAILED", 65000);
            if (g_state->isCrashed()) cout << "  Stability reached 0%.\n";
            else cout << "  Timer expired during pipeline recovery.\n";
            cout << "  Score: " << cs.points << "   Efficiency: " << efficiency << "%\n";
            Display::waitEnter();
            return;
        }
        HUD::render("CASE 8: BROKEN PIPELINE", 2, 3, g_state->getStability(), case_runner::time_left(ctx));
        cout << "\n  Pipeline overwhelmed. Correct fix?\n";
        cout << "  [1] Backpressure + bounded queue\n";
        cout << "  [2] Disable retries\n";
        cout << "  [3] One worker per request\n";
        cout << "\n  > Choice: ";
        cout.flush();
        int c; cin >> c; cin.ignore();
        if (c == 1) {
            Display::typewriter("  Fix applied. Queue bounded. API latency normal.", 50000);
            s3 = true; efficiency += 20;
        } else {
            Display::glitch("  Wrong. That breaks clients or causes thundering herd.");
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
