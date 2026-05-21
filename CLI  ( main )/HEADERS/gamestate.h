#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "userprofile.h"
#include <ctime>
using namespace std;

enum class Ending { Perfect, Good, Messy, Failed };
enum class Difficulty { Trainee, Engineer, Senior };

struct DifficultyConfig {
    int caseTimer;
    int stabilityDrain;
    bool hintsEnabled;
    float scoreMultiplier;
};

struct CaseScore {
    int points = 0;
    int wrongMoves = 0;
    bool timeBonus = false;
    Ending ending = Ending::Failed;
    int elapsedSec = 0;
    int efficiency = 0;
};

class GameState {
private:
    int stability;
    int totalScore;
    int currentCase;
    int caseWrongs;
    time_t caseStart;
    Difficulty difficulty;
    CaseScore scores[8];
    UserProfile* user;
    time_t campaignStart;
    bool inCampaign;

public:
    GameState();
    void newGame(Difficulty d, UserProfile* u);
    void beginCase(int n);
    void wrongMove();
    bool isCrashed();
    int getStability();
    int getTotalScore();
    Difficulty getDifficulty();
    DifficultyConfig getConfig();
    void setStability(int pct);
    CaseScore endCase(bool solved, bool perfect, int efficiency);
    CaseScore getScore(int index);
    void beginCampaign();
    void endCampaign();
    int getCampaignTime();
    UserProfile* getUser();
};

#endif

