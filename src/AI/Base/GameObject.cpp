#include "GameObject.h"

#include "../Navigation/AiDestinationList.h"
#include "../../Control/Game/Game.h"

GameObject::GameObject()
{
}

// 68K 0x10118af4 HitBullet__11CGameObjectFP7CBullet
// STUB: LEMBALL 0x00408980
void GameObject::HitBullet(Bullet* p_bullet)
{
}

// 68K 0x10118300 GetViewData__11CGameObjectFR9CViewData
// STUB: LEMBALL 0x0040a6f0
void GameObject::GetViewData(ViewData& p_viewData)
{
}

// 68K 0x10118858 ForgetObjectLink__11CGameObjectFUs
// FUNCTION: LEMBALL 0x0040a7f0
void GameObject::ForgetObjectLink(unsigned short p_arg0)
{
}

// 68K 0x101188ac Activate__11CGameObjectFP11CGameObject
// FUNCTION: LEMBALL 0x0040a800
bool GameObject::Activate(class GameObject* p_object)
{
	return 1;
}

// 68K 0x101188e0 IsFlying__11CGameObjectFv
// FUNCTION: LEMBALL 0x0040a810
bool GameObject::IsFlying()
{
	return m_isFlying;
}

// 68K 0x10118916 Usage__11CGameObjectFv
// FUNCTION: LEMBALL 0x0040a820
int GameObject::Usage()
{
	return 0;
}

// 68K 0x1011a3e6 Position__11CGameObjectFv
// STUB: LEMBALL 0x0040a830 FOLDED
AiCoord GameObject::Position()
{
	return *(AiCoord*) 0;
}

// 68K 0x1011893a ActivatePosition__11CGameObjectFv
// STUB: LEMBALL 0x0040a830 FOLDED
AiCoord GameObject::ActivatePosition()
{
	return *(AiCoord*) 0;
}

// 68K 0x10118982 StartStanding__11CGameObjectFv
// FUNCTION: LEMBALL 0x0040a860
void GameObject::StartStanding()
{
}

// 68K 0x1011842a SetSndEffect__11CGameObjectF12eSoundEffect
// FUNCTION: LEMBALL 0x0040a870
void GameObject::SetSndEffect(eSoundEffect p_arg0)
{
	m_soundEffect = p_arg0;
}

// 68K 0x101189ac GetSndEffect__11CGameObjectFv
// FUNCTION: LEMBALL 0x0040a880
eSoundEffect GameObject::GetSndEffect()
{
	return m_soundEffect;
}

// 68K 0x101189dc Collision__11CGameObjectFRC4CPt3
// STUB: LEMBALL 0x0040a890
bool GameObject::Collision(const Pt3& p_arg0)
{
	return 0;
}

// 68K 0x10118a60 Collision__11CGameObjectFRC6CRect3
// STUB: LEMBALL 0x0040a8e0
bool GameObject::Collision(const Rect3& p_arg0)
{
	return 0;
}

// 68K 0x10118b22 HitBall__11CGameObjectFv
// FUNCTION: LEMBALL 0x0040a940
void GameObject::HitBall()
{
}

// 68K 0x10118b46 IsHit__11CGameObjectFv
// FUNCTION: LEMBALL 0x0040a950
int GameObject::IsHit()
{
	return 0;
}

// 68K 0x10118b6a GetHit__11CGameObjectFv
// FUNCTION: LEMBALL 0x0040a960
void GameObject::GetHit()
{
}

// 68K 0x10118b8c HitMine__11CGameObjectFv
// FUNCTION: LEMBALL 0x0040a970
void GameObject::HitMine()
{
}

// 68K 0x10118bb0 Die__11CGameObjectFv
// FUNCTION: LEMBALL 0x0040a980
void GameObject::Die()
{
}

// 68K 0x10118bd0 Land__11CGameObjectFv
// FUNCTION: LEMBALL 0x0040a990
void GameObject::Land()
{
}

// 68K 0x10118bf0 FacingCursor__11CGameObjectFv
// FUNCTION: LEMBALL 0x0040a9a0
bool GameObject::FacingCursor()
{
	return 0;
}

// 68K 0x10118c1a TurnToFaceCursor__11CGameObjectFv
// FUNCTION: LEMBALL 0x0040a9b0
void GameObject::TurnToFaceCursor()
{
}

// 68K 0x10118c48 IsRequestingFire__11CGameObjectFv
// FUNCTION: LEMBALL 0x0040a9c0
bool GameObject::IsRequestingFire()
{
	return 0;
}

