#include "case_runner.h"

#include "display.h"
#include "gamestate.h"

using namespace std;

extern GameState* g_state;

namespace case_runner {

Context begin(int case_id, const std::string& hud_title, int total_steps, int extra_seconds) {
    g_state->beginCase(case_id);
    Context ctx;
    ctx.case_id       = case_id;
    ctx.total_steps   = total_steps;
    ctx.extra_seconds = extra_seconds;
    ctx.start         = time(NULL);
    ctx.hud_title     = hud_title;
    ctx.progress      = 0;
    return ctx;
}

int time_left(const Context& ctx) {
    int base = g_state->getConfig().caseTimer + ctx.extra_seconds;
    int t = base - (int)(time(NULL) - ctx.start);
    return t < 0 ? 0 : t;
}

bool should_abort(const Context& ctx) {
    return g_state->isCrashed() || time_left(ctx) <= 0;
}

void penalize(const std::string& reason) {
    Display::flashWarning(reason);
    g_state->wrongMove();
    Display::showStability(g_state->getStability());
}

void crash_ending(const std::string& title, const std::string& line) {
    Display::divider();
    Display::typewriter("  " + title, 65000);
    Display::divider();
    Display::typewriter("  " + line, 60000);
    Display::divider();
    Display::waitEnter();
}

void story_lines(const std::vector<std::string>& lines, int speed_us, int pause_ms) {
    for (const auto& s : lines) {
        Display::typewriter("  " + s, speed_us);
        if (pause_ms > 0) Display::pause(pause_ms);
    }
}

}  // namespace case_runner

