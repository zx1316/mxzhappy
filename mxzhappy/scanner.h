#pragma once
#include <vector>
#include <shared_mutex>
#include "mxzhappy.h"

#define MXZHAPPY_YS_FILENAME L"YuanShen.exe"
#define MXZHAPPY_SR_FILENAME L"StarRail.exe"
#define MXZHAPPY_ZZZ_FILENAME L"ZenlessZoneZero.exe"

class Scanner {
private:
	std::vector<std::wstring> paths;
	std::atomic_int scanThreadCount;
	std::shared_mutex mutex;

	Scanner() = default;
	void scanRecursively(wchar_t* path);
	void scanThreadFunc(char root);

public:
	Scanner(const Scanner&) = delete;
	Scanner(Scanner&&) noexcept = delete;
	Scanner& operator=(const Scanner&) = delete;
	Scanner& operator=(Scanner&&) noexcept = delete;

	static Scanner& getInstance();
	const std::vector<std::wstring>& getPaths();
	std::shared_mutex& getMutex();
	MXZHAPPY_STATUS scan();
	MXZHAPPY_STATUS isScanning();
	size_t getPathCount();
	const wchar_t* getPath(size_t index);
};
