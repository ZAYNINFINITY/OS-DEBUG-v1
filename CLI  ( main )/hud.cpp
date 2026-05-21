#include "hud.h"
#include "display.h"
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

void HUD::render(string title, int progress, int total, int stability, int timeLeft) {
    Display::clear();
    int w = 54;

    Display::boxTop(w);
    Display::boxLine("  " + title, w);
    Display::boxSeparator(w);

    int filled = stability / 5;
    string bar = "";
    for (int i = 0; i < 20; i++) {
        if (i < filled) {
            if (stability > 60) bar += "=";
            else if (stability > 30) bar += "-";
            else bar += "!";
        } else {
            bar += " ";
        }
    }

    string label;
    if (stability > 60) label = "STABLE  ";
    else if (stability > 30) label = "WARNING ";
    else if (stability > 0) label = "CRITICAL";
    else label = "CRASHED ";

    ostringstream stab;
    stab << "  [" << label << "]  [" << bar << "]  " << stability << "%";
    Display::boxLine(stab.str(), w);

    int m = timeLeft / 60;
    int s = timeLeft % 60;
    ostringstream timer;
    timer << "  TIME LEFT  " << setw(2) << setfill('0') << m << ":" << setw(2) << setfill('0') << s;
    Display::boxLine(timer.str(), w);

    string prog = "  [";
    for (int i = 0; i < total; i++) {
        prog += (i < progress) ? "###" : "...";
        if (i < total - 1) prog += "|";
    }
    prog += "]";
    Display::boxLine(prog, w);

    Display::boxBottom(w);
    cout << "\n";
}

