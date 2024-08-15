#include "pch.h"
#include "launcher.h"
#include "scanner.h"

Launcher::Launcher() : basicRate(0.006), rateInc(0.06), incBegin(75), maxAttempts(90), attempts(0) {
    std::random_device rd;
    generator.seed(rd());   // set the seed from random device
}

void Launcher::setParams(double newBasicRate, double newRateInc, int newIncBegin, int newMaxAttempts) {
    basicRate = newBasicRate;
    rateInc = newRateInc;
    incBegin = newIncBegin;
    maxAttempts = newMaxAttempts;
}

void Launcher::resetAttempts() {
    attempts = 0;
}

MXZHAPPY_STATUS Launcher::directLaunch(MXZHAPPY_GAME_TYPE type) {
    auto& scanner = Scanner::getInstance();
    auto& paths = scanner.getPaths();
    std::vector<std::wstring>::const_iterator it;
    std::shared_lock<std::shared_mutex> lock(scanner.getMutex());
    if (type == MXZHAPPY_YS) {
        it = std::find_if(paths.cbegin(), paths.cend(), [](const auto& path) {
            return path.ends_with(MXZHAPPY_YS_FILENAME);
        });
    } else if (type == MXZHAPPY_SR) {
        it = std::find_if(paths.cbegin(), paths.cend(), [](const auto& path) {
            return path.ends_with(MXZHAPPY_SR_FILENAME);
        });
    } else if (type == MXZHAPPY_ZZZ) {
        it = std::find_if(paths.cbegin(), paths.cend(), [](const auto& path) {
            return path.ends_with(MXZHAPPY_ZZZ_FILENAME);
        });
    } else {
        if (paths.empty()) {
            it = paths.cend();
        } else {
            std::uniform_int_distribution<size_t> distribution(0, paths.size() - 1);
            it = paths.begin() + distribution(generator);
        }
    }
    if (it == paths.cend()) {
        return MXZHAPPY_NO_SUCH_GAME;
    }
    SHELLEXECUTEINFOW se;
    ZeroMemory(&se, sizeof(SHELLEXECUTEINFOW));
    se.cbSize = sizeof(SHELLEXECUTEINFOW);
    se.lpVerb = L"runas";
    se.lpFile = it->c_str();
    se.lpParameters = L"Administrator";
    se.nShow = SW_NORMAL;
    se.fMask = SEE_MASK_NOCLOSEPROCESS;
    ShellExecuteExW(&se);
    CloseHandle(se.hProcess);
    return MXZHAPPY_OK;
}

MXZHAPPY_STATUS Launcher::gachaLaunch(MXZHAPPY_GAME_TYPE type) {
    attempts++;
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    if (attempts >= maxAttempts || (attempts >= incBegin && distribution(generator) <= basicRate + (attempts - incBegin + 1) * rateInc) || distribution(generator) <= basicRate) {
        attempts = 0;
        return directLaunch(type);
    }
    return MXZHAPPY_BAD_LUCK;
}
