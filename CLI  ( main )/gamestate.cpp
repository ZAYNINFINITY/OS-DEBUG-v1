#include "gamestate.h"
#include <iostream>
using namespace std;

GameState::GameState() {
    stability = 100;
    totalScore = 0;
    currentCase = 0;
    caseWrongs = 0;
    caseStart = 0;
    difficulty = Difficulty::Engineer;
    user = nullptr;
    campaignStart = 0;
    inCampaign = false;
    for (int i = 0; i < 8; i++) {
        scores[i] = CaseScore();
    }
}

void GameState::newGame(Difficulty d, UserProfile* u) {
    stability = 100;
    totalScore = 0;
    currentCase = 0;
    caseWrongs = 0;
    difficulty = d;
    user = u;
    inCampaign = false;
    for (int i = 0; i < 8; i++) {
        scores[i] = CaseScore();
    }
}

void GameState::beginCase(int n) {
    currentCase = n;
    caseWrongs = 0;
    caseStart = time(NULL);
}

void GameState::wrongMove() {
    caseWrongs++;
    DifficultyConfig cfg = getConfig();
    stability -= cfg.stabilityDrain;
    if (stability < 0) stability = 0;
}

bool GameState::isCrashed() {
    return stability <= 0;
}

int GameState::getStability() {
    return stability;
}

int GameState::getTotalScore() {
    return totalScore;
}

Difficulty GameState::getDifficulty() {
    return difficulty;
}

DifficultyConfig GameState::getConfig() {
    DifficultyConfig cfg;
    if (difficulty == Difficulty::Trainee) {
        cfg.caseTimer = 240;
        cfg.stabilityDrain = 10;
        cfg.hintsEnabled = true;
        cfg.scoreMultiplier = 1.0;
    } else if (difficulty == Difficulty::Senior) {
        cfg.caseTimer = 120;
        cfg.stabilityDrain = 25;
        cfg.hintsEnabled = false;
        cfg.scoreMultiplier = 1.5;
    } else {
        cfg.caseTimer = 180;
        cfg.stabilityDrain = 15;
        cfg.hintsEnabled = false;
        cfg.scoreMultiplier = 1.2;
    }
    return cfg;
}

void GameState::setStability(int pct) {
    if (pct < 0) pct = 0;
    if (pct > 100) pct = 100;
    stability = pct;
}

CaseScore GameState::endCase(bool solved, bool perfect, int efficiency) {
    CaseScore cs;
    cs.wrongMoves = caseWrongs;
    cs.elapsedSec = (int)(time(NULL) - caseStart);
    cs.efficiency = efficiency;
    DifficultyConfig cfg = getConfig();

    if (!solved || isCrashed()) {
        cs.ending = Ending::Failed;
        cs.points = 0;
    } else {
        cs.points = 1000 - caseWrongs * 150;
        if (cs.points < 0) cs.points = 0;

        if (cs.elapsedSec <= 90) {
            cs.timeBonus = true;
            cs.points += 200;
        }

        cs.points = (int)(cs.points * cfg.scoreMultiplier);

        if (efficiency >= 90 && caseWrongs == 0) {
            cs.ending = Ending::Perfect;
            cs.points += 200;
        } else if (efficiency >= 70) {
            cs.ending = Ending::Good;
        } else if (efficiency >= 40) {
            cs.ending = Ending::Messy;
        } else {
            cs.ending = Ending::Failed;
            cs.points = 0;
        }
    }

    totalScore += cs.points;
    if (currentCase >= 1 && currentCase <= 8) {
        scores[currentCase - 1] = cs;
    }

    if (user != nullptr) {
        string endingStr;
        if (cs.ending == Ending::Perfect) endingStr = "Perfect";
        else if (cs.ending == Ending::Good) endingStr = "Good";
        else if (cs.ending == Ending::Messy) endingStr = "Messy";
        else endingStr = "Failed";
        user->addCaseResult(currentCase, cs.points, endingStr, stability, caseWrongs);
    }

    return cs;
}

CaseScore GameState::getScore(int index) {
    if (index < 0 || index > 7) return CaseScore();
    return scores[index];
}

void GameState::beginCampaign() {
    campaignStart = time(NULL);
    inCampaign = true;
}

void GameState::endCampaign() {
    inCampaign = false;
}

int GameState::getCampaignTime() {
    if (!inCampaign) return 0;
    return (int)(time(NULL) - campaignStart);
}

UserProfile* GameState::getUser() {
    return user;
}

