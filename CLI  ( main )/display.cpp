#include "display.h"
#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <limits>
using namespace std;

static bool g_fastMode = false;

void Display::clear() {
    system(
#ifdef _WIN32
        "cls"
#else
        "clear"
#endif
    );
}

void Display::setFastMode(bool enabled) {
    g_fastMode = enabled;
}

bool Display::isFastMode() {
    return g_fastMode;
}

void Display::pause(int ms) {
    if (g_fastMode || ms <= 0) return;
    this_thread::sleep_for(chrono::milliseconds(ms));
}

void Display::typewriter(string text, int speed) {
    for (char c : text) {
        cout << c;
        cout.flush();
        if (!g_fastMode && speed > 0) {
            this_thread::sleep_for(chrono::microseconds(speed));
        }
    }
    cout << "\n";
}

void Display::loadingBar(string label, int step) {
    if (!label.empty()) cout << label << "\n";
    cout << "  [";
    for (int i = 0; i < 30; i++) {
        cout << "=";
        cout.flush();
        if (!g_fastMode && step > 0) {
            this_thread::sleep_for(chrono::microseconds(step));
        }
    }
    cout << "] DONE\n\n";
}

void Display::divider() {
    cout << "\n  ";
    for (int i = 0; i < 56; i++) cout << "~";
    cout << "\n\n";
}

void Display::thin_divider() {
    cout << "  ";
    for (int i = 0; i < 56; i++) cout << "-";
    cout << "\n";
}

void Display::boxTop(int w) {
    cout << "\n  +";
    for (int i = 0; i < w; i++) cout << "=";
    cout << "+\n";
}

void Display::boxBottom(int w) {
    cout << "  +";
    for (int i = 0; i < w; i++) cout << "=";
    cout << "+\n\n";
}

void Display::boxLine(string text, int w) {
    cout << "  |  " << text;
    int pad = w - 2 - (int)text.size();
    if (pad < 0) pad = 0;
    for (int i = 0; i < pad; i++) cout << " ";
    cout << "|\n";
}

void Display::boxSeparator(int w) {
    cout << "  +";
    for (int i = 0; i < w; i++) cout << "-";
    cout << "+\n";
}

void Display::showStability(int p) {
    if (p < 0) p = 0;
    if (p > 100) p = 100;
    int f = p / 5;
    string bar = "";
    for (int i = 0; i < 20; i++) {
        if (i < f) { if (p > 60) bar += "="; else if (p > 30) bar += "-"; else bar += "!"; }
        else bar += " ";
    }
    string l = p > 60 ? "STABLE  " : p > 30 ? "WARNING " : p > 0 ? "CRITICAL" : "CRASHED ";
    cout << "\r  [" << l << "] [" << bar << "] " << p << "%";
    cout.flush();
}

void Display::animate_stability_drop(int from, int to) {
    if (to < 0) to = 0;
    for (int p = from; p >= to; p--) {
        showStability(p);
        if (!g_fastMode) {
            this_thread::sleep_for(chrono::microseconds(10000));
        }
    }
    cout << "\n\n";
}

void Display::glitch(string text) {
    string noise = "!@#$%^&*?><~";
    int len = (int)text.size();
    for (int f = 0; f < 4; f++) {
        cout << "\r  >> ";
        for (int i = 0; i < len; i++) cout << noise[rand() % 12];
        cout.flush();
        if (!g_fastMode) {
            this_thread::sleep_for(chrono::microseconds(50000));
        }
    }
    cout << "\r  >> ";
    typewriter(text, 35000);
}

void Display::flashWarning(string msg) {
    for (int i = 0; i < 2; i++) {
        clear();
        cout << "\n\n\n";
        cout << "  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
        cout << "  !!                                                 !!\n";
        cout << "  !!  WRONG MOVE : " << msg;
        int pad = 34 - (int)msg.size();
        if (pad < 0) pad = 0;
        for (int p = 0; p < pad; p++) cout << " ";
        cout << "!!\n";
        cout << "  !!                                                 !!\n";
        cout << "  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
        cout.flush();
        pause(350);
        clear();
        pause(200);
    }
}

