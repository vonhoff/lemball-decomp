#ifndef LEMBALL_VISOS_TARGET_TARGETGRAPHICSSYSTEMSTATE_H
#define LEMBALL_VISOS_TARGET_TARGETGRAPHICSSYSTEMSTATE_H

#include "../../Common.h"

// SIZE 0x0c
struct TargetGraphicsSystemState {
	bool SelectDriver(int p_driverMode);
	bool IsFullscreenDriver()
	{
		return m_driverMode > 1 && m_driverMode < 6;
	}

	int m_driverMode;                    // 0x00
	unsigned int m_reserved04;           // 0x04
	unsigned int m_fallbackWarningShown; // 0x08
};

#endif
