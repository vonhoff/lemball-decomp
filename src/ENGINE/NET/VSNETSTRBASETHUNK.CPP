#include "ENGINE/CORE/VSINIT.H"

struct VsNetEffStreamCommon {
	virtual ~VsNetEffStreamCommon();
};

// Split from VSNETSTR.CPP to preserve compiler state in the original translation unit.

// FUNCTION: LEMBALL 0x00452fd0
void LEMBALL_FASTCALL DestroyEffStreamBase52FD0(void* pObject)
{
	((VsNetEffStreamCommon*) pObject)->VsNetEffStreamCommon::~VsNetEffStreamCommon();
}
