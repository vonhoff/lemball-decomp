#include "AI/CLift.h"

// FUNCTION: LEMBALL 0x00425640
int CLift::Activate(void)
{
	typedef void(__stdcall * SetStateProc)(int);
	void* pObject = this;
	*(int*) ((char*) pObject + 0x164) = 1;
	((SetStateProc) (*(void***) pObject)[2])(0x19);
	return 1;
}

// FUNCTION: LEMBALL 0x00425660
void CLift::ActivateDeactivate(void)
{
	typedef void(__stdcall * SetStateProc)(int);
	void* pObject = this;
	if (*(int*) ((char*) pObject + 0x164) == 0) {
		Activate();
	}
	else {
		((SetStateProc) (*(void***) pObject)[2])(8);
	}
}
