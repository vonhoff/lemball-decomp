#ifndef LEMBALL_AI_GROUPS_CGENERICGROUPMANAGER_H
#define LEMBALL_AI_GROUPS_CGENERICGROUPMANAGER_H

class CAi;
class AiCoord;
class CFormationManager;
class CGameObject;
class CGenericGroup;
class CObjectManager;
class CViewData;
class CVsRect;
struct Rect;
// SIZE 0xb0
// VTABLE: LEMBALL 0x00494d08
class CGenericGroupManager {
public:
	CGenericGroupManager(CAi* p_arg0, CObjectManager* p_arg1, CFormationManager* p_arg2);
	int Process();
	virtual int GetNumberOfGroups();                                                    // vtable+0x00
	virtual CGenericGroup* GetFirstGroup();                                             // vtable+0x04
	virtual CGenericGroup* GetNextGroup();                                              // vtable+0x08
	virtual CGenericGroup* GetNthGroup(int p_index);                                    // vtable+0x0c
	virtual CGenericGroup* GetCurrentGroup();                                           // vtable+0x10
	virtual void DeleteGroup(CGenericGroup* p_group);                                   // vtable+0x14
	virtual int GetNumberOfElements();                                                  // vtable+0x18
	virtual CGameObject* GetFirstElement();                                             // vtable+0x1c
	virtual CGameObject* GetNextElement();                                              // vtable+0x20
	virtual CGameObject* GetCurrentElement();                                           // vtable+0x24
	virtual CGameObject* GetNthElement(int p_index);                                    // vtable+0x28
	virtual CGenericGroup* GetGroupElementIsMemberOf(CGameObject* p_object);            // vtable+0x2c
	virtual void AddNewGroup(CGenericGroup* p_group);                                   // vtable+0x30
	virtual void CreateNewGroup();                                                      // vtable+0x38
	virtual void CreateNewGroup(unsigned short p_count, unsigned short* p_objectIds);   // vtable+0x34
	virtual void AddElementToGroup(CGameObject* p_object, CGenericGroup* p_group);      // vtable+0x3c
	virtual bool RemoveElementFromGroup(CGameObject* p_object, CGenericGroup* p_group); // vtable+0x40
	virtual void FindElementInGroupAndRemoveIt(CGameObject* p_object);                  // vtable+0x44
	virtual int GetAllBoundingBoxes(Rect* p_rects);                                     // vtable+0x48
	virtual int GetViewData(CViewData* p_viewData);                                     // vtable+0x4c
	virtual bool CheckGroupIntersection(CVsRect* p_rect, AiCoord* p_coordinate);        // vtable+0x50
	void Restart();
	~CGenericGroupManager();
	CGenericGroupManager();

protected:
	CGenericGroup* m_groups[40]; // 0x04
	int m_groupCount;            // 0xa4
	int m_currentGroup;          // 0xa8
	int m_state;                 // 0xac
};

#endif
