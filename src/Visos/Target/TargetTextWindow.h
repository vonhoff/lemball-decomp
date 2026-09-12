#ifndef LEMBALL_VISOS_TARGET_TARGETTEXTWINDOW_H
#define LEMBALL_VISOS_TARGET_TARGETTEXTWINDOW_H

#include "../../Common.h"
#include "../Foundation/Critical.h"

// SIZE 0x58
class TargetTextWindow : public Critical {
public:
	void PostAllocatedTextControlString(const char* p_text, unsigned int p_color);
	int PointToLine(int p_x, int p_y);
	void RedrawAll();
	void RedrawLines(int p_firstLine, int p_lineCount);
	int UpdateVisibleRows();
	int UpdateClientWidth();
	void ResizeToWholeRows(int p_clientWidth, int p_clientHeight, undefined4 p_arg2);
	void Scroll(int p_scrollCode, int p_thumbPos);
	void BeginSelection(int p_x, int p_y, undefined4 p_arg2);
	void EndSelection(undefined4 p_arg0, undefined4 p_arg1, undefined4 p_arg2);
	void SetSelectionHighlight(int p_selected);
	void EnsureLineVisible(int p_line);
	void UpdateSelection(int p_x, int p_y, undefined4 p_arg2);
	char* GetSelectionText();
	void CopySelection();

private:
	void* m_windowHandle;               // 0x1c
	void* m_fontHandle;                 // 0x20
	TargetTextLineBuffer* m_lineBuffer; // 0x24
	int m_lineCapacity;                 // 0x28
	int m_lineCount;                    // 0x2c
	int m_visibleRows;                  // 0x30
	int m_visibleRowsCeiling;           // 0x34
	int m_topLine;                      // 0x38
	int m_lineHeight;                   // 0x3c
	int m_clientWidth;                  // 0x40
	int m_selecting;                    // 0x44
	int m_dragLine;                     // 0x48
	int m_selectionAnchor;              // 0x4c
	int m_selectionStart;               // 0x50
	int m_selectionEnd;                 // 0x54
};

extern TargetTextWindow* g_pDebugWindow;
extern int g_nTargetTextWindowClassRegistered;
extern int g_nTargetTextWindowActive;
extern int g_nTargetTextWindowCreated;
extern void* g_apTargetTextWindowLockVtable[2];
// LIBRARY: LEMBALL 0x0047ffc0 SYMBOL
// __amsg_exit

// LIBRARY: LEMBALL 0x0047fff0 SYMBOL
// _free

// LIBRARY: LEMBALL 0x00480d70 SYMBOL
// _malloc

// LIBRARY: LEMBALL 0x00480d90 SYMBOL
// __nh_malloc

// LIBRARY: LEMBALL 0x00480dd0 SYMBOL
// __heap_alloc

// LIBRARY: LEMBALL 0x00480e90 SYMBOL
// __lock

// LIBRARY: LEMBALL 0x00480f00 SYMBOL
// __unlock

// LIBRARY: LEMBALL 0x00483fb0 SYMBOL
// __callnewh

#endif
