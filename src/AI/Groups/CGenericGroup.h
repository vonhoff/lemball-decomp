#ifndef LEMBALL_AI_GROUPS_CGENERICGROUP_H
#define LEMBALL_AI_GROUPS_CGENERICGROUP_H

#include "../../Visos/Foundation/CVsRect.h"
#include "../Base/AiCoord.h"
#include "../Base/CGameObject.h"

class CAI;
class CFormationManager;
class CObjectManager;
class CViewData;
enum eGroupState {
	GROUP_STATE_IDLE = 0,
	GROUP_STATE_MOVING = 1,
	GROUP_STATE_ATTACKING = 2,
	GROUP_STATE_USING_OBJECT = 3
};

// SIZE 0x168
// VTABLE: LEMBALL 0x00494b60
class CGenericGroup : public CGameObject {
public:
	CGenericGroup(CAI* p_ai, CObjectManager* p_objectManager, CFormationManager* p_formationManager);
	virtual ~CGenericGroup();                                                                 // vtable+0x00
	virtual bool Process();                                                                   // vtable+0x14
	virtual void Restart();                                                                   // vtable+0x104
	virtual int GetElementsInGroup();                                                         // vtable+0x108
	virtual int GetFormationIndex();                                                          // vtable+0x10c
	virtual void SetFormationIndex(int p_formationIndex);                                     // vtable+0x110
	virtual CGameObject* GetFirstElementInGroup();                                            // vtable+0x114
	virtual CGameObject* GetNextElementInGroup();                                             // vtable+0x118
	virtual CGameObject* GetCurrentElementInGroup();                                          // vtable+0x11c
	virtual CGameObject* GetNthElementInGroup(int p_index);                                   // vtable+0x120
	virtual void AddElementToGroup(CGameObject* p_object);                                    // vtable+0x124
	virtual void RemoveElementFromGroup(CGameObject* p_object);                               // vtable+0x128
	virtual bool ConfirmElementIsInGroup(CGameObject* p_object);                              // vtable+0x130
	virtual bool ConfirmElementIsInGroup(unsigned short p_objectId);                          // vtable+0x12c
	virtual CVsRect GetBoundingBox();                                                         // vtable+0x134
	virtual void CalculateBoundingBox(int p_radius);                                          // vtable+0x138
	virtual eGroupState GetGroupState();                                                      // vtable+0x13c
	virtual void SetGroupState(eGroupState p_state);                                          // vtable+0x140
	virtual void AddNewWaypoint(AiCoord p_coordinate, CFormationManager* p_formationManager); // vtable+0x144
	virtual void SendNewWaypoint(AiCoord p_coordinate);                                       // vtable+0x148
	virtual void OverideExistingWaypoints(AiCoord p_coordinate);                              // vtable+0x14c
	virtual void ClearExistingWaypoints();                                                    // vtable+0x150
	virtual void ReformAlteredGroup(CFormationManager* p_formationManager);                   // vtable+0x154
	virtual int GetViewData(CViewData* p_viewData);                                           // vtable+0x158
	virtual bool CheckGroupIntersection(CVsRect* p_rect, AiCoord* p_coordinate);              // vtable+0x15c
	virtual void GetBoundingBox(CVsRect& p_rect);                                             // vtable+0x48
	void SwapElements(CGameObject* p_first, CGameObject* p_second);
	CGenericGroup();

protected:
	int m_elementCount;          // 0x124
	CGameObject* m_elements[10]; // 0x128
	int m_currentElement;        // 0x150
	eGroupState m_groupState;    // 0x154
	CVsRect m_bounds;            // 0x158
	int m_formationIndex;        // 0x160
	int m_altered;               // 0x164
};

extern CObjectManager* g_pGroupObjectManager;
extern CFormationManager* g_pGroupFormationManager;
extern CAI* g_pGroupAI;

// SYNTHETIC: LEMBALL 0x0041e8c0
// CGenericGroup::`scalar deleting destructor'

#endif
