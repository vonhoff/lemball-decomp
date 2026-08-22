#ifndef LEMBALL_FRONTEND_DRAWERS_PREVIEWDRAWER_H
#define LEMBALL_FRONTEND_DRAWERS_PREVIEWDRAWER_H

#include "../../Common.h"
#include "PreviewDrawerPrims.h" // complete type

// SIZE 0x488
// VTABLE: LEMBALL 0x00497bf0
class PreviewDrawer {
public:
	PreviewDrawer(Main2DDisplay* p_arg0, Gdi* p_arg1, const VsRect& p_arg2);
	bool AddWord(char* p_source, char* p_line, int& p_sourcePos, int& p_linePos);
	bool ConfirmedAction(int p_action);
	virtual bool ProcessMessages(Message* p_message); // vtable+0x3c
	virtual void DrawAnims();                         // vtable+0x48
	virtual void DrawBackGround();                    // vtable+0x50
	virtual void DrawText();                          // vtable+0x4c
	virtual void Load();                              // vtable+0x40
	virtual void Processing();                        // vtable+0x38
	virtual void UnLoad();                            // vtable+0x44
	virtual ~PreviewDrawer();                         // vtable+0x00
	void DisableNextLastButtons();
	void Go();
	void LoadLevelInformation();
	void NextLevel();
	void PreviousLevel();
	void RegisterRemaps();
	void Return();
	void SubWord(char* p_source, char* p_line, int& p_sourcePos, int& p_linePos);
	void UnRegisterRemaps();

private:
	PreviewDrawerPrims m_primitive;        // 0x398
	ResBitmap* m_backgroundBitmap;         // 0x3bc
	RepeatAnim* m_teamAnim;                // 0x3c0
	RepeatAnim* m_lemmingAnim;             // 0x3c4
	RepeatAnim* m_opponentAnim;            // 0x3c8
	unsigned char* m_remapTable;           // 0x3cc
	BaseRemap* m_remap;                    // 0x3d0
	void* m_layout;                        // 0x3d4
	unsigned int m_lemmingAnimId;          // 0x3d8
	unsigned int m_teamAnimId;             // 0x3dc
	unsigned int m_opponentAnimId;         // 0x3e0
	unsigned int* m_nextButtonAnimIds;     // 0x3e4
	unsigned int* m_previousButtonAnimIds; // 0x3e8
	int m_opponentCount;                   // 0x3ec
	int m_lemmingCount;                    // 0x3f0
	int m_teamCount;                       // 0x3f4
	int m_timeSeconds;                     // 0x3f8
	char m_levelNameLines[3][32];          // 0x3fc
	char m_timeText[5];                    // 0x45c
	int m_textPositions[6];                // 0x464
	unsigned int m_buttonBinding;          // 0x47c
	unsigned int m_nextDisabled;           // 0x480
	unsigned int m_previousDisabled;       // 0x484
};

#endif
