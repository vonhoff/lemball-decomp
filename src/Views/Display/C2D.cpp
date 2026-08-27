#include "C2D.h"

#include "../../AI/Navigation/Ai.h"
#include "../../AI/Objects/PlayerLemming.h"
#include "../../Visos/Graphics/BasePalManager.h"

// 68K 0x10b06778 __ct__3C2DFP14CMain2DDisplayP3CAIP4CGDIP4CMapRC7CVSRect
// STUB: LEMBALL 0x004358d0
C2D::C2D(Main2DDisplay* p_arg0, Ai* p_arg1, Gdi* p_arg2, Map* p_arg3, const VsRect& p_arg4)
{
}

// 68K 0x10b06e3e __dt__3C2DFv
// STUB: LEMBALL 0x00436050
C2D::~C2D()
{
}

// 68K 0x10b070a2 ShutDown__3C2DFv
// STUB: LEMBALL 0x00436190
void C2D::ShutDown()
{
}

// 68K 0x10b0735a RegisterRemaps__3C2DFv
// STUB: LEMBALL 0x004363c0
void C2D::RegisterRemaps()
{
}

// 68K 0x10b07444 UnRegisterRemaps__3C2DFv
// STUB: LEMBALL 0x00436480
void C2D::UnRegisterRemaps()
{
}

#include "../../Visos/Graphics/Cursor.h"

// 68K 0x10b07492 CursorChangeType__3C2DF18eCursorDisplayTypei
// FUNCTION: LEMBALL 0x004364b0
void C2D::CursorChangeType(int p_cursorType, int p_value)
{
	::CursorChangeType((eCursorDisplayType) p_cursorType, p_value);
}

// 68K 0x10b074d8 OnLoaded__3C2DFv
// STUB: LEMBALL 0x004364d0
void C2D::OnLoaded()
{
}

// 68K 0x10b0780e DoButtons__3C2DFv
// FUNCTION: LEMBALL 0x00436690
void C2D::DoButtons()
{
}

// 68K 0x10b0782a OnZoom__3C2DFRC7CVSRect
// FUNCTION: LEMBALL 0x004366a0
void C2D::OnZoom(const VsRect& p_rect)
{
}

// 68K 0x10b0784c OnSize__3C2DFRC7CVSRect
// STUB: LEMBALL 0x004366b0
void C2D::OnSize(const VsRect& p_rect)
{
}

// 68K 0x10b07924 SetUpRemapPalettes__3C2DFv
// STUB: LEMBALL 0x00436760
void C2D::SetUpRemapPalettes()
{
}

// 68K 0x10b079c8 KillRemapPalettes__3C2DFv
// FUNCTION: LEMBALL 0x00436830
void C2D::KillRemapPalettes()
{
	g_pBasePalManager->UnRegisterRemap(m_paletteRemap);
}

// 68K 0x10b07a00 Restart__3C2DFv
// STUB: LEMBALL 0x00436850
void C2D::Restart()
{
}

// 68K 0x10b07a86 CheckValidFormGroup__3C2DFv
// STUB: LEMBALL 0x004369b0
void C2D::CheckValidFormGroup()
{
}

// 68K 0x10b07b18 Process__3C2DFv
// STUB: LEMBALL 0x00436a10
void C2D::Process()
{
}

// 68K 0x10b07f46 FindGameObject__3C2DFRC8CVSPointRiUc
// STUB: LEMBALL 0x00436e40
bool C2D::FindGameObject(const VsPoint& p_point, int& p_index, unsigned char p_preferLemming)
{
	return 0;
}

// 68K 0x10b0827e AddObjectToGroup__3C2DFiUc
// STUB: LEMBALL 0x00437130
void C2D::AddObjectToGroup(int p_objectNo, unsigned char p_markSelection)
{
}

// 68K 0x10b082cc FormGroup__3C2DFv
// STUB: LEMBALL 0x00437170
void C2D::FormGroup()
{
}

// 68K 0x10b08354 MoveGroup__3C2DFRC8CVSPoint
// STUB: LEMBALL 0x004371e0
void C2D::MoveGroup(const VsPoint& p_point)
{
}

// 68K 0x10b083d6 CancelMoves__3C2DFv
// STUB: LEMBALL 0x00437250
void C2D::CancelMoves()
{
}

// 68K 0x10b0843a NextGroup__3C2DFv
// STUB: LEMBALL 0x004372a0
void C2D::NextGroup()
{
}

// 68K 0x10b0849c PrevGroup__3C2DFv
// STUB: LEMBALL 0x004372f0
void C2D::PrevGroup()
{
}

