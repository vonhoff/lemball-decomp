#ifndef LEMBALL_CGENERICGROUP_H
#define LEMBALL_CGENERICGROUP_H

#include "AI/CGameObject.h"

class CFormationManager;
class CViewData;
struct CGenericGroupVtable;

class CGenericGroup {
public:
	CGenericGroupVtable* m_pVtable00;
	char m_abReserved04[0x120];
	int m_nElementCount124;
	CGameObject* m_apElements128[10];
	int m_nCurrentElement150;
	char m_abReserved154[4];
	CVSRect m_BoundingBox158;

	void Restart(void);
	int Process(void);
	int GetElementsInGroup(void);
	CGameObject* GetFirstElementInGroup(void);
	CGameObject* GetNextElementInGroup(void);
	CGameObject* GetCurrentElementInGroup(void);
	CGameObject* GetNthElementInGroup(int nElement);
	void SwapElements(CGameObject* pFirst, CGameObject* pSecond);
	void AddElementToGroup(CGameObject* pObject);
	void RemoveElementFromGroup(CGameObject* pObject);
	int ConfirmElementIsInGroup(CGameObject* pObject);
	int ConfirmElementIsInGroup(unsigned short nId);
	CVSRect GetBoundingBox(void);
	void GetBoundingBox(CVSRect& rect);
	void CalculateBoundingBox(int nRadius);
	void AddNewWaypoint(AICOORD position, CFormationManager* pFormationManager);
	void SendNewWaypoint(AICOORD position);
	void OverideExistingWaypoints(AICOORD position);
	void ClearExistingWaypoints(void);
	void SetFormationIndex(int nIndex);
	int GetFormationIndex(void);
	int CheckGroupIntersection(CVSRect* pRect, AICOORD* pPosition);
	int GetViewData(CViewData* pViewData);
};

typedef int (CGenericGroup::*GenericGroupContainsProc)(CGameObject* pObject);
typedef void (CGenericGroup::*GenericGroupDeleteProc)(int fFree);
typedef int (CGenericGroup::*GenericGroupCountProc)(void);
typedef CGameObject* (CGenericGroup::*GenericGroupIteratorProc)(void);
typedef void (CGenericGroup::*GenericGroupElementProc)(CGameObject* pObject);
typedef void (CGenericGroup::*GenericGroupBoundsProc)(short* pBounds);
typedef void (CGenericGroup::*GenericGroupWaypointProc)(AICOORD position, CFormationManager* pFormationManager);
typedef void (CGenericGroup::*GenericGroupNoArgProc)(void);
typedef int (CGenericGroup::*GenericGroupViewDataProc)(CViewData* pViewData);
typedef int (CGenericGroup::*GenericGroupIntersectionProc)(CVSRect* pRect, AICOORD* pPosition);

struct CGenericGroupChunkVtable {
	void* m_apSlots00[5];
	int (CGenericGroup::*m_pProcess14)(void);
	void* m_apSlots18[10];
};

struct CGenericGroupVtable {
	GenericGroupDeleteProc m_pDelete00;
	void* m_apReserved04[17];
	GenericGroupBoundsProc m_pGetBoundingBox48;
	void* m_apReserved4C[47];
	GenericGroupCountProc m_pGetNumberOfElements108;
	void* m_apReserved10C[2];
	GenericGroupIteratorProc m_pGetFirstElement114;
	GenericGroupIteratorProc m_pGetNextElement118;
	GenericGroupIteratorProc m_pGetCurrentElement11C;
	void* m_pReserved120;
	GenericGroupElementProc m_pAddElement124;
	GenericGroupElementProc m_pRemoveElement128;
	void* m_pReserved12C;
	GenericGroupContainsProc m_pContains130;
	void* m_apReserved134[4];
	GenericGroupWaypointProc m_pAddNewWaypoint144;
	void* m_apReserved148[2];
	GenericGroupNoArgProc m_pClearExistingWaypoints150;
	void* m_pReserved154;
	GenericGroupViewDataProc m_pGetViewData158;
	GenericGroupIntersectionProc m_pCheckIntersection15C;
};

#endif
