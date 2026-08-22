#include "GenericGroup.h"

GenericGroup::GenericGroup()
{
}

// 68K 0x10118774 GetGroupState__13CGenericGroupFv
// STUB: LEMBALL 0x00414c60
eGroupState GenericGroup::GetGroupState()
{
	return *(eGroupState*) 0;
}

// 68K 0x101187a8 SetGroupState__13CGenericGroupF11eGroupState
// STUB: LEMBALL 0x00414c70
void GenericGroup::SetGroupState(eGroupState p_state)
{
}

// 68K 0x1060c298 __ct__13CGenericGroupFP3CAIP14CObjectManagerP17CFormationManagerii
// STUB: LEMBALL 0x0041dda0
GenericGroup::GenericGroup(Ai* p_arg0, ObjectManager* p_arg1, FormationManager* p_arg2, int p_arg3, int p_arg4)
{
}

// 68K 0x1060c44c Restart__13CGenericGroupFv
// STUB: LEMBALL 0x0041de80
void GenericGroup::Restart()
{
}

// 68K 0x1060c4a0 Process__13CGenericGroupFv
// STUB: LEMBALL 0x0041deb0
bool GenericGroup::Process()
{
	return 0;
}

// 68K 0x1060c50a GetElementsInGroup__13CGenericGroupFv
// STUB: LEMBALL 0x0041def0
int GenericGroup::GetElementsInGroup()
{
	return 0;
}

// 68K 0x1060c544 GetFirstElementInGroup__13CGenericGroupFv
// STUB: LEMBALL 0x0041df00
GameObject* GenericGroup::GetFirstElementInGroup()
{
	return 0;
}

// 68K 0x1060c588 GetNextElementInGroup__13CGenericGroupFv
// STUB: LEMBALL 0x0041df20
GameObject* GenericGroup::GetNextElementInGroup()
{
	return 0;
}

// 68K 0x1060c5de GetCurrentElementInGroup__13CGenericGroupFv
// STUB: LEMBALL 0x0041df40
GameObject* GenericGroup::GetCurrentElementInGroup()
{
	return 0;
}

// 68K 0x1060c634 GetNthElementInGroup__13CGenericGroupFi
// STUB: LEMBALL 0x0041df60
GameObject* GenericGroup::GetNthElementInGroup(int p_index)
{
	return 0;
}

// 68K 0x1060c68c SwapElements__13CGenericGroupFP11CGameObjectP11CGameObject
// STUB: LEMBALL 0x0041dfc0
void GenericGroup::SwapElements(GameObject* p_first, GameObject* p_second)
{
}

// 68K 0x1060c71c AddElementToGroup__13CGenericGroupFP11CGameObject
// STUB: LEMBALL 0x0041e020
void GenericGroup::AddElementToGroup(GameObject* p_object)
{
}

// 68K 0x1060c776 RemoveElementFromGroup__13CGenericGroupFP11CGameObject
// STUB: LEMBALL 0x0041e050
void GenericGroup::RemoveElementFromGroup(GameObject* p_object)
{
}

// 68K 0x1060c814 ConfirmElementIsInGroup__13CGenericGroupFP11CGameObject
// STUB: LEMBALL 0x0041e0c0
bool GenericGroup::ConfirmElementIsInGroup(GameObject* p_object)
{
	return 0;
}

// 68K 0x1060c880 ConfirmElementIsInGroup__13CGenericGroupFUs
// STUB: LEMBALL 0x0041e100
bool GenericGroup::ConfirmElementIsInGroup(unsigned short p_objectId)
{
	return 0;
}

// 68K 0x1060c8f0 GetBoundingBox__13CGenericGroupFv
// STUB: LEMBALL 0x0041e140
VsRect GenericGroup::GetBoundingBox()
{
	return *(VsRect*) 0;
}

// 68K 0x1060c974 GetBoundingBox__13CGenericGroupFR7CVSRect
// STUB: LEMBALL 0x0041e180
void GenericGroup::GetBoundingBox(VsRect& p_rect)
{
}

// 68K 0x1060c9dc CalculateBoundingBox__13CGenericGroupFi
// STUB: LEMBALL 0x0041e1c0
void GenericGroup::CalculateBoundingBox(int p_radius)
{
}

// 68K 0x1060cb04 AddNewWaypoint__13CGenericGroupF7AICOORDP17CFormationManager
// STUB: LEMBALL 0x0041e290
void GenericGroup::AddNewWaypoint(AiCoord p_coordinate, FormationManager* p_formationManager)
{
}

// 68K 0x1060cb96 SendNewWaypoint__13CGenericGroupF7AICOORD
// STUB: LEMBALL 0x0041e2e0
void GenericGroup::SendNewWaypoint(AiCoord p_coordinate)
{
}

// 68K 0x1060cd04 OverideExistingWaypoints__13CGenericGroupF7AICOORD
// STUB: LEMBALL 0x0041e3c0
void GenericGroup::OverideExistingWaypoints(AiCoord p_coordinate)
{
}

// 68K 0x1060cd42 ClearExistingWaypoints__13CGenericGroupFv
// STUB: LEMBALL 0x0041e3d0
void GenericGroup::ClearExistingWaypoints()
{
}

// 68K 0x1060cdbc SetFormationIndex__13CGenericGroupFi
// STUB: LEMBALL 0x0041e400
void GenericGroup::SetFormationIndex(int p_formationIndex)
{
}

// 68K 0x1060cdf6 GetFormationIndex__13CGenericGroupFv
// STUB: LEMBALL 0x0041e410
int GenericGroup::GetFormationIndex()
{
	return 0;
}

// 68K 0x1060ce2e ReformAlteredGroup__13CGenericGroupFP17CFormationManager
// STUB: LEMBALL 0x0041e420
void GenericGroup::ReformAlteredGroup(FormationManager* p_formationManager)
{
}

// 68K 0x1060cff2 CheckGroupIntersection__13CGenericGroupFP7CVSRectP7AICOORD
// STUB: LEMBALL 0x0041e530
bool GenericGroup::CheckGroupIntersection(VsRect* p_rect, AiCoord* p_coordinate)
{
	return 0;
}

// 68K 0x1060d14a GetViewData__13CGenericGroupFP9CViewData
// STUB: LEMBALL 0x0041e640
void GenericGroup::GetViewData(ViewData* p_viewData)
{
}

// 68K 0x1060c3aa __dt__13CGenericGroupFv
// SYNTHETIC: LEMBALL 0x0041e8c0
// GenericGroup::`scalar deleting destructor'
GenericGroup::~GenericGroup()
{
}
