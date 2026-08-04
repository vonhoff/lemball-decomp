#include "AI/CGenericGroup.h"
#include "AI/CFormationManager.h"
#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

class CViewData;

extern unsigned int __cdecl compute_direction_octant_between_points(int nX1, int nY1, int nX2, int nY2);
extern void LEMBALL_FASTCALL ResetManagedEntityRuntimeStateThunk(void* pObject);

// FUNCTION: LEMBALL 0x0041de80
void CGenericGroup::Restart(void)
{
	ResetManagedEntityRuntimeStateThunk(this);
	for (int i = 0; i < 10; ++i) {
		m_apElements128[i] = 0;
	}
	m_nElementCount124 = 0;
}

// FUNCTION: LEMBALL 0x0041deb0
int CGenericGroup::Process(void)
{
	typedef void(LEMBALL_FASTCALL * GroupStateProc)(CGenericGroup*, void*, int);
	typedef void(LEMBALL_FASTCALL * ChildProcessProc)(CGameObject*);
	((GroupStateProc) (*(void***) this)[0x138 / sizeof(void*)])(this, 0, 0x18);
	for (int i = 0; i < m_nElementCount124; ++i) {
		CGameObject* pObject = m_apElements128[i];
		((ChildProcessProc) ((void**) pObject->m_pVtable00)[0x14 / sizeof(void*)])(pObject);
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0041def0
int CGenericGroup::GetElementsInGroup(void)
{
	return m_nElementCount124;
}

// FUNCTION: LEMBALL 0x0041df00
CGameObject* CGenericGroup::GetFirstElementInGroup(void)
{
	m_nCurrentElement150 = 0;
	return m_apElements128[0];
}

// FUNCTION: LEMBALL 0x0041df20
CGameObject* CGenericGroup::GetNextElementInGroup(void)
{
	++m_nCurrentElement150;
	if (m_nCurrentElement150 >= m_nElementCount124) {
		return 0;
	}
	return m_apElements128[m_nCurrentElement150];
}

// FUNCTION: LEMBALL 0x0041df40
CGameObject* CGenericGroup::GetCurrentElementInGroup(void)
{
	if (m_nCurrentElement150 >= m_nElementCount124) {
		return 0;
	}
	return m_apElements128[m_nCurrentElement150];
}

// FUNCTION: LEMBALL 0x0041df60
CGameObject* CGenericGroup::GetNthElementInGroup(int nElement)
{
	m_nCurrentElement150 = nElement;
	if (nElement >= m_nElementCount124) {
		return 0;
	}
	return m_apElements128[nElement];
}

// FUNCTION: LEMBALL 0x0041dfc0
void CGenericGroup::SwapElements(CGameObject* pFirst, CGameObject* pSecond)
{
	int i;
	int iFirst;
	int iSecond;
	CGameObject* pElement;

	iSecond = -1;
	for (i = 0; i < m_nElementCount124; ++i) {
		pElement = m_apElements128[i];
		if (pElement == pFirst) {
			iFirst = i;
		}
		if (pElement == pSecond) {
			iSecond = i;
		}
	}
	m_apElements128[iFirst] = pSecond;
	m_apElements128[iSecond] = pFirst;
}

// FUNCTION: LEMBALL 0x0041e020
void CGenericGroup::AddElementToGroup(CGameObject* pObject)
{
	m_apElements128[m_nElementCount124] = pObject;
	*(int*) ((char*) this + 0x164) = 1;
	++m_nElementCount124;
}

// FUNCTION: LEMBALL 0x0041e050
void CGenericGroup::RemoveElementFromGroup(CGameObject* pObject)
{
	int i;
	int j;

	for (i = 0; i < 10; ++i) {
		if (m_apElements128[i] == pObject) {
			if (i < 9) {
				for (j = i; j < 9; ++j) {
					m_apElements128[j] = m_apElements128[j + 1];
				}
				i = j;
			}
			m_apElements128[i] = 0;
			--m_nElementCount124;
			*(int*) ((char*) this + 0x164) = 1;
			return;
		}
	}
}

// FUNCTION: LEMBALL 0x0041e0c0
int CGenericGroup::ConfirmElementIsInGroup(CGameObject* pObject)
{
	int i;

	for (i = 0; i < m_nElementCount124; ++i) {
		if (m_apElements128[i] == pObject) {
			return 1;
		}
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0041e100
int CGenericGroup::ConfirmElementIsInGroup(unsigned short nId)
{
	int i;
	CGameObject* pObject;

	for (i = 0; i < m_nElementCount124; ++i) {
		pObject = m_apElements128[i];
		if (pObject != 0 && *(unsigned short*) ((char*) pObject + 0x6a) == nId) {
			return 1;
		}
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0041e140
CVSRect CGenericGroup::GetBoundingBox(void)
{
	return m_BoundingBox158;
}

// FUNCTION: LEMBALL 0x0041e180
void CGenericGroup::GetBoundingBox(CVSRect& rect)
{
	rect.width = m_BoundingBox158.width;
	rect.height = m_BoundingBox158.height;
	rect.x = m_BoundingBox158.x;
	rect.y = m_BoundingBox158.y;
}

// FUNCTION: LEMBALL 0x0041e1c0
void CGenericGroup::CalculateBoundingBox(int nRadius)
{
	int nMinX = 99999;
	int nMinY = 99999;
	int nMaxX = -1;
	int nMaxY = -1;

	for (int i = 0; i < m_nElementCount124; ++i) {
		CGameObject* pObject = m_apElements128[i];
		if (pObject != 0) {
			int x = pObject->m_WorldPosition9C.x >> 12;
			int y = pObject->m_WorldPosition9C.y >> 12;
			if (x - nRadius < nMinX) {
				nMinX = x - nRadius;
			}
			if (y - nRadius < nMinY) {
				nMinY = y - nRadius;
			}
			if (x + nRadius > nMaxX) {
				nMaxX = x + nRadius;
			}
			if (y + nRadius > nMaxY) {
				nMaxY = y + nRadius;
			}
		}
	}

	m_BoundingBox158.x = (short) nMinX;
	nMaxX = (short) nMaxX - (short) nMinX;
	nMaxY = (short) nMaxY - (short) nMinY;
	m_BoundingBox158.y = (short) nMinY;
	m_BoundingBox158.width = (short) nMaxX;
	m_BoundingBox158.height = (short) nMaxY;
}

CFormationManager* g_pGenericGroupFormationManager;

// FUNCTION: LEMBALL 0x0041e290
void CGenericGroup::AddNewWaypoint(AICOORD position, CFormationManager* pFormationManager)
{
	g_pGenericGroupFormationManager = pFormationManager;
	CGameObjectCommandQueue* pQueue = *(CGameObjectCommandQueue**) ((char*) this + 0x70);
	unsigned short nIndex = pQueue->m_cEntries;
	if (nIndex < pQueue->m_cCapacity) {
		pQueue->m_cEntries = nIndex + 1;
		CGameObjectCommand* pCommand = &pQueue->m_pEntries[nIndex];
		pCommand->m_nType = 1;
		pCommand->m_Position.x = position.x;
		pCommand->m_Position.y = position.y;
		pCommand->m_Position.z = position.z;
	}
}

// FUNCTION: LEMBALL 0x0041e2e0
void CGenericGroup::SendNewWaypoint(AICOORD position)
{
	typedef int(LEMBALL_FASTCALL * GroupCountProc)(CGenericGroup*);
	typedef CGameObject*(LEMBALL_FASTCALL * ElementIteratorProc)(CGenericGroup*);
	void** pVtable = *(void***) this;
	CGameObject* pObject = ((ElementIteratorProc) pVtable[0x114 / sizeof(void*)])(this);
	if (pObject != 0) {
		unsigned int nDirection = compute_direction_octant_between_points(pObject->m_WorldPosition9C.x >> 12,
																						 pObject->m_WorldPosition9C.y >> 12,
																						 position.x >> 12,
																						 position.y >> 12);
		g_pGenericGroupFormationManager->TransformFormation(
				*(int*) ((char*) this + 0x160), (nDirection - 2) << 6);
		int nElements = ((GroupCountProc) pVtable[0x108 / sizeof(void*)])(this);
		for (int i = 0; i < nElements; ++i) {
			CFormationVector* pVector = g_pGenericGroupFormationManager->GetAVector(i);
			AICOORD destination;
			destination.x = pVector->x + position.x;
			destination.y = pVector->y + position.y;
			destination.z = position.z;
			pObject->AddDestination(destination);
			pObject = ((ElementIteratorProc) (*(void***) this)[0x118 / sizeof(void*)])(this);
		}
	}
}

// FUNCTION: LEMBALL 0x0041e3c0
void CGenericGroup::OverideExistingWaypoints(AICOORD position)
{
	(void) position;
}

// FUNCTION: LEMBALL 0x0041e3d0
void CGenericGroup::ClearExistingWaypoints(void)
{
	typedef CGameObject*(LEMBALL_FASTCALL * ElementIteratorProc)(CGenericGroup*);
	void** pVtable = *(void***) this;
	*(unsigned short*) *(void**) ((char*) this + 0x70) = 0;
	CGameObject* pObject = ((ElementIteratorProc) pVtable[0x114 / sizeof(void*)])(this);
	if (pObject != 0) {
		ElementIteratorProc pGetNext = (ElementIteratorProc) pVtable[0x118 / sizeof(void*)];
		do {
			pObject->ResetInstructions();
			pObject = pGetNext(this);
		} while (pObject != 0);
	}
}

// FUNCTION: LEMBALL 0x0041e400
void CGenericGroup::SetFormationIndex(int nIndex)
{
	*(int*) ((char*) this + 0x160) = nIndex;
}

// FUNCTION: LEMBALL 0x0041e410
int CGenericGroup::GetFormationIndex(void)
{
	return *(int*) ((char*) this + 0x160);
}

// FUNCTION: LEMBALL 0x0041e530
int CGenericGroup::CheckGroupIntersection(CVSRect* pRect, AICOORD* pPosition)
{
	typedef CGameObject*(LEMBALL_FASTCALL * ElementIteratorProc)(CGenericGroup*);
	int nLeft = pRect->x;
	int nTop = pRect->y;
	int nRight = nLeft + pRect->width;
	int nBottom = nTop + pRect->height;
	if (m_BoundingBox158.x < nRight && nLeft < m_BoundingBox158.x + m_BoundingBox158.width &&
			m_BoundingBox158.y < nBottom && nTop < m_BoundingBox158.y + m_BoundingBox158.height) {
		CGameObject* pObject = ((ElementIteratorProc) (*(void***) this)[0x114 / sizeof(void*)])(this);
		while (pObject != 0) {
			int nObjectX = pObject->m_WorldPosition9C.x >> 12;
			int nObjectY = pObject->m_WorldPosition9C.y >> 12;
			if (nObjectX - 24 < nRight && nLeft < nObjectX + 24 &&
					nObjectY - 24 < nBottom && nTop < nObjectY + 24) {
				*pPosition = pObject->m_WorldPosition9C;
				return 1;
			}
			pObject = ((ElementIteratorProc) (*(void***) this)[0x118 / sizeof(void*)])(this);
		}
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0041e640
int CGenericGroup::GetViewData(CViewData* pViewData)
{
	typedef CGameObject*(LEMBALL_FASTCALL * ElementIteratorProc)(CGenericGroup*);
	typedef void(LEMBALL_FASTCALL * ObjectViewDataProc)(CGameObject*, void*, CViewData*);
	int nCount = 0;
	void** pVtable = *(void***) this;
	CGameObject* pObject = ((ElementIteratorProc) pVtable[0x114 / sizeof(void*)])(this);
	while (pObject != 0) {
		((ObjectViewDataProc) ((void**) pObject->m_pVtable00)[3])(pObject, 0, pViewData);
		pViewData = (CViewData*) ((char*) pViewData + 0x4c);
		++nCount;
		pObject = ((ElementIteratorProc) pVtable[0x118 / sizeof(void*)])(this);
	}
	return nCount;
}
