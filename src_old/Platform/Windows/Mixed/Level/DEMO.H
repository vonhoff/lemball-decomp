#ifndef LEMBALL_DEMO_PLAYBACK_H
#define LEMBALL_DEMO_PLAYBACK_H

#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"

void CreateFrameTimerController(unsigned int uFrameInterval);
void DestroyFrameTimerController(void);
struct DemoLevelDemoPlaybackController {
	unsigned char m_abData[0x10];
	void* m_pActiveRecord10;                // 0x10
	int m_nFrameInterval14;                 // 0x14
	int m_nLoadedRecordId18;                // 0x18  (CDemo::Reset sets to -1)
	void* m_pRecordBuffer1C;                // 0x1c
	char* m_pResourceHandle20;              // 0x20  (demo record file path)
	void* m_pResourceObject24;              // 0x24
	int m_nResourceId28;                    // 0x28
	int m_nResourceCount2C;                 // 0x2c
	int m_nResourceTotal30;                 // 0x30
	void* m_pResourceTable34;              // 0x34
	int m_nFrameTimeBase38;                 // 0x38  (CDemo frame time base)
	int m_nActiveFrameTick3C;               // 0x3c
	unsigned short m_nReserved40;           // 0x40
	unsigned short m_nReserved42;           // 0x42
	int m_nPlaybackFrame44;                 // 0x44
	int m_nReserved48;                      // 0x48
	int m_nPlaybackEnabled4C;               // 0x4c  (CDemo::SetDemoMode)
	int m_nStopFlag50;                      // 0x50
	int m_nReserved54;                      // 0x54

	DemoLevelDemoPlaybackController* ConstructLevelDemoPlaybackController(unsigned int uFrameInterval);
	DemoLevelDemoPlaybackController* ConstructLevelDemoPlaybackControllerThunk(unsigned int uFrameInterval);
	void ResetLevelDemoPlaybackCursor(void);
	void ResetLevelDemoPlaybackCursorThunk(void);
	void SetLevelDemoPlaybackEnabled(int fEnabled);
	void DestroyLevelDemoPlaybackController(void);
	void DestroyLevelDemoPlaybackControllerThunk(void);
};
void LEMBALL_FASTCALL ReleaseLevelDemoRecordBufferThunk(void* pPlaybackController);
void LEMBALL_FASTCALL ServiceLevelDemoPlayback(void* pPlaybackController);
void LEMBALL_FASTCALL ReleaseLevelDemoRecordBuffer(void* pPlaybackController);
void LEMBALL_FASTCALL StopLevelDemoPlayback(void* pPlaybackController);
int LEMBALL_FASTCALL FilterLevelDemoPlaybackInputEvent(void* pPlaybackController, int nUnused, unsigned short* pEventId);
void* LEMBALL_FASTCALL RestoreLevelDemoPlaybackBaseVtable(void* pPlaybackController, void*, unsigned char fDelete);

extern void* g_pLevelDemoPlaybackController;
extern void* g_pLevelDemoPlaybackDescriptor;
extern int g_nStoredLevelDemoModeEnabled;

#endif
