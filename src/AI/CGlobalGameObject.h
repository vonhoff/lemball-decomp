#ifndef LEMBALL_CGLOBALGAMEOBJECT_H
#define LEMBALL_CGLOBALGAMEOBJECT_H

#include "AI/CGameObject.h"

struct CNetworkMessage;

class CGlobalGameObject : public CGameObject {
public:
	int m_fNetworkStateUpdateActive124;
	int m_nStateResetCode128;
	int m_nSerializedState12C;
	int m_fResetPending130;
	int m_nUsableState134;
	unsigned char m_abReserved138[0x15c - 0x138];
	unsigned short m_cActionCount15C;
	unsigned char m_abReserved15E[0x160 - 0x15e];

	struct GModeAction {
		int m_nType;
		unsigned short m_nParam;
	};
	GModeAction m_aActions160[0x20];

	int UsableState(void);
	void Restart(void);
	void CancelRequest(void);
	void SendRemove(void);
	void SendCancel(void);
#ifdef LEMBALL_CGLOBALGAMEOBJECT_SETMESSAGES
	void SetMessages(void);
#endif
	int Receive(unsigned short nMessage, CNetworkMessage* pMessage);
	void AppendGmobChunkType14Action(int nType, unsigned short nParam);
};

#endif
