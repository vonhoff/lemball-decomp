#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"
#include "Visos/Generic/Memory.h"
#include "AI/CDoorManager.h"
#include "AI/AICoord.h"
#include "AI/CGameObject.h"

// TU-local view (matches CHandManager.cpp) of the EFF stream common base.
struct VsNetEffStreamCommon {
	virtual ~VsNetEffStreamCommon(void);
};

// MACINTOSH: CDoor::~CDoor()
// FUNCTION: LEMBALL 0x0040efd0
void LEMBALL_FASTCALL DestroyDoorChunkObject(void* pDoor)
{
	*((void**) pDoor) = (void*) 0x493890;
	*((void**) ((char*) pDoor + 0x138)) = (void*) 0x493870;
	((VsNetEffStreamCommon*) ((char*) pDoor + 0x138))->VsNetEffStreamCommon::~VsNetEffStreamCommon();
	((CGameObject*) pDoor)->DestroyLevelChunkObjectBase();
}

extern unsigned short LEMBALL_FASTCALL GetManagedEntitySlotIdThunk(int nManagedEntityObject);
extern void* g_pLevelTileGrid;
extern int g_nLevelFrameClockTick;
extern int g_nLevelFrameClockTimeMs;

typedef void(LEMBALL_FASTCALL* CDoorNoArgVirtualProc)(void* pEntity);
typedef int(LEMBALL_FASTCALL* CDoorIntReturnVirtualProc)(void* pEntity);

struct ManagedEntityStateView {
	virtual void Reserved00(void) = 0;
	virtual void Reserved01(void) = 0;
	virtual void Reserved02(void) = 0;
	virtual void Reserved03(void) = 0;
	virtual void Reserved04(void) = 0;
	virtual void Reserved05(void) = 0;
	virtual void Reserved06(void) = 0;
	virtual void Reserved07(void) = 0;
	virtual void Reserved08(void) = 0;
	virtual void Reserved09(void) = 0;
	virtual void Reserved10(void) = 0;
	virtual void Reserved11(void) = 0;
	virtual void Reserved12(void) = 0;
	virtual void SetTrigger(int nTrigger) = 0;

	void RequestManagedEntityStateId(int nStateId);
};

struct CDoor : public ManagedEntityStateView {
	unsigned char m_abReserved04[0x5c];
	void* m_pOwnerManager60;             // 0x60
	unsigned char m_abReserved64[0x54];
	int m_nStateB8;
	unsigned char m_abReservedBC[0x10];
	int m_nFrameTickCC;

	void DoActivate(void);
	void SetCollision(void);
	void ResetCollision(void);
	int Process(void);
	int Hits(const AICOORD* pCoord, CGameObject* pGameObject);
	void Set(void* pObjectType, unsigned short nDoorType, int nWorldX, int nWorldY, int nWorldZ);
};

// Split from the original LINKSCF source group to preserve MSVC 4.20 code generation.

// MACINTOSH: CDoor::Unlock()
// FUNCTION: LEMBALL 0x0040dd00
void CDoor::DoActivate(void)
{
	if (m_nStateB8 >= 0x1c && m_nStateB8 <= 0x1d) {
		m_nFrameTickCC = 0x14;
		SetTrigger(0xb);
		RequestManagedEntityStateId(0x20);
	}
}

