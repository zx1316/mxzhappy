#pragma once
#ifdef MXZHAPPY_EXPORTS
#define MXZHAPPY_API __declspec(dllexport)
#else
#define MXZHAPPY_API __declspec(dllimport)
#endif

#define MXZHAPPY_RANDOM 0
#define MXZHAPPY_YS 1
#define MXZHAPPY_SR 2
#define MXZHAPPY_ZZZ 3

#define MXZHAPPY_OK 0
#define MXZHAPPY_IS_SCANNING 1
#define MXZHAPPY_NO_SUCH_GAME 2
#define MXZHAPPY_BAD_LUCK 3

typedef void* MXZHAPPY_LAUNCHER_CTX;
typedef int MXZHAPPY_GAME_TYPE, MXZHAPPY_STATUS;

extern "C" MXZHAPPY_API MXZHAPPY_STATUS mxzhappy_scan();							// Scan mhy games in the computer asynchronously
extern "C" MXZHAPPY_API MXZHAPPY_STATUS mxzhappy_is_scanning();
extern "C" MXZHAPPY_API size_t mxzhappy_get_path_count();
extern "C" MXZHAPPY_API const wchar_t* mxzhappy_get_path(size_t index);
extern "C" MXZHAPPY_API MXZHAPPY_LAUNCHER_CTX mxzhappy_create_launcher();			// Create a launcher with default mhy gacha params
extern "C" MXZHAPPY_API void mxzhappy_destroy_launcher(MXZHAPPY_LAUNCHER_CTX ctx);
extern "C" MXZHAPPY_API void mxzhappy_set_gacha_paras(MXZHAPPY_LAUNCHER_CTX ctx, double basic_rate, double rate_inc, int inc_begin, int max_attemps);
extern "C" MXZHAPPY_API void mxzhappy_reset_attempts(MXZHAPPY_LAUNCHER_CTX ctx);
extern "C" MXZHAPPY_API MXZHAPPY_STATUS mxzhappy_direct_launch(MXZHAPPY_LAUNCHER_CTX ctx, MXZHAPPY_GAME_TYPE type);
extern "C" MXZHAPPY_API MXZHAPPY_STATUS mxzhappy_gacha_launch(MXZHAPPY_LAUNCHER_CTX ctx, MXZHAPPY_GAME_TYPE type);
