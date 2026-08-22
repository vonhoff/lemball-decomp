#ifndef LEMBALL_SCAFFOLD_FRONTEND_DRAWERS_NETWORKOPTIONSDRAWER_H
#define LEMBALL_SCAFFOLD_FRONTEND_DRAWERS_NETWORKOPTIONSDRAWER_H

#include "../../Common.h"

// SIZE 0x438
// VTABLE: LEMBALL 0x00498640
class NetworkOptionsDrawer {
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
	void Load();
	void Lock();
	void Processing();
	void RegisterRemaps();
	void ResetHandlers();
	void SetMessage(int p_message);
	void Start(unsigned char p_mode);
	void StartBroadcast();
	void StartEditing(int p_stage, unsigned char p_clear);
	void StartMessageTimeout(int p_message, unsigned long p_duration);
	void Stop();
	void StopEditing();
	void UnLoad();
	void UnLock();
	void UnRegisterRemaps();
	void UpdateHighlightedEntry();

private:
	EditString* m_editor;            // 0x398
	unsigned int m_editingActive;    // 0x39c
	int m_editingStage;              // 0x3a0
	unsigned int m_pendingStage;     // 0x3a4
	int m_message;                   // 0x3a8
	unsigned int m_messageDirty;     // 0x3ac
	unsigned int m_messageStartTime; // 0x3b0
	unsigned int m_messageDuration;  // 0x3b4
	unsigned int m_broadcasting;     // 0x3b8
	unsigned int m_networkMode;      // 0x3bc
	unsigned int m_redrawPending;    // 0x3c0
	unsigned int m_networkState;     // 0x3c4
	unsigned int m_lastDrawTime;     // 0x3c8
	void* m_handlers;                // 0x3cc
	int m_acceptedPlayer;            // 0x3d0
	int m_highlightedPlayer;         // 0x3d4
	int m_lastError;                 // 0x3d8
	unsigned int m_locked;           // 0x3dc
	unsigned int m_startPending;     // 0x3e0
	unsigned int m_stopPending;      // 0x3e4
	unsigned int m_connectionState;  // 0x3e8
	undefined m_unknown3ec[0x28];    // 0x3ec
	void* m_remaps[6];               // 0x414
	void* m_layoutTable;             // 0x42c
	int m_pendingEvent;              // 0x430
	unsigned int m_handlerCount;     // 0x434
};

#endif
