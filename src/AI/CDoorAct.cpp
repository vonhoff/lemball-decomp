#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"
#include "Visos/Generic/Memory.h"

extern unsigned short LEMBALL_FASTCALL GetManagedEntitySlotIdThunk(int nManagedEntityObject);

typedef void(LEMBALL_FASTCALL* CDoorNoArgVirtualProc)(void* pEntity);

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

struct CDoorManager {
private:
	unsigned char m_abReserved00[0x34];
	int m_nObjectCount34;
	int m_nCapacity38;
	CDoor* m_pObjects3C;

public:
	void Switch(int nAction, unsigned int nSlot);
	void Restart(void);
	int Open(void* pCoord, void* pGameObject);
	void Initialise(int nCount);
};

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
