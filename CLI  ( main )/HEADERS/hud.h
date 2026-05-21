#ifndef HUD_H
#define HUD_H

#include <string>
using namespace std;

class HUD {
public:
    static void render(string title, int progress, int total, int stability, int timeLeft);
};

#endif

