#include "AI/CAICursor.h"

// FUNCTION: LEMBALL 0x00414e80
int CAICursor::ProcessMsg(tagMESSAGE* pMsg)
{
	short* raw = (short*) pMsg;
	if (*raw != 1) {
		m_nUnhandledCount0C++;
		return 0;
	}
	((void(__fastcall*)(void*, int, int)) 0x40281f)(this, *(int*) ((char*) raw + 8), *(int*) ((char*) raw + 0xc));
	return 1;
}
