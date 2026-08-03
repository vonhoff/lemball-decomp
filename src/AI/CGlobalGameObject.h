#ifndef LEMBALL_CGLOBALGAMEOBJECT_H
#define LEMBALL_CGLOBALGAMEOBJECT_H

#include "AI/CGameObject.h"

class CGlobalGameObject : public CGameObject {
public:
	int m_fNetworkStateUpdateActive124;
	int m_nStateResetCode128;
	int m_nSerializedState12C;
	int m_fResetPending130;
	int m_nUsableState134;

	int UsableState(void);
	void Restart(void);
	void CancelRequest(void);
	void SendRemove(void);
	void SendCancel(void);
};

#endif
