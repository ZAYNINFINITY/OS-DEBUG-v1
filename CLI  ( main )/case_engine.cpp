#include "case_engine.h"
#include <iostream>
using namespace std;

CaseEngine::CaseEngine(GameState* gs, int id, string title) {
    state = gs;
    caseId = id;
    caseTitle = title;
    diagnosticScore = 0;
    maxDiagnosticScore = 0;
    diagnosisMade = false;
    fixApplied = false;
    correctDiagnosis = "";
    correctFix = "";
}

void CaseEngine::addDiagnostic(string cmd, string output, int value) {
    maxDiagnosticScore += value;
}

void CaseEngine::addFix(string name, string desc, int eff, string result) {
}

void CaseEngine::setCorrectDiagnosis(string d) {
    correctDiagnosis = d;
}

void CaseEngine::setCorrectFix(string f) {
    correctFix = f;
}

bool CaseEngine::runDiagnostic(string cmd) {
    commandsUsed.push_back(cmd);
    return true;
}

bool CaseEngine::makeDiagnosis(string diag) {
    if (diag == correctDiagnosis) {
        diagnosisMade = true;
        return true;
    }
    state->wrongMove();
    return false;
}

bool CaseEngine::applyFix(string fix) {
    if (fix == correctFix) {
        fixApplied = true;
        return true;
    }
    state->wrongMove();
    return false;
}

int CaseEngine::getEfficiency() {
    int eff = 50;
    if (diagnosisMade) eff += 25;
    if (fixApplied) eff += 25;
    if (state->getStability() > 80) eff += 10;
    if (eff > 100) eff = 100;
    return eff;
}

void CaseEngine::showStatus() {
    cout << "\n  [" << caseTitle << "]\n";
    cout << "  Stability: " << state->getStability() << "%\n";
    cout << "  Commands used: " << commandsUsed.size() << "\n";
    if (diagnosisMade) cout << "  Diagnosis: COMPLETE\n";
    if (fixApplied) cout << "  Fix: APPLIED\n";
    cout << "\n";
}

bool CaseEngine::canDiagnose() {
    return commandsUsed.size() >= 2;
}

bool CaseEngine::canFix() {
    return diagnosisMade;
}

void CaseEngine::showHelp() {
    cout << "\n  DEBUG COMMANDS:\n";
    cout << "  ps          - Show process table\n";
    cout << "  resources   - Show resource graph\n";
    cout << "  analyze     - Run system analysis\n";
    cout << "  diagnose    - Submit diagnosis\n";
    cout << "  fix         - Apply fix\n";
    cout << "  help        - Show this help\n\n";
}

int CaseEngine::getCaseId() {
    return caseId;
}