// 68K 0x10118c78 Bored__11CGameObjectFv
// FUNCTION: LEMBALL 0x0040a9d0
int GameObject::Bored()
{
	return 0;
}

// 68K 0x10118c9c SetBored__11CGameObjectFUl
// FUNCTION: LEMBALL 0x0040a9e0
void GameObject::SetBored(unsigned long p_arg0)
{
}

// 68K 0x10118cc2 Fire__11CGameObjectFv
// STUB: LEMBALL 0x0040a9f0
bool GameObject::Fire()
{
	return 0;
}

// 68K 0x10118ce2 StartFiring__11CGameObjectFv
// FUNCTION: LEMBALL 0x0040aa00
void GameObject::StartFiring()
{
}

// 68K 0x10118d0a EndFiring__11CGameObjectFv
// FUNCTION: LEMBALL 0x0040aa10
void GameObject::EndFiring()
{
}

// 68K 0x10118d30 RandomAction__11CGameObjectFv
// STUB: LEMBALL 0x0040aa20
void GameObject::RandomAction()
{
}

// 68K 0x10118d58 FacingTarget__11CGameObjectFv
// FUNCTION: LEMBALL 0x0040aa30
bool GameObject::FacingTarget()
{
	return 0;
}

// 68K 0x10118d82 TurnToFaceTarget__11CGameObjectFv
// FUNCTION: LEMBALL 0x0040aa40
void GameObject::TurnToFaceTarget()
{
}

// 68K 0x10118db0 PossiblyOnLift__11CGameObjectFv
// FUNCTION: LEMBALL 0x0040aa50
bool GameObject::PossiblyOnLift()
{
	return 1;
}

// 68K 0x10118ddc HasObject__11CGameObjectF11eObjectType
// FUNCTION: LEMBALL 0x0040aa60
bool GameObject::HasObject(eObjectType p_arg0)
{
	return 0;
}

// 68K 0x10118e10 AddObject__11CGameObjectF11eObjectTypeP11CGameObject
// FUNCTION: LEMBALL 0x0040aa70
bool GameObject::AddObject(eObjectType p_arg0, class GameObject* p_arg1)
{
	return 0;
}

// 68K 0x10118e52 ReNumberNode__11CGameObjectFii
// FUNCTION: LEMBALL 0x0040aa80
void GameObject::ReNumberNode(int p_arg0, int p_arg1)
{
}

// 68K 0x10118e7c NeedsNode__11CGameObjectFi
// FUNCTION: LEMBALL 0x0040aa90
bool GameObject::NeedsNode(int p_arg0)
{
	return 0;
}

// 68K 0x10118ea4 ConvertVer0ToVer1__11CGameObjectFv
// STUB: LEMBALL 0x0040aaa0
unsigned int GameObject::ConvertVer0ToVer1()
{
	return 0;
}

// 68K 0x10118ed2 Delete__11CGameObjectFv
// FUNCTION: LEMBALL 0x0040aab0
void GameObject::Delete()
{
}

// 68K 0x10118ef4 PickUpAmmo__11CGameObjectFUs
// FUNCTION: LEMBALL 0x0040aac0
void GameObject::PickUpAmmo(unsigned short p_arg0)
{
}

// 68K 0x10118f1c ExternalControlEnd__11CGameObjectFv
// FUNCTION: LEMBALL 0x0040aad0
void GameObject::ExternalControlEnd()
{
}

// 68K 0x10118f4c RequestBalloon__11CGameObjectFv
// FUNCTION: LEMBALL 0x0040aae0
void GameObject::RequestBalloon()
{
	m_balloonPostActive = 1;
}

// 68K 0x10118f82 StartBalloon__11CGameObjectFv
// FUNCTION: LEMBALL 0x0040aaf0
void GameObject::StartBalloon()
{
	m_balloonPostActive = 0;
}

// 68K 0x10118fb6 OnBalloon__11CGameObjectFv
// FUNCTION: LEMBALL 0x0040ab00
void GameObject::OnBalloon()
{
	m_balloonPostActive = 0;
}

// 68K 0x10118fe8 QOnBalloon__11CGameObjectFv
// FUNCTION: LEMBALL 0x0040ab10
int GameObject::QOnBalloon()
{
	return m_balloonPostActive;
}

