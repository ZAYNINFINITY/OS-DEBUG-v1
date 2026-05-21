#pragma once

#include <ctime>
#include <string>
#include <vector>

namespace case_runner {

struct Context {
    int   case_id       = 0;
    int   total_steps   = 3;
    int   progress      = 0;
    int   extra_seconds = 0;     // e.g. boss fight gets extra time
    time_t start        = 0;
    std::string hud_title;
};

Context begin(int case_id, const std::string& hud_title, int total_steps = 3, int extra_seconds = 0);
int     time_left(const Context& ctx);
bool    should_abort(const Context& ctx);

void penalize(const std::string& reason);
void crash_ending(const std::string& title, const std::string& line);

void story_lines(const std::vector<std::string>& lines, int speed_us = 50000, int pause_ms = 0);

}  // namespace case_runner

