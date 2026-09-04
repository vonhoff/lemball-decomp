#ifndef LEMBALL_FRONTEND_DRAWERS_NETWORKOPTIONSDRAWER_H
#define LEMBALL_FRONTEND_DRAWERS_NETWORKOPTIONSDRAWER_H

#include "../../Common.h"
#include "../Base/BaseFrontendDrawer.h"
#include "../Support/EntryHandler.h"

// SIZE 0xa0
// SIZE 0x08
struct NetworkOptionsFramePos {
	int m_x;
	int m_y;
};

struct NetworkOptionsLayout {
	NetworkOptionsFramePos m_framePos[14]; // 0x00
	short m_entryWidth;                    // 0x70
	short m_unknown72;                     // 0x72
	short m_entryHeight;                   // 0x74
	undefined m_unknown76[0x16];           // 0x76
	int m_rowStride;                       // 0x8c
	int m_entryX;                          // 0x90
	unsigned short m_entryY;               // 0x94
	undefined m_unknown96[0xa];            // 0x96
};

// SIZE 0x438
// VTABLE: LEMBALL 0x00498640 Drawer
// VTABLE: LEMBALL 0x00498630 BaseQueueHandler
// VTABLE: LEMBALL 0x0049862c AnimsManager
class NetworkOptionsDrawer : public BaseFrontendDrawer {
public:
	NetworkOptionsDrawer(Main2DDisplay* p_arg0, Gdi* p_arg1, const VsRect& p_arg2);
	bool AcceptingLock();
	bool HighlightNextEntry();
	bool HighlightPreviousEntry();
	bool ProcessMessages(Message* p_message);
	virtual void DrawAnims();        // vtable+0x48
	virtual ~NetworkOptionsDrawer(); // vtable+0x00
	void DrawBackGround();
	void DrawEntry(unsigned long p_index, int& p_value, int p_remap);
	void DrawFrame(int p_position);
	void DrawText();
	void GameNotReady(int p_index);
	void GameReady(int p_index);
	void InitialiseHandlers();
	void LastError();
	virtual void Load(); // vtable+0x40
	void Lock();
	void Processing();
	void RegisterRemaps();
	void ResetHandlers();
	void SetMessage(int p_message);
	void Start(unsigned int p_mode);
	void StartBroadcast();
	void StartEditing(int p_stage, unsigned int p_clear);
	void StartMessageTimeout(int p_message, unsigned long p_duration);
	void Stop();
	void StopEditing();
	virtual void UnLoad(); // vtable+0x44
	void UnLock();
	void UnRegisterRemaps();
	void UpdateHighlightedEntry();

	friend class NetworkOptionsProc;

private:
	EditString* m_editor;                // 0x398
	unsigned int m_editingActive;        // 0x39c
	int m_editingStage;                  // 0x3a0
	unsigned int m_pendingStage;         // 0x3a4
	int m_message;                       // 0x3a8
	unsigned int m_messageDirty;         // 0x3ac
	unsigned int m_messageStartTime;     // 0x3b0
	unsigned int m_messageDuration;      // 0x3b4
	unsigned int m_broadcasting;         // 0x3b8
	unsigned int m_networkMode;          // 0x3bc
	unsigned int m_redrawPending;        // 0x3c0
	unsigned int m_networkState;         // 0x3c4
	unsigned int m_lastDrawTime;         // 0x3c8
	EntryHandler* m_playerEntries;       // 0x3cc
	int m_acceptedPlayer;                // 0x3d0
	int m_highlightedPlayer;             // 0x3d4
	int m_visibleEntryCount;             // 0x3d8
	unsigned int m_locked;               // 0x3dc
	unsigned int m_startPending;         // 0x3e0
	unsigned int m_stopPending;          // 0x3e4
	unsigned int m_connectionState;      // 0x3e8
	undefined m_unknown3ec[0x28];        // 0x3ec
	BaseRemap* m_remaps[6];              // 0x414
	NetworkOptionsLayout* m_layoutTable; // 0x42c
	int m_pendingEvent;                  // 0x430
	unsigned int m_handlerCount;         // 0x434
};

// 68K 0x10806800 __dt__21CNetworkOptionsDrawerFv
// SYNTHETIC: LEMBALL 0x00455de0
// NetworkOptionsDrawer::`scalar deleting destructor'

#endif
