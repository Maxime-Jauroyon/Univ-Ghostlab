#ifndef CIMTUI_CIMTUI_H
#define CIMTUI_CIMTUI_H

#ifdef __cplusplus
#define CIMTUI_API extern "C"
#else
#define CIMTUI_API
#endif

CIMTUI_API void igTuiInit();
CIMTUI_API void igTuiShutdown();
CIMTUI_API bool igTuiNewFrame();
CIMTUI_API void igTuiRender();
CIMTUI_API bool igTuiProcessEvent();
CIMTUI_API void igTuiShowDemoWindow(bool*);

#endif /* CIMTUI_CIMTUI_H */
