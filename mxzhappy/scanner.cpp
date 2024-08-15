#include "pch.h"
#include "scanner.h"
#define _CRT_SECURE_NO_WARNINGS

Scanner& Scanner::getInstance() {
    static Scanner s;
    return s;
}

const std::vector<std::wstring>& Scanner::getPaths() {
    return paths;
}

std::shared_mutex& Scanner::getMutex() {
    return mutex;
}

MXZHAPPY_STATUS Scanner::scan() {
    if (isScanning()) {
        return MXZHAPPY_IS_SCANNING;
    }
    mutex.lock();
    paths.clear();
    mutex.unlock();
    DWORD drives = GetLogicalDrives();
    for (char ch = 'C' - 'A'; ch <= 'Z' - 'A'; ch++) {
        if ((drives >> ch) & 1) {
            scanThreadCount.fetch_add(1);
            std::thread thread(&Scanner::scanThreadFunc, this, ch + 'A');
            thread.detach();
        }
    }
    return MXZHAPPY_OK;
}

MXZHAPPY_STATUS Scanner::isScanning() {
    return scanThreadCount.load() != 0;
}

size_t Scanner::getPathCount() {
    std::shared_lock<std::shared_mutex> lock(mutex);
    return paths.size();
}

const wchar_t* Scanner::getPath(size_t index) {
    std::shared_lock<std::shared_mutex> lock(mutex);
    if (index < 0 || index >= paths.size()) {
        return nullptr;
    }
    return paths[index].c_str();
}

void Scanner::scanRecursively(wchar_t* path) {
    size_t pathLen = wcslen(path);
    path[pathLen] = '\\';
    path[pathLen + 1] = '*';
    path[pathLen + 2] = 0;
    WIN32_FIND_DATAW findFileData;
    HANDLE hFind = FindFirstFileW(path, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        return;
    }
    do {
        if (findFileData.cFileName[0] == '.' && (findFileData.cFileName[1] == 0 || (findFileData.cFileName[1] == '.' && findFileData.cFileName[2] == 0))) {
            continue;
        }
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            wcscpy(path + pathLen + 1, findFileData.cFileName);
            scanRecursively(path);
        } else if (wcscmp(findFileData.cFileName, MXZHAPPY_SR_FILENAME) == 0 || wcscmp(findFileData.cFileName, MXZHAPPY_YS_FILENAME) == 0 || wcscmp(findFileData.cFileName, MXZHAPPY_ZZZ_FILENAME) == 0) {
            wcscpy(path + pathLen + 1, findFileData.cFileName);
            std::unique_lock<std::shared_mutex> lock(mutex);
            paths.emplace_back(path);
        }
    } while (FindNextFileW(hFind, &findFileData) != 0);
    FindClose(hFind);
}

void Scanner::scanThreadFunc(char root) {
    wchar_t path[32768];    // max path len in windows is 32767
    path[0] = root;
    path[1] = ':';
    path[2] = 0;
    path[3] = 0;
    scanRecursively(path);
    scanThreadCount.fetch_sub(1);
}
