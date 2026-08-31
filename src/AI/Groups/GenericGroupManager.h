#ifndef LEMBALL_AI_GROUPS_GENERICGROUPMANAGER_H
#define LEMBALL_AI_GROUPS_GENERICGROUPMANAGER_H

#include "../../Common.h"

// SIZE 0xb0
// VTABLE: LEMBALL 0x00494d08
class GenericGroupManager {
public:
	GenericGroupManager(Ai* p_arg0, ObjectManager* p_arg1, FormationManager* p_arg2);
	int Process();
	virtual int GetNumberOfGroups();                                                  // vtable+0x00
	virtual GenericGroup* GetFirstGroup();                                            // vtable+0x04
	virtual GenericGroup* GetNextGroup();                                             // vtable+0x08
	virtual GenericGroup* GetNthGroup(int p_index);                                   // vtable+0x0c
	virtual GenericGroup* GetCurrentGroup();                                          // vtable+0x10
	virtual void DeleteGroup(GenericGroup* p_group);                                  // vtable+0x14
	virtual int GetNumberOfElements();                                                // vtable+0x18
	virtual GameObject* GetFirstElement();                                            // vtable+0x1c
	virtual GameObject* GetNextElement();                                             // vtable+0x20
	virtual GameObject* GetCurrentElement();                                          // vtable+0x24
	virtual GameObject* GetNthElement(int p_index);                                   // vtable+0x28
	virtual GenericGroup* GetGroupElementIsMemberOf(GameObject* p_object);            // vtable+0x2c
	virtual void AddNewGroup(GenericGroup* p_group);                                  // vtable+0x30
	virtual void CreateNewGroup();                                                    // vtable+0x38
	virtual void CreateNewGroup(unsigned short p_count, unsigned short* p_objectIds); // vtable+0x34
	virtual void AddElementToGroup(GameObject* p_object, GenericGroup* p_group);      // vtable+0x3c
	virtual bool RemoveElementFromGroup(GameObject* p_object, GenericGroup* p_group); // vtable+0x40
	virtual void FindElementInGroupAndRemoveIt(GameObject* p_object);                 // vtable+0x44
	virtual int GetAllBoundingBoxes(Rect* p_rects);                                   // vtable+0x48
	virtual int GetViewData(ViewData* p_viewData);                                    // vtable+0x4c
	virtual bool CheckGroupIntersection(VsRect* p_rect, AiCoord* p_coordinate);       // vtable+0x50
	void Restart();
	~GenericGroupManager();
	GenericGroupManager();

protected:
	GenericGroup* m_groups[40]; // 0x04
	int m_groupCount;           // 0xa4
	int m_currentGroup;         // 0xa8
	int m_state;                // 0xac
};

// SYNTHETIC: LEMBALL 0x0041e940
// GenericGroupManager::`scalar deleting destructor'

#endif
