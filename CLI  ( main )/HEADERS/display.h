#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
using namespace std;

class Display {
public:
    static void clear();
    static void setFastMode(bool enabled);
    static bool isFastMode();
    static void pause(int ms);
    static void typewriter(string text, int speed = 45000);
    static void loadingBar(string label, int step = 50000);
    static void divider();
    static void thin_divider();
    static void boxTop(int w = 54);
    static void boxBottom(int w = 54);
    static void boxLine(string text, int w = 54);
    static void boxSeparator(int w = 54);
    static void showStability(int p);
    static void animate_stability_drop(int from, int to);
    static void glitch(string text);
    static void flashWarning(string msg);
    static void waitEnter();
    static void showTitle();
    static void showMenuHeader(string user, int score, int cases);
    static void matrix_rain();
    static void center(string text, int width = 80);
    static void draw_gantt_bar(int pid, int units, int delay_us = 90000);
    static void draw_memory_map(int used, int total, int pid);
    static void show_os_concept_banner(string topic);
    static void show_case_briefing_header(int case_num, string title);
};

#endif
