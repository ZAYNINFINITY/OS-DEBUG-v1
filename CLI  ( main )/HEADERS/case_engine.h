#ifndef CASE_ENGINE_H
#define CASE_ENGINE_H

#include "gamestate.h"
#include "debugger.h"
#include <vector>
#include <string>
using namespace std;

struct DiagnosticCommand {
    string name;
    string description;
    string output;
    int infoValue;
};

struct FixOption {
    string name;
    string description;
    int efficiency;
    string resultText;
};

class CaseEngine {
private:
    GameState* state;
    vector<string> commandsUsed;
    int diagnosticScore;
    int maxDiagnosticScore;
    string caseTitle;
    int caseId;
    bool diagnosisMade;
    bool fixApplied;
    string correctDiagnosis;
    string correctFix;

public:
    CaseEngine(GameState* gs, int id, string title);
    void addDiagnostic(string cmd, string output, int value);
    void addFix(string name, string desc, int eff, string result);
    void setCorrectDiagnosis(string d);
    void setCorrectFix(string f);
    bool runDiagnostic(string cmd);
    bool makeDiagnosis(string diag);
    bool applyFix(string fix);
    int getEfficiency();
    void showStatus();
    bool canDiagnose();
    bool canFix();
    void showHelp();
    int getCaseId();
};

#endif