// 68K 0x10b084fe SelectLemming__3C2DFi
// STUB: LEMBALL 0x00437340
void C2D::SelectLemming(int p_playerIndex)
{
}

// 68K 0x10b08588 SelectObject__3C2DFi
// STUB: LEMBALL 0x004373b0
void C2D::SelectObject(int p_viewIndex)
{
}

// 68K 0x10b0860e InGroupByObjectNo__3C2DFi
// STUB: LEMBALL 0x00437420
bool C2D::InGroupByObjectNo(int p_objectNo)
{
	return 0;
}

// 68K 0x10b08662 RemoveFromGroupByObjectNo__3C2DFi
// STUB: LEMBALL 0x00437460
void C2D::RemoveFromGroupByObjectNo(int p_objectNo)
{
}

// 68K 0x10b086f8 IsInGrouping__3C2DFP11CGameObject
// STUB: LEMBALL 0x004374e0
bool C2D::IsInGrouping(GameObject* p_object)
{
	return 0;
}

// 68K 0x10b0875a NoStateLeftClick__3C2DFRC8CVSPointRC8CVSPointUcUc
// STUB: LEMBALL 0x00437520
void C2D::NoStateLeftClick(const VsPoint& p_screenPoint,
						   const VsPoint& p_gamePoint,
						   unsigned char p_commitMoves,
						   unsigned char p_alternate)
{
}

// 68K 0x10b088fa GroupingLeftClick__3C2DFRC8CVSPointRC8CVSPointUc
// STUB: LEMBALL 0x004376b0
void C2D::GroupingLeftClick(const VsPoint& p_screenPoint, const VsPoint& p_gamePoint, unsigned char p_alternate)
{
}

// 68K 0x10b08aae LeftClick__3C2DFRC8CVSPointRC8CVSPointUcUc
// STUB: LEMBALL 0x00437840
void C2D::LeftClick(const VsPoint& p_screenPoint,
					const VsPoint& p_gamePoint,
					unsigned char p_commitMoves,
					unsigned char p_alternate)
{
}

// 68K 0x10b08b30 NoStateRightClick__3C2DFRC8CVSPointRC8CVSPoint
// STUB: LEMBALL 0x00437890
void C2D::NoStateRightClick(const VsPoint& p_screenPoint, const VsPoint& p_gamePoint)
{
}

// 68K 0x10b08c02 RightClick__3C2DFRC8CVSPointRC8CVSPoint
// STUB: LEMBALL 0x00437930
void C2D::RightClick(const VsPoint& p_screenPoint, const VsPoint& p_gamePoint)
{
}

// 68K 0x10b08c70 ScreenToGame__3C2DFiiRiRi
// STUB: LEMBALL 0x00437970
bool C2D::ScreenToGame(int p_screenX, int p_screenY, int& p_gameX, int& p_gameY)
{
	return 0;
}

// 68K 0x10b08e94 ProcessMsg__3C2DFP10tagMESSAGE
// STUB: LEMBALL 0x00437b60
int C2D::ProcessMsg(Message* p_message)
{
	return 0;
}

// 68K 0x10b09008 NewPauseWindow__3C2DF20ePauseWindowMessages
// STUB: LEMBALL 0x00437d00
void C2D::NewPauseWindow(int p_message)
{
}

// 68K 0x10b090e0 TriggerPause__3C2DFUc
// STUB: LEMBALL 0x00437da0
void C2D::TriggerPause(unsigned char p_paused)
{
}

// 68K 0x10b0914c SetPause__3C2DFUc
// STUB: LEMBALL 0x00437de0
void C2D::SetPause(unsigned char p_paused)
{
}

// 68K 0x10b09208 SetMouseShape__3C2DFv
// STUB: LEMBALL 0x00437e90
void C2D::SetMouseShape()
{
}

// 68K 0x10b09436 SendCursorMsg__3C2DFv
// STUB: LEMBALL 0x004380c0
void C2D::SendCursorMsg()
{
}

// 68K 0x10b09514 OnInside__3C2DFRC8CVSPoint
// STUB: LEMBALL 0x00438170
void C2D::OnInside(const VsPoint& p_point)
{
}

// 68K 0x10b0958c OnButtonUp__3C2DFRC8CVSPoint12BUTTON_FLAGS
// STUB: LEMBALL 0x004381c0
void C2D::OnButtonUp(const VsPoint& p_point, unsigned int p_flags)
{
}

