#ifndef LEMBALL_VISOS_TARGET_TARGETTEXTWINDOW_H
#define LEMBALL_VISOS_TARGET_TARGETTEXTWINDOW_H

#include "../../Common.h"

// SIZE 0x58
class TargetTextWindow {
public:
	void PostAllocatedTextControlString(const char* p_text, unsigned int p_color);

private:
	void* m_lockVtable;                  // 0x00
	unsigned char m_criticalSection[24]; // 0x04
	void* m_windowHandle;                // 0x1c
	void* m_fontHandle;                  // 0x20
	TargetTextLineBuffer* m_lineBuffer;  // 0x24
	int m_lineCapacity;                  // 0x28
	int m_lineCount;                     // 0x2c
	int m_visibleRows;                   // 0x30
	int m_visibleRowsCeiling;            // 0x34
	int m_topLine;                       // 0x38
	int m_lineHeight;                    // 0x3c
	int m_clientWidth;                   // 0x40
	int m_selecting;                     // 0x44
	int m_dragLine;                      // 0x48
	int m_selectionAnchor;               // 0x4c
	int m_selectionStart;                // 0x50
	int m_selectionEnd;                  // 0x54
};

// Confirmed class-scoped globals.
extern TargetTextWindow* g_pDebugWindow;
extern int g_nTargetTextWindowClassRegistered;
extern int g_nTargetTextWindowActive;
extern int g_nTargetTextWindowCreated;
extern void* g_apTargetTextWindowLockVtable[2];
#endif