// 68K 0x10119024 OnConveyor__11CGameObjectFUcP4CIceUc
// FUNCTION: LEMBALL 0x0040ab20
void GameObject::OnConveyor(unsigned int p_arg0, Ice* p_arg1, unsigned int p_arg2)
{
}

// 68K 0x10119054 OnConveyor__11CGameObjectFv
// FUNCTION: LEMBALL 0x0040ab30
int GameObject::OnConveyor()
{
	return 0;
}

// 68K 0x1011907c Conveyor__11CGameObjectFv
// FUNCTION: LEMBALL 0x0040ab40
Ice* GameObject::Conveyor()
{
	return 0;
}

// 68K 0x10119118 IsUsable__11CGameObjectF7eAction
// STUB: LEMBALL 0x0040ab50
bool GameObject::IsUsable(eAction p_action)
{
	return 0;
}

// 68K 0x101187ea Action__11CGameObjectF7eAction
// FUNCTION: LEMBALL 0x0040c170
void GameObject::Action(eAction p_arg0)
{
	m_action = p_arg0;
}

// 68K 0x1011881e Action__11CGameObjectF7eActioni
// STUB: LEMBALL 0x0040c180
void GameObject::Action(eAction p_arg0, int p_arg1)
{
}

// 68K 0x101190a2 SendRemove__11CGameObjectFv
// FUNCTION: LEMBALL 0x0040c1a0
void GameObject::SendRemove()
{
}

// 68K 0x101190c8 SendCancel__11CGameObjectFv
// FUNCTION: LEMBALL 0x0040c1b0
void GameObject::SendCancel()
{
}

// 68K 0x101190ee UsableState__11CGameObjectFv
// FUNCTION: LEMBALL 0x0040c1c0
int GameObject::UsableState()
{
	return 2;
}

// 68K 0x10608fec __ct__11CGameObjectF11eObjectTypeUsUs
// STUB: LEMBALL 0x00414f30
GameObject::GameObject(eObjectType p_arg0, unsigned short p_arg1, unsigned short p_arg2)
{
}

// 68K 0x10609206 Restart__11CGameObjectFv
// FUNCTION: LEMBALL 0x004150d0
void GameObject::Restart()
{
	m_position = m_spawnPosition;
	m_auxiliaryPosition.m_xFixed = 0;
	m_auxiliaryPosition.m_yFixed = 0;
	m_auxiliaryPosition.m_zFixed = 0;
	Initialise();
	m_position.m_zFixed = 0;
	m_unk0x120 = (undefined2) 0xffff;
	m_unk0x110 = (undefined4) 0xffff;
	m_position.m_yFixed = 0;
	m_position.m_xFixed = 0;
	if (m_destinationList != 0) {
		m_destinationList->m_count = 0;
	}
	switch (m_objectType) {
	case (eObjectType) 1:
		m_runtimeFlags = 0x200;
		break;
	case (eObjectType) 2:
		m_runtimeFlags = 0x100;
		break;
	}
}

// 68K 0x1060936a Initialise__11CGameObjectFv
// FUNCTION: LEMBALL 0x004151b0
void GameObject::Initialise()
{
	m_actionArgument = 0;
	m_unk0x2c = 0;
	m_action = (eAction) 0;
	m_isRemoteObject = 0;
	m_facingDirection = 0;
	m_heading = 0;
	m_unk0xc4 = 0;
	m_unk0x3c = 0;
	m_isFlying = 0;
	m_unk0xc0 = 0;
	m_unk0x8c = 0;
	m_routeSearchFailed = 0;
	m_routeSearchActive = 0;
	m_unk0x104 = 0;
	m_unk0x108 = 0;
	m_unk0x10c = 0;
	m_balloonPostActive = 0;
	m_balloonPostId = 0;
	m_flightVelocity.m_xFixed = 0;
	m_flightVelocity.m_yFixed = 0;
	m_desiredFacingDirection = 0;
	m_flightVelocity.m_zFixed = 0;
	m_unk0x58 = 0;
	m_activator = 0;
	m_unk0x11c = 0;
	m_hasDestination = 0;
	m_actionDeadline = g_dwGameTick;
	m_soundEffect = (eSoundEffect) 0;
	m_transientFlags = 0;
}

// 68K 0x106093e4 StartFly__11CGameObjectFR9C3DVectorP9C3DVector
// STUB: LEMBALL 0x00415240
void GameObject::StartFly(C3DVector& p_arg0, C3DVector* p_arg1)
{
}