// 68K 0x10b0960a OnButtonDown__3C2DFRC8CVSPoint12BUTTON_FLAGS
// STUB: LEMBALL 0x00438210
unsigned int C2D::OnButtonDown(const VsPoint& p_point, unsigned int p_flags)
{
	return 0;
}

// 68K 0x10b09786 UseBalloon__3C2DFi
// STUB: LEMBALL 0x00438330
void C2D::UseBalloon(int p_playerIndex)
{
}

// 68K 0x10b09802 UseBalloon__3C2DFP14CPlayerLemming
// STUB: LEMBALL 0x00438380
void C2D::UseBalloon(PlayerLemming* p_lemming)
{
}

// 68K 0x10b09876 OnDriverChange__3C2DFv
// STUB: LEMBALL 0x004383c0
void C2D::OnDriverChange()
{
}

// 68K 0x10b09a30 SetClipSize__3C2DFv
// STUB: LEMBALL 0x00438500
void C2D::SetClipSize()
{
}

// 68K 0x1011cbe0 QuitYet__3C2DFv
// FUNCTION: LEMBALL 0x00439450
bool C2D::QuitYet()
{
	return m_quitRequested;
}

// 68K 0x1011cc02 GetReturnState__3C2DFv
// FUNCTION: LEMBALL 0x00439460
int C2D::GetReturnState()
{
	return m_returnState;
}

// 68K 0x1011cc2c GetPauser__3C2DFv
// FUNCTION: LEMBALL 0x00439470
bool C2D::GetPauser()
{
	return m_pauser;
}

// 68K 0x10b01120 DrawGround__3C2DFii11eObjectTypeUs
// STUB: LEMBALL 0x0043a880
void C2D::DrawGround(int p_x, int p_y, eObjectType p_groundType, unsigned short p_frame)
{
}

// 68K 0x10b015a6 DrawCliff__3C2DFiiii
// STUB: LEMBALL 0x0043ace0
void C2D::DrawCliff(int p_x, int p_y, int p_height, int p_count)
{
}

// 68K 0x10b0162c DoClipWidth__3C2DFiii
// STUB: LEMBALL 0x0043ad40
void C2D::DoClipWidth(int p_mapX, int p_mapY, int p_count)
{
}

// 68K 0x10b01988 DoClipWidthSearch__3C2DFiii
// STUB: LEMBALL 0x0043b0e0
void C2D::DoClipWidthSearch(int p_mapX, int p_mapY, int p_count)
{
}

// 68K 0x10b01cf0 DrawClipData__3C2DFv
// STUB: LEMBALL 0x0043b4b0
void C2D::DrawClipData()
{
}

// 68K 0x10b01d3a DrawClippedRectangle__3C2DFRC7CVSRect
// STUB: LEMBALL 0x0043b4d0
void C2D::DrawClippedRectangle(const VsRect& p_rect)
{
}

// 68K 0x10b02196 LemmingFly__3C2DFR9CViewDataRi
// STUB: LEMBALL 0x0043bce0
unsigned long C2D::LemmingFly(ViewData& p_viewData, int& p_frame)
{
	return 0;
}

// 68K 0x10b022ac DrawLemmingFlyShadow__3C2DFR9CViewData
// STUB: LEMBALL 0x0043bde0
void C2D::DrawLemmingFlyShadow(ViewData& p_viewData)
{
}

// 68K 0x10b023d4 DrawLemmingJump__3C2DFR9CViewDataUc
// STUB: LEMBALL 0x0043bee0
void C2D::DrawLemmingJump(ViewData& p_viewData, unsigned char p_remapped)
{
}

// 68K 0x10b024ea DrawLemmingLanding__3C2DFR9CViewDataUc
// STUB: LEMBALL 0x0043bfc0
void C2D::DrawLemmingLanding(ViewData& p_viewData, unsigned char p_remapped)
{
}

// 68K 0x10b025d2 DrawLemmingFall__3C2DFR9CViewDataUc
// STUB: LEMBALL 0x0043c070
void C2D::DrawLemmingFall(ViewData& p_viewData, unsigned char p_remapped)
{
}

// 68K 0x10b02612 DrawLemmingExternal__3C2DFR9CViewDataUc
// STUB: LEMBALL 0x0043c090
void C2D::DrawLemmingExternal(ViewData& p_viewData, unsigned char p_remapped)
{
}

// 68K 0x10b0274e DrawLemmingOnConveyor__3C2DFR9CViewDataUc
// STUB: LEMBALL 0x0043c1a0
void C2D::DrawLemmingOnConveyor(ViewData& p_viewData, unsigned char p_remapped)
{
}

