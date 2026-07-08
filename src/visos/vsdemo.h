#ifndef LEMBALL_VSDEMO_H
#define LEMBALL_VSDEMO_H

void CreateFrameTimerController(unsigned int uFrameInterval);
void DestroyFrameTimerController(void);
void *ConstructLevelDemoPlaybackController(void *pPlaybackController, unsigned int uFrameInterval);
void DestroyLevelDemoPlaybackController(void *pPlaybackController);
void ResetLevelDemoPlaybackCursor(void *pPlaybackController);
void SetLevelDemoPlaybackEnabled(void *pPlaybackController, int fEnabled);
void ServiceLevelDemoPlayback(void *pPlaybackController);
void ServiceLevelDemoPlaybackThunk(void *pPlaybackController);
void ReleaseLevelDemoRecordBuffer(void *pPlaybackController);
void StopLevelDemoPlayback(void *pPlaybackController);
int FilterLevelDemoPlaybackInputEvent(void *pPlaybackController, unsigned short *pEventId);
void *DeleteLevelDemoPlaybackController(void *pPlaybackController, unsigned char fDelete);
void *RestoreLevelDemoPlaybackBaseVtable(void *pPlaybackController, unsigned char fDelete);

extern void *g_pLevelDemoPlaybackController;
extern void *g_pLevelDemoPlaybackDescriptor;
extern int g_nStoredLevelDemoModeEnabled;

#endif