// MACINTOSH: CDoor::SetCollision()
// FUNCTION: LEMBALL 0x0040d910
void CDoor::SetCollision(void)
{
	char* pObjectBytes = (char*) this;
	int nCellColumns = *(int*) ((char*) g_pLevelTileGrid + 0x10);
	int nCellRows = *(int*) ((char*) g_pLevelTileGrid + 0x14);
	unsigned char* pCellFlags;
	int nTileX;
	int nTileY;

	nTileX = (*(int*) (pObjectBytes + 0x9c) >> 12);
	nTileY = (*(int*) (pObjectBytes + 0xa0) >> 12);
	nTileX = (nTileX + ((nTileX >> 31) & 0xf)) >> 4;
	nTileY = (nTileY + ((nTileY >> 31) & 0xf)) >> 4;
	if (*(int*) (pObjectBytes + 0x64) == 0x19) {
		if (nTileX >= 0) {
			if (nTileY >= 0 && nTileX < nCellColumns && nTileY < nCellRows) {
				pCellFlags = (unsigned char*) (*(int*) ((char*) g_pLevelTileGrid + 0xc) + 6 + (nTileY * nCellColumns + nTileX) * 0xc);
				*pCellFlags = (unsigned char) (*pCellFlags | 1);
			}
			if (nTileX >= 0 && (nTileY = nTileY - 1, nTileY >= 0) && nTileX < nCellColumns && nTileY < nCellRows) {
				pCellFlags = (unsigned char*) (*(int*) ((char*) g_pLevelTileGrid + 0xc) + 6 + (nCellColumns * nTileY + nTileX) * 0xc);
				*pCellFlags = (unsigned char) (*pCellFlags | 1);
			}
		}
	}
	else if (*(int*) (pObjectBytes + 0x64) == 0x1a) {
		if (nTileX >= 0 && nTileY >= 0 && nTileX < nCellColumns && nTileY < nCellRows) {
			pCellFlags = (unsigned char*) (*(int*) ((char*) g_pLevelTileGrid + 0xc) + 6 + (nTileY * nCellColumns + nTileX) * 0xc);
			*pCellFlags = (unsigned char) (*pCellFlags | 1);
		}
		if (nTileX - 1 >= 0 && nTileY >= 0 && nTileX - 1 < nCellColumns && nTileY < nCellRows) {
			pCellFlags = (unsigned char*) (*(int*) ((char*) g_pLevelTileGrid + 0xc) + -6 + (nTileY * nCellColumns + nTileX) * 0xc);
			*pCellFlags = (unsigned char) (*pCellFlags | 1);
		}
	}
}

// MACINTOSH: CDoor::ResetCollision()
// FUNCTION: LEMBALL 0x0040da40
void CDoor::ResetCollision(void)
{
	char* pObjectBytes = (char*) this;
	int nCellColumns = *(int*) ((char*) g_pLevelTileGrid + 0x10);
	int nCellRows = *(int*) ((char*) g_pLevelTileGrid + 0x14);
	unsigned short* pCellFlags;
	int nTileX;
	int nTileY;

	nTileX = (*(int*) (pObjectBytes + 0x9c) >> 12);
	nTileY = (*(int*) (pObjectBytes + 0xa0) >> 12);
	nTileX = (nTileX + ((nTileX >> 31) & 0xf)) >> 4;
	nTileY = (nTileY + ((nTileY >> 31) & 0xf)) >> 4;
	if (*(int*) (pObjectBytes + 0x64) == 0x19) {
		if (nTileX >= 0) {
			if (nTileY >= 0 && nTileX < nCellColumns && nTileY < nCellRows) {
				pCellFlags = (unsigned short*) (*(int*) ((char*) g_pLevelTileGrid + 0xc) + 6 + (nTileY * nCellColumns + nTileX) * 0xc);
				*pCellFlags = (unsigned short) (*pCellFlags & 0xfffe);
			}
			if (nTileX >= 0 && (nTileY = nTileY - 1, nTileY >= 0) && nTileX < nCellColumns && nTileY < nCellRows) {
				pCellFlags = (unsigned short*) (*(int*) ((char*) g_pLevelTileGrid + 0xc) + 6 + (nCellColumns * nTileY + nTileX) * 0xc);
				*pCellFlags = (unsigned short) (*pCellFlags & 0xfffe);
			}
		}
	}
	else if (*(int*) (pObjectBytes + 0x64) == 0x1a) {
		if (nTileX >= 0 && nTileY >= 0 && nTileX < nCellColumns && nTileY < nCellRows) {
			pCellFlags = (unsigned short*) (*(int*) ((char*) g_pLevelTileGrid + 0xc) + 6 + (nTileY * nCellColumns + nTileX) * 0xc);
			*pCellFlags = (unsigned short) (*pCellFlags & 0xfffe);
		}
		if (nTileX - 1 >= 0 && nTileY >= 0 && nTileX - 1 < nCellColumns && nTileY < nCellRows) {
			pCellFlags = (unsigned short*) (*(int*) ((char*) g_pLevelTileGrid + 0xc) + -6 + (nTileY * nCellColumns + nTileX) * 0xc);
			*pCellFlags = (unsigned short) (*pCellFlags & 0xfffe);
		}
	}
}

