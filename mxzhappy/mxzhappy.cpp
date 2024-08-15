#include "pch.h"
#include "mxzhappy.h"
#include "scanner.h"
#include "launcher.h"

MXZHAPPY_STATUS mxzhappy_scan() {
    return Scanner::getInstance().scan();
}

MXZHAPPY_STATUS mxzhappy_is_scanning() {
    return Scanner::getInstance().isScanning();
}

size_t mxzhappy_get_path_count() {
    return Scanner::getInstance().getPathCount();
}

const wchar_t* mxzhappy_get_path(size_t index) {
    return Scanner::getInstance().getPath(index);
}

MXZHAPPY_LAUNCHER_CTX mxzhappy_create_launcher() {
    return (MXZHAPPY_LAUNCHER_CTX) new Launcher;
}

void mxzhappy_destroy_launcher(MXZHAPPY_LAUNCHER_CTX ctx) {
    delete (Launcher*) ctx;
}

void mxzhappy_set_gacha_paras(MXZHAPPY_LAUNCHER_CTX ctx, double basic_rate, double rate_inc, int inc_begin, int max_attemps) {
    ((Launcher*) ctx)->setParams(basic_rate, rate_inc, inc_begin, max_attemps);
}

void mxzhappy_reset_attempts(MXZHAPPY_LAUNCHER_CTX ctx) {
    ((Launcher*) ctx)->resetAttempts();
}

MXZHAPPY_STATUS mxzhappy_direct_launch(MXZHAPPY_LAUNCHER_CTX ctx, MXZHAPPY_GAME_TYPE type) {
    return ((Launcher*) ctx)->directLaunch(type);
}

MXZHAPPY_STATUS mxzhappy_gacha_launch(MXZHAPPY_LAUNCHER_CTX ctx, MXZHAPPY_GAME_TYPE type) {
    return ((Launcher*) ctx)->gachaLaunch(type);
}
