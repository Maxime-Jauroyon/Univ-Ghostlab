#ifndef GHOSTLAB_CIMTUI_H
#define GHOSTLAB_CIMTUI_H

#ifdef __cplusplus
#define GHOSTLAB_CIMTUI_API extern "C"
#else
#define GHOSTLAB_CIMTUI_API
#endif

GHOSTLAB_CIMTUI_API void igTuiInit();
GHOSTLAB_CIMTUI_API void igTuiShutdown();
GHOSTLAB_CIMTUI_API bool igTuiNewFrame();
GHOSTLAB_CIMTUI_API void igTuiRender();
GHOSTLAB_CIMTUI_API bool igTuiProcessEvent();
GHOSTLAB_CIMTUI_API void igTuiShowDemoWindow(bool*);

#endif /* GHOSTLAB_CIMTUI_H */