// MACINTOSH: CDoor::Process()
// FUNCTION: LEMBALL 0x0040db80
int CDoor::Process(void)
{
	char* pObjectBytes = (char*) this;
	void** ppVtable = *(void***) pObjectBytes;
	int nStateId;

	if (*(int*) (pObjectBytes + 0x114) != 0) {
		nStateId = *(int*) (pObjectBytes + 0xb8);
		if (*(int*) (pObjectBytes + 0x128) != nStateId) {
			if (nStateId == 0x20) {
				((CDoorNoArgVirtualProc) ppVtable[0x34 / sizeof(void*)]) (this);
				ResetCollision();
			}
			else if (nStateId == 0x22) {
				SetCollision();
			}
			*(int*) (pObjectBytes + 0x128) = *(int*) (pObjectBytes + 0xb8);
		}
		return 1;
	}
	if (*(int*) (pObjectBytes + 0x144) != 0) {
		if ((unsigned int) *(int*) (pObjectBytes + 0xcc) <= (unsigned int) g_nLevelFrameClockTick) {
			*(int*) (pObjectBytes + 0x94) = g_nLevelFrameClockTimeMs;
			switch (*(int*) (pObjectBytes + 0xb8)) {
			case 0x1c:
				((CDoorNoArgVirtualProc) ppVtable[8 / sizeof(void*)]) (this);
				*(int*) (pObjectBytes + 0x144) = 0;
				return 1;
			case 0x20:
				*(int*) (pObjectBytes + 0x94) = g_nLevelFrameClockTimeMs;
				*(int*) (pObjectBytes + 0xcc) = g_nLevelFrameClockTick + 0x50;
				ResetCollision();
				((CDoorNoArgVirtualProc) ppVtable[8 / sizeof(void*)]) (this);
				return 1;
			case 0x21:
				if (*(short*) (pObjectBytes + 0x140) != 0) {
					*(int*) (pObjectBytes + 0x144) = 0;
					return 1;
				}
				*(int*) (pObjectBytes + 0x94) = g_nLevelFrameClockTimeMs;
				*(int*) (pObjectBytes + 0xcc) = g_nLevelFrameClockTick + 0x14;
				SetCollision();
				((CDoorNoArgVirtualProc) ppVtable[0x34 / sizeof(void*)]) (this);
				((CDoorNoArgVirtualProc) ppVtable[8 / sizeof(void*)]) (this);
				return 1;
			case 0x22:
				*(int*) (pObjectBytes + 0x144) = 0;
				((CDoorNoArgVirtualProc) ppVtable[8 / sizeof(void*)]) (this);
				return 1;
			}
		}
		return 1;
	}
	return 1;
}

// MACINTOSH: CDoor::Hits(const AICOORD&, CGameObject*)
// FUNCTION: LEMBALL 0x0040dd80
int CDoor::Hits(const AICOORD* pCoord, CGameObject* pGameObject)
{
	char* pObjectBytes = (char*) this;
	void** ppVtable = *(void***) pObjectBytes;
	int nTileX;
	int nTileY;

	nTileX = *(int*) (pObjectBytes + 0x9c) >> 12;
	nTileY = *(int*) (pObjectBytes + 0xa0) >> 12;
	if (nTileX - 0x28 <= (pCoord->x >> 12) && (pCoord->x >> 12) <= nTileX + 8 &&
		nTileY - 8 <= (pCoord->y >> 12) && (pCoord->y >> 12) <= nTileY + 8) {
		switch (*(int*) (pObjectBytes + 0xb8)) {
		case 0x1c:
		case 0x1d:
			if (((CDoorIntReturnVirtualProc) (*(void***) pGameObject)[0xb4 / sizeof(void*)]) (this) != 0) {
				*(int*) (pObjectBytes + 0xcc) = 0x14;
				((CDoorNoArgVirtualProc) ppVtable[0x34 / sizeof(void*)]) (this);
				RequestManagedEntityStateId(0x20);
				return 1;
			}
			*(int*) (pObjectBytes + 0xcc) = 0x28;
			RequestManagedEntityStateId(0x1c);
			return 0;
		case 0x1e:
			*(int*) (pObjectBytes + 0xcc) = 0x14;
			((CDoorNoArgVirtualProc) ppVtable[0x34 / sizeof(void*)]) (this);
			RequestManagedEntityStateId(0x20);
			return 1;
		case 0x21:
			return 0;
		}
	}
	return 0;
}

