#include "ENGINE/CORE/VSINIT.H"

extern unsigned short LEMBALL_FASTCALL GetManagedEntitySlotIdThunk(int nManagedEntityObject);

struct VsNetEffStreamBase {
	void WriteEffStreamU16BE(unsigned short nValue);
	void** m_pVtable00;
	int m_nManagedEntityType04;
	char m_abReserved08[0x24];
	void* m_pManagedEntity2C;
};

// Split from LEVELRUN.CPP to preserve compiler state in the original translation unit.

// FUNCTION: LEMBALL 0x004168a0
void LEMBALL_FASTCALL SerializeManagedEntityTypeAndSlotId(void* pObject)
{
	VsNetEffStreamBase* pStream;

	pStream = (VsNetEffStreamBase*) pObject;
	pStream->WriteEffStreamU16BE(pStream->m_nManagedEntityType04);
	pStream->WriteEffStreamU16BE(GetManagedEntitySlotIdThunk((int) (unsigned long) pStream->m_pManagedEntity2C));
}
