#ifndef LEMBALL_VISOS_TARGET_TARGETNETWORKWINDOW_H
#define LEMBALL_VISOS_TARGET_TARGETNETWORKWINDOW_H

#include "../../Common.h"

// SIZE 0x10
// VTABLE: LEMBALL 0x0049ac30
class TargetNetworkWindow {
public:
	TargetNetworkWindow(const char* p_className, int* p_registered);
	// FUNCTION: LEMBALL 0x004714f0
	virtual int Process(unsigned int p_message, unsigned int p_wParam, long p_lParam) // vtable+0x00
	{
		return -1;
	}
	~TargetNetworkWindow();

public:
	void* m_windowHandle;        // 0x04
	unsigned int m_firstMessage; // 0x08
	unsigned int m_lastMessage;  // 0x0c
};

#endif