// 68K 0x106094cc Fly__11CGameObjectFv
// STUB: LEMBALL 0x00415300
void GameObject::Fly()
{
}

// 68K 0x106097cc RotateClockwise__11CGameObjectFv
// FUNCTION: LEMBALL 0x00415520
void GameObject::RotateClockwise()
{
	m_facingDirection++;
	if (m_facingDirection >= 8) {
		m_facingDirection %= 8;
	}
}

// 68K 0x1060980e RotateAnticlockwise__11CGameObjectFv
// FUNCTION: LEMBALL 0x00415550
void GameObject::RotateAnticlockwise()
{
	m_facingDirection--;
	if (m_facingDirection < 0) {
		m_facingDirection = (m_facingDirection + 8) % 8;
	}
}

// 68K 0x10609852 StartMoving__11CGameObjectFv
// STUB: LEMBALL 0x00415580
void GameObject::StartMoving()
{
}

// 68K 0x10609ab0 StopMoving__11CGameObjectFv
// FUNCTION: LEMBALL 0x00415780
void GameObject::StopMoving()
{
	DeleteFirstEntryFromDestinationList();
	m_moveDurationTicks = 0;
	m_actionDeadline = g_dwGameTick;
	m_lastMovementTick = g_dwGameTick;
}

// 68K 0x10609afc MapCheck__11CGameObjectFii
// STUB: LEMBALL 0x004157b0
unsigned short GameObject::MapCheck(int p_arg0, int p_arg1)
{
	return 0;
}

// 68K 0x10609bba StartRoute__11CGameObjectFv
// STUB: LEMBALL 0x00415830
bool GameObject::StartRoute()
{
	return 0;
}

// 68K 0x10609c9c SearchRoute__11CGameObjectFv
// STUB: LEMBALL 0x004158b0
bool GameObject::SearchRoute()
{
	return 0;
}

// 68K 0x10609ed2 Blocked__11CGameObjectFv
// FUNCTION: LEMBALL 0x00415a20
void GameObject::Blocked()
{
	if ((m_collisionFlags & 1) != 0) {
		m_routeSearchFailed = 1;
	}
}

// 68K 0x10609f0c Move__11CGameObjectFv
// STUB: LEMBALL 0x00415a30
bool GameObject::Move()
{
	return 0;
}

// 68K 0x1060a266 TurnToFaceDestination__11CGameObjectFv
// STUB: LEMBALL 0x00415d90
void GameObject::TurnToFaceDestination()
{
}

// 68K 0x1060a34a FacingDestination__11CGameObjectFv
// STUB: LEMBALL 0x00415e20
bool GameObject::FacingDestination()
{
	return 0;
}

// 68K 0x1060a3e0 DeleteFirstEntryFromDestinationList__11CGameObjectFv
// STUB: LEMBALL 0x00415e80
void GameObject::DeleteFirstEntryFromDestinationList()
{
}

// 68K 0x1060a4a4 AddDestination__11CGameObjectFRC7AICOORD
// STUB: LEMBALL 0x00415ef0
void GameObject::AddDestination(const AiCoord& p_arg0)
{
}

// 68K 0x1060a51c AlterDestination__11CGameObjectFRC7AICOORD
// STUB: LEMBALL 0x00415f30
void GameObject::AlterDestination(const AiCoord& p_arg0)
{
}

// 68K 0x1060a626 GetDestination__11CGameObjectFv
// STUB: LEMBALL 0x00416000
AiCoord* GameObject::GetDestination()
{
	return 0;
}

// 68K 0x1060a6b0 DestinationExists__11CGameObjectFv
// STUB: LEMBALL 0x004160c0
bool GameObject::DestinationExists()
{
	return 0;
}

// 68K 0x1060a6f2 EmptyDestinationList__11CGameObjectFv
// FUNCTION: LEMBALL 0x004160e0
void GameObject::EmptyDestinationList()
{
	*(unsigned short*) m_destinationList = 0;
}

// 68K 0x1060a72e GetBoundingBox__11CGameObjectFR7CVSRect
// STUB: LEMBALL 0x004160f0
void GameObject::GetBoundingBox(VsRect& p_rect)
{
}

// 68K 0x1060a796 Jump__11CGameObjectFv
// STUB: LEMBALL 0x00416130
bool GameObject::Jump()
{
	return 0;
}

// 68K 0x1060a8c6 Fall__11CGameObjectFv
// STUB: LEMBALL 0x00416220
bool GameObject::Fall()
{
	return 0;
}

