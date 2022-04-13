#ifndef GHOSTLAB_CIMTUI_H
#define GHOSTLAB_CIMTUI_H

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

#include "../../types.h"

//typedef struct tui_screen_t {
//    int32_t nx;
//    int32_t ny;
//    int32_t nmax;
//    uint32_t *data;
//} tui_screen_t;

EXTERNC void igTuiInit();
EXTERNC void igTuiShutdown();
EXTERNC bool igTuiNewFrame();
EXTERNC void igTuiDrawScreen();
EXTERNC bool igTuiProcessEvent();
//void igTuiScreenResize(tui_screen_t *screen, int32_t pnx, int32_t pny);
//void igTuiScreenClear(tui_screen_t *screen);
//uint32_t igTuiScreenSize(tui_screen_t *screen);

#endif /* GHOSTLAB_CIMTUI_H */