// MACINTOSH: CDoor::Set(eObjectType, unsigned short, int, int, int)
// FUNCTION: LEMBALL 0x0040d4a0
void CDoor::Set(void* pObjectType, unsigned short nDoorType, int nWorldX, int nWorldY, int nWorldZ)
{
	char* pObjectBytes = (char*) this;

	*(int*) (pObjectBytes + 0x64) = (int) pObjectType;
	*(int*) (pObjectBytes + 0x40) = nWorldX << 12;
	*(int*) (pObjectBytes + 0x44) = nWorldY << 12;
	*(unsigned short*) (pObjectBytes + 0x140) = nDoorType;
	*(int*) (pObjectBytes + 0x48) = nWorldZ << 12;
	*(short*) (pObjectBytes + 0x138) = *(short*) 0x49cf48;
	*(short*) 0x49cf48 = (short) (*(short*) 0x49cf48 + 1);
	*(int*) (pObjectBytes + 0xb8) = 0x1e;
	if (*(short*) (pObjectBytes + 0x140) != 0) {
		*(int*) (pObjectBytes + 0xb8) = 0x1d;
	}
	switch (*(short*) (pObjectBytes + 0x140)) {
	case 0: *(unsigned short*) (pObjectBytes + 0xbc) = 0xffff; break;
	case 1: *(unsigned short*) (pObjectBytes + 0xbc) = 0x15; break;
	case 2: *(unsigned short*) (pObjectBytes + 0xbc) = 0x16; break;
	case 3: *(unsigned short*) (pObjectBytes + 0xbc) = 0x17; break;
	case 4: *(unsigned short*) (pObjectBytes + 0xbc) = 0x14; break;
	}
	*(int*) (pObjectBytes + 0x13c) = g_nLevelFrameClockTick;
	*(int*) (pObjectBytes + 0x9c) = *(int*) (pObjectBytes + 0x40);
	*(int*) (pObjectBytes + 0xa0) = *(int*) (pObjectBytes + 0x44);
	*(int*) (pObjectBytes + 0xa4) = *(int*) (pObjectBytes + 0x48);
	*(int*) (pObjectBytes + 0x144) = 0;
}

// MACINTOSH: CDoorManager::Add(short, eObjectType, ushort, int, int, int)
// FUNCTION: LEMBALL 0x0040e0c0
int CDoorManager::Add(short nSlot, void* pObjectType, unsigned short nDoorType, int nWorldX, int nWorldY, int nWorldZ)
{
	CDoor* pObject;

	if (m_nObjectCount34 < m_nCapacity38) {
		if (nSlot == -1) {
			nSlot = (short) ((unsigned int(*)()) 0x40214e)();
		}
		pObject = (CDoor*) ((char*) m_pObjects3C + m_nObjectCount34 * 0x14c);
		((CGameObject*) pObject)->SetId((unsigned short) nSlot);
		pObject->Set(pObjectType, nDoorType, nWorldX, nWorldY, nWorldZ);
		return m_nObjectCount34;
	}
	return -1;
}

// MACINTOSH: CDoorManager::LoadLevel(ushort*, int, uchar)
// FUNCTION: LEMBALL 0x0040e630
void CDoorManager::LoadLevel(unsigned short* pLevelData, int nLen, unsigned char nFormat)
{
	typedef unsigned int(__fastcall* FindFreeSlotProc)(void);
	unsigned int nCount = (unsigned int) *pLevelData;
	int nObjectType;
	int nDoorType;
	int nX;
	int nY;
	int nZ;

	pLevelData = pLevelData + 1;
	((void(__fastcall*)(void*, unsigned int)) 0x401857)(this, nCount);
	while (nCount != 0) {
		unsigned short nSlot2;
		if (*(unsigned short*) ((char*) m_pLevelMode30 + 0x54) <= 1) {
			nSlot2 = (unsigned short) ((FindFreeSlotProc) 0x40214e)();
		}
		else {
			nSlot2 = *pLevelData++;
		}
		nObjectType = *pLevelData++;
		nDoorType = 0;
		if (*(unsigned short*) ((char*) m_pLevelMode30 + 0x54) > 2) {
			nDoorType = *pLevelData++;
		}
		nX = *pLevelData++;
		nY = *pLevelData++;
		nZ = *pLevelData++;
		Add((short) nSlot2, (void*) nObjectType, (unsigned short) nDoorType, nX, nY, nZ);
		nCount = nCount - 1;
	}
}

