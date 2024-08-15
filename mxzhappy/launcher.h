#pragma once
#include <random>
#include "mxzhappy.h"

class Launcher {
private:
    std::mt19937 generator;
    double basicRate;
    double rateInc;
    int incBegin;
    int maxAttempts;
    int attempts;

public:
    Launcher();
    void setParams(double newBasicRate, double newRateInc, int newIncBegin, int newMaxAttempts);
    void resetAttempts();
    MXZHAPPY_STATUS directLaunch(MXZHAPPY_GAME_TYPE type);
    MXZHAPPY_STATUS gachaLaunch(MXZHAPPY_GAME_TYPE type);
};
