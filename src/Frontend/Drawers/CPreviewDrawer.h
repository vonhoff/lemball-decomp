#ifndef LEMBALL_FRONTEND_DRAWERS_CPREVIEWDRAWER_H
#define LEMBALL_FRONTEND_DRAWERS_CPREVIEWDRAWER_H

#include "../../Visos/Graphics/CBigBitmap.h"
#include "../Base/CBaseFrontendDrawer.h"
#include "../Support/CoordPair.h"
#include "Frontend/Support/CUserActionMessage.h"

#define PREVIEW_LEVEL_NAME_LINE_COUNT 3
#define PREVIEW_LEVEL_NAME_LINE_BUFFER_SIZE_BYTES 32
#define PREVIEW_TEXT_MAX_CHARACTERS 32
#define PREVIEW_TEXT_POSITION_COMPONENT_COUNT 2
#define PREVIEW_TEXT_POSITION_X 0
#define PREVIEW_TEXT_POSITION_Y 1

class CBaseRemap;
class CGDI;
class CMain2DDisplay;
class CRepeatAnim;
class CResBITMAP;
class CVSRect;
enum PreviewLayoutIndex {
	PreviewReturnButton = 0,
	PreviewGoButton = 1,
	PreviewPreviousButton = 2,
	PreviewNextButton = 3,
	PreviewBackground = 4,
	PreviewFormationAnchor = 5,
	PreviewFormationOffset = 6,
	PreviewGunLemming = 7,
	PreviewGunLemmingOffset = 8,
	PreviewOpponentAnim = 9,
	PreviewOpponentText = 10,
	PreviewTimeText = 11,
	PreviewTeamAnim = 12,
	PreviewTeamText = 13,
	PreviewTeamRow = 14,
	PreviewLemmingAnim = 15,
	PreviewLemmingText = 16,
	PreviewOpponentRow = 17,
	PreviewLevelNameOrigin = 18,
	PreviewLevelNameBounds = 19,
	PreviewSkillText = 20,
	PreviewNetworkLemmingAnim = 21,
	PreviewNetworkLemmingText = 22,
	PreviewNetworkLemmingRow = 23,
	PreviewNetworkFrameEndpoint = 24,
	PreviewTeamOffset = 25,
	PreviewNoneText = 26,
};

// SIZE 0xd8
struct PreviewLayout {
	CoordPair m_positions[27];
};

// SIZE 0x488
// VTABLE: LEMBALL 0x00497bf0 CDrawer
// VTABLE: LEMBALL 0x00497be0 CBaseQueueHandler
// VTABLE: LEMBALL 0x00497bdc CAnimsManager
class CPreviewDrawer : public CBaseFrontendDrawer {
public:
	CPreviewDrawer(CMain2DDisplay* p_display, CGDI* p_gdi, const CVSRect& p_rect);
	bool AddWord(char* p_source, char* p_line, int& p_sourcePos, int& p_linePos);
	bool ConfirmedAction(eUserActions p_action);
	virtual bool ProcessMessages(Message* p_message); // vtable+0x3c
	virtual void DrawAnims();                         // vtable+0x48
	virtual void DrawBackGround();                    // vtable+0x50
	virtual void DrawText();                          // vtable+0x4c
	virtual void Load();                              // vtable+0x40
	virtual void Processing();                        // vtable+0x38
	virtual void UnLoad();                            // vtable+0x44
	virtual ~CPreviewDrawer();                        // vtable+0x00
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
	// SIZE 0x24
	struct tagPRIMS {
		tagPRIMS();
		~tagPRIMS();

		CBigBitmap m_bitmap; // 0x00
	};

	tagPRIMS m_primitive[1];                                                                         // 0x398
	CResBITMAP* m_backgroundBitmap;                                                                  // 0x3bc
	CRepeatAnim* m_teamAnim;                                                                         // 0x3c0
	CRepeatAnim* m_lemmingAnim;                                                                      // 0x3c4
	CRepeatAnim* m_opponentAnim;                                                                     // 0x3c8
	unsigned char* m_remapTable;                                                                     // 0x3cc
	CBaseRemap* m_remap;                                                                             // 0x3d0
	PreviewLayout* m_layout;                                                                         // 0x3d4
	unsigned long m_lemmingAnimId;                                                                   // 0x3d8
	unsigned long m_teamAnimId;                                                                      // 0x3dc
	unsigned long m_opponentAnimId;                                                                  // 0x3e0
	unsigned long* m_nextButtonAnimIds;                                                              // 0x3e4
	unsigned long* m_previousButtonAnimIds;                                                          // 0x3e8
	int m_opponentCount;                                                                             // 0x3ec
	int m_lemmingCount;                                                                              // 0x3f0
	int m_teamCount;                                                                                 // 0x3f4
	int m_timeSeconds;                                                                               // 0x3f8
	char m_levelNameLines[PREVIEW_LEVEL_NAME_LINE_COUNT][PREVIEW_LEVEL_NAME_LINE_BUFFER_SIZE_BYTES]; // 0x3fc
	char m_timeText[5];                                                                              // 0x45c
	int m_textPositions[PREVIEW_LEVEL_NAME_LINE_COUNT * PREVIEW_TEXT_POSITION_COMPONENT_COUNT];      // 0x464
	unsigned int m_buttonBinding;                                                                    // 0x47c
	unsigned int m_nextDisabled;                                                                     // 0x480
	unsigned int m_previousDisabled;                                                                 // 0x484
};

// SYNTHETIC: LEMBALL 0x0044a940
// CPreviewDrawer::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x0044aa10
// CPreviewDrawer::`vector deleting destructor'

#endif
