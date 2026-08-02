#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

extern unsigned short LEMBALL_FASTCALL GetManagedEntitySlotIdThunk(int nManagedEntityObject);

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
	unsigned char m_abReserved04[0xb4];
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
	unsigned char m_abReserved38[4];
	CDoor* m_pObjects3C;

public:
	void Switch(int nAction, unsigned int nSlot);
};

// FUNCTION: LEMBALL 0x0040e5a0
void CDoorManager::Switch(int nAction, unsigned int nSlot)
{
	int nObjectOffset;
	int iObject;

	iObject = 0;

	if (m_nObjectCount34 > 0) {
		nObjectOffset = 0;
		while (GetManagedEntitySlotIdThunk((int) (unsigned long) ((char*) m_pObjects3C + nObjectOffset)) !=
			   nSlot) {
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