// MACINTOSH: CDoorManager::Delete(uchar) - array/scalar delete[]
// FUNCTION: LEMBALL 0x0040ec10
void LEMBALL_FASTCALL DeleteDoorChunkObjectArray(void* pObjectArray, int nUnused, unsigned char fDelete)
{
	int nCount;

	if ((fDelete & 2) == 0) {
		((CGameObject*) pObjectArray)->DestroyLevelChunkObjectBase();
		if ((fDelete & 1) != 0) {
			FreeVSMemBlock(pObjectArray);
		}
		return;
	}
	nCount = *(int*) ((char*) pObjectArray - 4);
	while (--nCount >= 0) {
		((CGameObject*) ((char*) pObjectArray + nCount * 0x14c))->DestroyLevelChunkObjectBase();
	}
	FreeVSMemBlock((char*) pObjectArray - 4);
}

// FUNCTION: LEMBALL 0x0040df90
void CDoorManager::Restart(void)
{
	CDoor* pObject;
	int iObject;

	if (m_pObjects3C == 0) {
		return;
	}
	for (iObject = 0; iObject < m_nCapacity38; ++iObject) {
		pObject = (CDoor*) ((char*) m_pObjects3C + iObject * 0x14c);
		((CDoorNoArgVirtualProc) (*(void***) pObject)[0x104 / sizeof(void*)])(pObject);
	}
}

// FUNCTION: LEMBALL 0x0040dfc0
void CDoorManager::Initialise(int nCount)
{
	CDoor* pBase;
	CDoor* pObject;
	int iObject;

	*(unsigned short*) 0x49cf48 = 0;
	m_nObjectCount34 = 0;
	m_nCapacity38 = nCount;
	if (nCount != 0) {
		if (m_pObjects3C == 0) {
			pBase = (CDoor*) ((char*) AllocateVSMemBlock((unsigned int) (nCount * 0x14c + 4)) + 4);
			if (pBase != 0) {
				*(int*) ((char*) pBase - 4) = nCount;
				for (iObject = nCount - 1; iObject >= 0; --iObject) {
					((void(__fastcall*)(void*)) 0x401eba)((char*) pBase + iObject * 0x14c);
				}
				m_pObjects3C = pBase;
			}
			else {
				m_pObjects3C = 0;
			}
		}
		for (iObject = 0; iObject < m_nCapacity38; ++iObject) {
			pObject = (CDoor*) ((char*) m_pObjects3C + iObject * 0x14c);
			((CDoorNoArgVirtualProc) (*(void***) pObject)[0x104 / sizeof(void*)])(pObject);
			pObject->m_pOwnerManager60 = this;
		}
	}
}

// FUNCTION: LEMBALL 0x0040e500
int CDoorManager::Open(void* pCoord, void* pGameObject)
{
	int nOffset;
	int i;
	CDoor* pObject;

	if (m_nObjectCount34 <= 0) {
		return 0;
	}
	for (i = 0, nOffset = 0; i < m_nObjectCount34; ++i, nOffset += 0x14c) {
		pObject = (CDoor*) ((char*) m_pObjects3C + nOffset);
		if (((int(__stdcall*)(void*, void*, void*)) 0x40193d)(pObject, pCoord, pGameObject) != 0) {
			return 1;
		}
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0040e5a0
void CDoorManager::Switch(int nAction, unsigned int nSlot)
{
	int nObjectOffset;
	int iObject;

	iObject = 0;

	if (m_nObjectCount34 > 0) {
		nObjectOffset = 0;
		while (GetManagedEntitySlotIdThunk((int) (unsigned long) ((char*) m_pObjects3C + nObjectOffset)) != nSlot) {
			nObjectOffset += 0x14c;
			++iObject;
			if (m_nObjectCount34 <= iObject) {
				return;
			}
		}
		if (nAction == 3) {
			((CDoor*) ((char*) m_pObjects3C + iObject * 0x14c))->DoActivate();
		}
	}
}