void Display::waitEnter() {
    cout << "\n  >> Press ENTER to continue...";
    cout.flush();
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void Display::showTitle() {
    clear();
    cout << "\n";
    cout << "  +============================================================+\n";
    cout << "  |                                                            |\n";
    cout << "  |       O S   F A U L T   D E T E C T I V E                 |\n";
    cout << "  |   Real-time OS Incident Simulator  --  CLI Edition         |\n";
    cout << "  |   8 Cases: Deadlock | Starvation | Memory | IPC            |\n";
    cout << "  |                                                            |\n";
    cout << "  +============================================================+\n\n";
    pause(600);
}

void Display::showMenuHeader(string user, int score, int cases) {
    cout << "\n  ========================================\n";
    cout << "        OS FAULT DETECTIVE v2.0\n";
    cout << "  ========================================\n";
    cout << "  User: " << user << "\n";
    cout << "  Score: " << score << "  |  Cases Solved: " << cases << "/8\n";
    cout << "  ========================================\n\n";
}

void Display::matrix_rain() {
    clear();
    srand((unsigned)time(NULL));
    for (int r = 0; r < 8; r++) {
        cout << "  ";
        for (int c = 0; c < 56; c++) {
            if (rand() % 3 == 0) cout << " ";
            else cout << (rand() % 10);
            cout.flush();
            if (!g_fastMode) {
                this_thread::sleep_for(chrono::microseconds(1800));
            }
        }
        cout << "\n";
        pause(20);
    }
    pause(200);
    clear();
}

void Display::center(string text, int width) {
    int pad = (width - (int)text.size()) / 2;
    if (pad < 0) pad = 0;
    for (int i = 0; i < pad; i++) cout << " ";
    cout << text << "\n";
}

void Display::draw_gantt_bar(int pid, int units, int delay_us) {
    int drawn = (units > 24) ? 24 : units;
    cout << "  P" << pid << " |";
    cout.flush();
    for (int i = 0; i < drawn; i++) {
        cout << "#";
        cout.flush();
        if (!g_fastMode && delay_us > 0) {
            this_thread::sleep_for(chrono::microseconds(delay_us));
        }
    }
    if (units > 24) cout << "...(" << units << ")";
    cout << "|\n";
}

void Display::draw_memory_map(int used, int total, int pid) {
    cout << "\n";
    thin_divider();
    cout << "  MEMORY MAP  (" << total * 64 << " MB total)\n\n  [";
    for (int i = 0; i < total; i++) cout << (i < used ? "##" : "..");
    cout << "]\n\n";
    cout << "  P" << pid << "  Used: " << (used * 64) << " MB  |  Allowed: 64 MB\n";
    thin_divider();
    cout << "\n";
}

void Display::show_os_concept_banner(string topic) {
    cout << "\n  +================================================+\n";
    cout << "  |  OS CONCEPT: " << topic;
    int pad = 34 - (int)topic.size();
    if (pad < 0) pad = 0;
    for (int i = 0; i < pad; i++) cout << " ";
    cout << "|\n  +================================================+\n\n";
    pause(500);
}

void Display::show_case_briefing_header(int n, string title) {
    clear();
    int w = 56;
    boxTop(w);
    boxLine("  CASE " + to_string(n) + " -- " + title, w);
    boxSeparator(w);
    string statuses[] = {
        "STATUS: CRITICAL  |  Deadlock detected",
        "STATUS: CRITICAL  |  Process starvation",
        "STATUS: HIGH      |  Memory leak found",
        "STATUS: CRITICAL  |  System cascade failure",
        "STATUS: HIGH      |  Syscall path broken",
        "STATUS: CRITICAL  |  Race condition active",
        "STATUS: HIGH      |  Zombie growth observed",
        "STATUS: CRITICAL  |  Pipeline outage detected"
    };
    int idx = n - 1;
    if (idx < 0 || idx >= 8) idx = 0;
    boxLine(statuses[idx], w);
    boxSeparator(w);
    boxLine("  Investigating NovaCorp failure...", w);
    boxBottom(w);
    pause(600);
}