// 68K 0x10b027f2 DrawLemming__3C2DFR9CViewDataiUc
// STUB: LEMBALL 0x0043c200
void C2D::DrawLemming(ViewData& p_viewData, int p_objectNo, unsigned char p_remapped)
{
}

// 68K 0x10b02c40 DrawBullet__3C2DFR9CViewDatai
// STUB: LEMBALL 0x0043c610
void C2D::DrawBullet(ViewData& p_viewData, int p_objectNo)
{
}

// 68K 0x10b02cce DrawAmmo__3C2DFR9CViewDatai
// STUB: LEMBALL 0x0043c660
void C2D::DrawAmmo(ViewData& p_viewData, int p_objectNo)
{
}

// 68K 0x10b02d9e DrawRocket__3C2DFR9CViewData
// STUB: LEMBALL 0x0043c6e0
void C2D::DrawRocket(ViewData& p_viewData)
{
}

// 68K 0x10b02ede DrawHand__3C2DFR9CViewData
// STUB: LEMBALL 0x0043c7f0
void C2D::DrawHand(ViewData& p_viewData)
{
}

// 68K 0x10b02fb0 DrawLemmingOnBalloon__3C2DFR9CViewDataiUc
// STUB: LEMBALL 0x0043c8a0
void C2D::DrawLemmingOnBalloon(ViewData& p_viewData, int p_balloonType, unsigned char p_remapped)
{
}

// 68K 0x10b030b6 DrawBalloon__3C2DFR9CViewDatai
// STUB: LEMBALL 0x0043c940
void C2D::DrawBalloon(ViewData& p_viewData, int p_playerIndex)
{
}

// 68K 0x10b031aa DrawBalloonPost__3C2DFR9CViewDatai
// STUB: LEMBALL 0x0043c9f0
void C2D::DrawBalloonPost(ViewData& p_viewData, int p_playerIndex)
{
}

// 68K 0x10b03222 DrawTrampoline__3C2DFR9CViewData
// STUB: LEMBALL 0x0043ca30
void C2D::DrawTrampoline(ViewData& p_viewData)
{
}

// 68K 0x10b032ec DrawMover__3C2DFR9CViewData
// STUB: LEMBALL 0x0043cac0
void C2D::DrawMover(ViewData& p_viewData)
{
}

// 68K 0x10b033f0 DrawSlinky__3C2DFR9CViewData
// STUB: LEMBALL 0x0043cbb0
void C2D::DrawSlinky(ViewData& p_viewData)
{
}

// 68K 0x10b035b4 DrawPaintGun__3C2DFR9CViewData
// STUB: LEMBALL 0x0043cd50
void C2D::DrawPaintGun(ViewData& p_viewData)
{
}

// 68K 0x10b036dc DrawLaserFire__3C2DFR9CViewData
// STUB: LEMBALL 0x0043ce30
void C2D::DrawLaserFire(ViewData& p_viewData)
{
}

// 68K 0x10b03788 DrawLaser__3C2DFR9CViewData
// STUB: LEMBALL 0x0043cea0
void C2D::DrawLaser(ViewData& p_viewData)
{
}

// 68K 0x10b03882 DrawDuplicator__3C2DFR9CViewData
// STUB: LEMBALL 0x0043cfa0
void C2D::DrawDuplicator(ViewData& p_viewData)
{
}

// 68K 0x10b0397e DrawCrate__3C2DFR9CViewDatai
// STUB: LEMBALL 0x0043d070
void C2D::DrawCrate(ViewData& p_viewData, int p_objectNo)
{
}

// 68K 0x10b03a4e DrawTimeBonus__3C2DFR9CViewData
// STUB: LEMBALL 0x0043d0f0
void C2D::DrawTimeBonus(ViewData& p_viewData)
{
}

// 68K 0x10b03acc DrawCatapult__3C2DFR9CViewDatai
// STUB: LEMBALL 0x0043d130
void C2D::DrawCatapult(ViewData& p_viewData, int p_objectNo)
{
}

// 68K 0x10b03d7e DrawSheep__3C2DFR9CViewDatai
// STUB: LEMBALL 0x0043d370
void C2D::DrawSheep(ViewData& p_viewData, int p_objectNo)
{
}

// 68K 0x10b03e6e DrawBall__3C2DFR9CViewData
// STUB: LEMBALL 0x0043d420
void C2D::DrawBall(ViewData& p_viewData)
{
}

