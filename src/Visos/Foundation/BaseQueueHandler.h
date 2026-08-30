#ifndef LEMBALL_VISOS_FOUNDATION_BASEQUEUEHANDLER_H
#define LEMBALL_VISOS_FOUNDATION_BASEQUEUEHANDLER_H

#include "../../Common.h"

// SIZE 0x10
// VTABLE: LEMBALL 0x00493110
class BaseQueueHandler {
public:
	BaseQueueHandler();
	virtual VsOStream& StreamOut(VsOStream& p_stream); // vtable+0x00
	virtual ~BaseQueueHandler() {}                     // vtable+0x04
	virtual int ProcessMsg(Message* p_message);        // vtable+0x08

	friend class Game;
	friend class IntroAnimDrawer;
	friend class MainOptions1Drawer;
	friend class MainOptions2Drawer;
	friend class NetworkOptionsDrawer;
	friend class PasswordDrawer;
	friend class PreviewDrawer;
	friend class SuccFailDrawer;
	friend class Translator;
	friend class GunController;
	friend class GunButtons;
	friend class HotAreaList;
	friend class BaseFrontendDrawer;
	friend class TargetAboutScreenQueue;

private:
	unsigned int m_signature;      // 0x04
	unsigned int m_dispatchState;  // 0x08
	unsigned int m_processedCount; // 0x0c
};

extern BaseQueueHandler* g_pNetworkStatusQueue;
extern BaseQueueHandler* g_pNetworkPacketQueue;

// SYNTHETIC: LEMBALL 0x004098e0
// BaseQueueHandler::`scalar deleting destructor'

#endif