// 68K 0x1060aaac OnLift__11CGameObjectFR8tCoord3d
// STUB: LEMBALL 0x00416340
bool GameObject::OnLift(Coord3d& p_arg0)
{
	return 0;
}

// 68K 0x1060ab76 OffLift__11CGameObjectFR8tCoord3d
// STUB: LEMBALL 0x00416410
void GameObject::OffLift(Coord3d& p_arg0)
{
}

// 68K 0x1060abb8 OnLift__11CGameObjectFR8tCoord3dR8tCoord3d
// STUB: LEMBALL 0x00416420
bool GameObject::OnLift(Coord3d& p_arg0, Coord3d& p_arg1)
{
	return 0;
}

// 68K 0x1060ac8e OffLift__11CGameObjectFR8tCoord3dR8tCoord3d
// STUB: LEMBALL 0x004164f0
void GameObject::OffLift(Coord3d& p_arg0, Coord3d& p_arg1)
{
}

// 68K 0x1060acde StartSommersault__11CGameObjectFv
// STUB: LEMBALL 0x00416510
void GameObject::StartSommersault()
{
}

// 68K 0x1060ad78 IsSelectable__11CGameObjectFv
// STUB: LEMBALL 0x00416570
bool GameObject::IsSelectable()
{
	return 0;
}

// 68K 0x1060adc0 ResetInstructions__11CGameObjectFv
// STUB: LEMBALL 0x00416590
bool GameObject::ResetInstructions()
{
	return 0;
}

// 68K 0x1060ae5c Init__11CGameObjectFP3CAI
// STUB: LEMBALL 0x004165e0
void GameObject::Init(Ai* p_arg0)
{
}

// 68K 0x1060aeb2 GetId__11CGameObjectFv
// FUNCTION: LEMBALL 0x00416610
short GameObject::GetId()
{
	return m_linkedObjectId;
}

// 68K 0x1060aedc SetId__11CGameObjectFUs
// STUB: LEMBALL 0x00416620
void GameObject::SetId(unsigned short p_arg0)
{
}

// 68K 0x1060af12 ReSetId__11CGameObjectFv
// STUB: LEMBALL 0x00416640
void GameObject::ReSetId()
{
}

// 68K 0x1060af92 NextId__11CGameObjectFv
// FUNCTION: LEMBALL 0x00416670
int GameObject::NextId()
{
	return 0;
}

// 68K 0x1060b006 NextLoadingId__11CGameObjectFv
// STUB: LEMBALL 0x004166a0
int GameObject::NextLoadingId()
{
	return 0;
}

// 68K 0x1060b080 RegisterId__11CGameObjectFv
// STUB: LEMBALL 0x00416740
void GameObject::RegisterId()
{
}

// 68K 0x1060b144 UpdateCollision__11CGameObjectFv
// STUB: LEMBALL 0x004167c0
void GameObject::UpdateCollision()
{
}

// 68K 0x1060b1e6 StartLand__11CGameObjectFv
// STUB: LEMBALL 0x00416820
void GameObject::StartLand()
{
}

// 68K 0x10118886 Process__11CGameObjectFv
// FUNCTION: LEMBALL 0x00417aa0
bool GameObject::Process()
{
	return 0;
}

// 68K 0x106092da __dt__11CGameObjectFv
GameObject::~GameObject()
{
}

// GLOBAL: LEMBALL 0x0049cf4c
word g_wNetworkLemmingIndex = 0;

// GLOBAL: LEMBALL 0x0049cf50
word g_wLocalLemmingIndex = 0;

// GLOBAL: LEMBALL 0x0049d070
int g_anTurnDelayCursor[16] = {0, 30, 20, 12, 0, 0, 0, 15, 32, 0, 0, 0, 0, 0, 0, 0};

// GLOBAL: LEMBALL 0x0049d0b0
int g_anTurnDelayTarget[16] = {0, 87, 75, 0, 0, 0, 0, 75, 75, 0, 0, 0, 0, 0, 0, 0};

// GLOBAL: LEMBALL 0x0049d108
unsigned char g_abBitMasks[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

// GLOBAL: LEMBALL 0x004a640c
int g_wLemmingCount;

// GLOBAL: LEMBALL 0x004a6410
unsigned char g_abObjectIdBitmap[32];

// GLOBAL: LEMBALL 0x004a6510
GameObject* g_pObjects[256];

// GLOBAL: LEMBALL 0x004a74bc
word g_wObjectCount;