// 68K 0x10b03f56 DrawKey__3C2DFR9CViewDatai
// STUB: LEMBALL 0x0043d4b0
void C2D::DrawKey(ViewData& p_viewData, int p_playerIndex)
{
}

// 68K 0x10b03fc8 DrawMine__3C2DFR9CViewData
// STUB: LEMBALL 0x0043d500
void C2D::DrawMine(ViewData& p_viewData)
{
}

// 68K 0x10b0409a DrawDoor__3C2DFR9CViewData
// STUB: LEMBALL 0x0043d590
void C2D::DrawDoor(ViewData& p_viewData)
{
}

// 68K 0x10b042fa DrawSwitch__3C2DFR9CViewData
// STUB: LEMBALL 0x0043d7e0
void C2D::DrawSwitch(ViewData& p_viewData)
{
}

// 68K 0x10b0440c DrawFlag__3C2DFR9CViewData11eObjectType
// STUB: LEMBALL 0x0043d8d0
void C2D::DrawFlag(ViewData& p_viewData, eObjectType p_objectType)
{
}

// 68K 0x10b044dc DrawBonus__3C2DFR9CViewData
// STUB: LEMBALL 0x0043d950
void C2D::DrawBonus(ViewData& p_viewData)
{
}

// 68K 0x10b0454a DrawTrapDoor__3C2DFR9CViewData
// STUB: LEMBALL 0x0043d990
void C2D::DrawTrapDoor(ViewData& p_viewData)
{
}

// 68K 0x10b0487c DrawObject__3C2DFR9CViewData
// STUB: LEMBALL 0x0043dc70
void C2D::DrawObject(ViewData& p_viewData)
{
}

// 68K 0x10b04c66 SetOrigin__3C2DFv
// STUB: LEMBALL 0x0043ed20
void C2D::SetOrigin()
{
}

// 68K 0x10b04ede DrawObjects__3C2DFv
// STUB: LEMBALL 0x0043f620
void C2D::DrawObjects()
{
}

// 68K 0x10b05744 DrawDemo__3C2DFv
// STUB: LEMBALL 0x0043fce0
void C2D::DrawDemo()
{
}

// 68K 0x10b05804 DrawTime__3C2DFv
// STUB: LEMBALL 0x0043fd80
void C2D::DrawTime()
{
}

// 68K 0x10b05914 DrawPaused__3C2DFv
// FUNCTION: LEMBALL 0x0043fe70
void C2D::DrawPaused()
{
}

// 68K 0x10b05932 DrawScore__3C2DFv
// STUB: LEMBALL 0x0043fe80
void C2D::DrawScore()
{
}

// 68K 0x10b05a64 SortViewData__3C2DFv
// STUB: LEMBALL 0x0043ff70
void C2D::SortViewData()
{
}

// 68K 0x10b05b06 Draw__3C2DFRC7CVSRect
// STUB: LEMBALL 0x00440000
void C2D::Draw(const VsRect& p_rect)
{
}

// 68K 0x10b05fa8 ReplaceBackground__3C2DFv
// STUB: LEMBALL 0x00440400
void C2D::ReplaceBackground()
{
}

// 68K 0x10b0600e ResetPrimitives__3C2DFv
// STUB: LEMBALL 0x00440430
void C2D::ResetPrimitives()
{
}

// 68K 0x10b06056 DrawZBuff_Sprite__3C2DFiUs
// STUB: LEMBALL 0x00440460
void C2D::DrawZBuffSprite(int p_index, unsigned short p_z)
{
}

// 68K 0x10b060ac DrawZBuff_Anim__3C2DFiUs
// STUB: LEMBALL 0x00440490
void C2D::DrawZBuffAnim(int p_index, unsigned short p_z)
{
}

// 68K 0x10b061ac DrawObjectsZBuff__3C2DFv
// STUB: LEMBALL 0x00440560
void C2D::DrawObjectsZBuff()
{
}

// 68K 0x10b06416 CalcZValue_Sprite__3C2DFi
// STUB: LEMBALL 0x004407e0
int C2D::CalcZValueSprite(int p_index)
{
	return 0;
}

// 68K 0x10b064c6 CalcGroundCode__3C2DF11eObjectTypeiiUs
// STUB: LEMBALL 0x00440840
unsigned short C2D::CalcGroundCode(eObjectType p_objectType, int p_x, int p_y, unsigned short p_z)
{
	return 0;
}

// 68K 0x10b06752 InitSpriteGroundLU__3C2DFv
// FUNCTION: LEMBALL 0x00440c00
void C2D::InitSpriteGroundLu()
{
}
