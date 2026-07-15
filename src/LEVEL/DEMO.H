#ifndef LEMBALL_DEMO_PLAYBACK_H
#define LEMBALL_DEMO_PLAYBACK_H

#include "CORE/COMMON.H"

void CreateFrameTimerController(unsigned int uFrameInterval);
void DestroyFrameTimerController(void);
struct DemoLevelDemoPlaybackController {
    DemoLevelDemoPlaybackController *ConstructLevelDemoPlaybackController(unsigned int uFrameInterval);
    DemoLevelDemoPlaybackController *ConstructLevelDemoPlaybackControllerThunk(unsigned int uFrameInterval);
    void ResetLevelDemoPlaybackCursor(void);
    void ResetLevelDemoPlaybackCursorThunk(void);
    void SetLevelDemoPlaybackEnabled(int fEnabled);
};
void DestroyLevelDemoPlaybackController(void *pPlaybackController);
void LEMBALL_FASTCALL ReleaseLevelDemoRecordBufferThunk(void *pPlaybackController);
void ServiceLevelDemoPlayback(void *pPlaybackController);
void LEMBALL_FASTCALL ServiceLevelDemoPlaybackThunk(void *pPlaybackController);
void LEMBALL_FASTCALL ReleaseLevelDemoRecordBuffer(void *pPlaybackController);
void StopLevelDemoPlayback(void *pPlaybackController);
int FilterLevelDemoPlaybackInputEvent(void *pPlaybackController, unsigned short *pEventId);
void *DeleteLevelDemoPlaybackController(void *pPlaybackController, unsigned char fDelete);
void *RestoreLevelDemoPlaybackBaseVtable(void *pPlaybackController, unsigned char fDelete);

extern void *g_pLevelDemoPlaybackController;
extern void *g_pLevelDemoPlaybackDescriptor;
extern int g_nStoredLevelDemoModeEnabled;

#endif
