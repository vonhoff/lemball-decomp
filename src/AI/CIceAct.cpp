#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

extern unsigned short LEMBALL_FASTCALL GetManagedEntitySlotIdThunk(int nManagedEntityObject);

struct ManagedEntityStateView {
	void RequestManagedEntityStateId(int nStateId);
};

struct CIce : public ManagedEntityStateView {
	void Switch(void);
};

// Split from the original LINKSCF source group to preserve MSVC 4.20 code generation.

// FUNCTION: LEMBALL 0x0042d550
void CIce::Switch(void)
{
	RequestManagedEntityStateId(0x1a);
}

struct CIceManager {
private:
	unsigned char m_abReserved00[0x34];
	int m_nObjectCount34;
	CIce* m_pObjects38;

public:
	void Switch(int nAction, unsigned int nSlot);
};

// FUNCTION: LEMBALL 0x0042dd90
void CIceManager::Switch(int nAction, unsigned int nSlot)
{
	int iObject = 0;
	int nObjectOffset;

	if (m_nObjectCount34 > 0) {
		nObjectOffset = 0;
		while (GetManagedEntitySlotIdThunk((int) (unsigned long) ((char*) m_pObjects38 + nObjectOffset)) !=
			   nSlot) {
			nObjectOffset += 0x188;
			++iObject;
			if (m_nObjectCount34 <= iObject) {
				return;
			}
		}
		if (nAction == 5) {
			((CIce*) ((char*) m_pObjects38 + iObject * 0x188))->Switch();
		}
	}
}
