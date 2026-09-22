#ifndef LEMBALL_VISOS_FOUNDATION_CBASEQUEUEHANDLER_H
#define LEMBALL_VISOS_FOUNDATION_CBASEQUEUEHANDLER_H

#include "Message.h"

class CBaseQueue;
class CVSOStream;

// SIZE 0x10
// VTABLE: LEMBALL 0x00493110
class CBaseQueueHandler {
public:
	CBaseQueueHandler();
	virtual CVSOStream& StreamOut(CVSOStream& p_stream); // vtable+0x00
	virtual ~CBaseQueueHandler() {}                      // vtable+0x04
	virtual int ProcessMsg(Message* p_message);          // vtable+0x08

	friend class CGame;
	friend class CIntroAnimDrawer;
	friend class CMainOptions1Drawer;
	friend class CMainOptions2Drawer;
	friend class CNetworkOptionsDrawer;
	friend class CPasswordDrawer;
	friend class CPreviewDrawer;
	friend class CSuccFailDrawer;
	friend class CTranslator;
	friend class CGunController;
	friend class CGunButtons;
	friend class CHotAreaList;
	friend class CBaseFrontendDrawer;
	friend class CAboutScreen;
	friend class CAI;
	friend class CAICursor;

private:
	unsigned int m_signature;     // 0x04
	unsigned int m_dispatchState; // 0x08

protected:
	unsigned int m_processedCount; // 0x0c
};

extern CBaseQueue* g_pNetworkStatusQueue;
extern CBaseQueue* g_pNetworkPacketQueue;

// SYNTHETIC: LEMBALL 0x004098e0
// CBaseQueueHandler::`scalar deleting destructor'

#endif
