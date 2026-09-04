#ifndef LEMBALL_AI_GROUPS_GENERICGROUP_H
#define LEMBALL_AI_GROUPS_GENERICGROUP_H

#include "../../Common.h"
#include "../../Visos/Foundation/VsRect.h" // complete type
#include "../Base/AiCoord.h"               // complete type
#include "../Base/GameObject.h"            // complete type

// SIZE 0x168
// VTABLE: LEMBALL 0x00494b60
class GenericGroup : public GameObject {
public:
	GenericGroup(Ai* p_arg0, ObjectManager* p_arg1, FormationManager* p_arg2, int p_arg3, int p_arg4);
	virtual ~GenericGroup();                                                                 // vtable+0x00
	virtual bool Process();                                                                  // vtable+0x14
	virtual void Restart();                                                                  // vtable+0x104
	virtual int GetElementsInGroup();                                                        // vtable+0x108
	virtual int GetFormationIndex();                                                         // vtable+0x10c
	virtual void SetFormationIndex(int p_formationIndex);                                    // vtable+0x110
	virtual GameObject* GetFirstElementInGroup();                                            // vtable+0x114
	virtual GameObject* GetNextElementInGroup();                                             // vtable+0x118
	virtual GameObject* GetCurrentElementInGroup();                                          // vtable+0x11c
	virtual GameObject* GetNthElementInGroup(int p_index);                                   // vtable+0x120
	virtual void AddElementToGroup(GameObject* p_object);                                    // vtable+0x124
	virtual void RemoveElementFromGroup(GameObject* p_object);                               // vtable+0x128
	virtual bool ConfirmElementIsInGroup(GameObject* p_object);                              // vtable+0x130
	virtual bool ConfirmElementIsInGroup(unsigned short p_objectId);                         // vtable+0x12c
	virtual VsRect GetBoundingBox();                                                         // vtable+0x134
	virtual void CalculateBoundingBox(int p_radius);                                         // vtable+0x138
	virtual eGroupState GetGroupState();                                                     // vtable+0x13c
	virtual void SetGroupState(eGroupState p_state);                                         // vtable+0x140
	virtual void AddNewWaypoint(AiCoord p_coordinate, FormationManager* p_formationManager); // vtable+0x144
	virtual void SendNewWaypoint(AiCoord p_coordinate);                                      // vtable+0x148
	virtual void OverideExistingWaypoints(AiCoord p_coordinate);                             // vtable+0x14c
	virtual void ClearExistingWaypoints();                                                   // vtable+0x150
	virtual void ReformAlteredGroup(FormationManager* p_formationManager);                   // vtable+0x154
	virtual void GetViewData(ViewData* p_viewData);                                          // vtable+0x158
	virtual bool CheckGroupIntersection(VsRect* p_rect, AiCoord* p_coordinate);              // vtable+0x15c
	virtual void GetBoundingBox(VsRect& p_rect);                                             // vtable+0x48
	void SwapElements(GameObject* p_first, GameObject* p_second);
	GenericGroup();

protected:
	int m_elementCount;         // 0x124
	GameObject* m_elements[10]; // 0x128
	int m_currentElement;       // 0x150
	eGroupState m_groupState;   // 0x154
	short m_minX;               // 0x158
	short m_minY;               // 0x15a
	short m_maxX;               // 0x15c
	short m_maxY;               // 0x15e
	int m_formationIndex;       // 0x160
	int m_altered;              // 0x164
};

// SYNTHETIC: LEMBALL 0x0041e8c0
// GenericGroup::`scalar deleting destructor'

#endif
