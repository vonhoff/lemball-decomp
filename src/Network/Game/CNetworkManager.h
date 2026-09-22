#ifndef LEMBALL_NETWORK_GAME_CNETWORKMANAGER_H
#define LEMBALL_NETWORK_GAME_CNETWORKMANAGER_H

#include "../../Visos/Foundation/CBaseQueueHandler.h" // complete type
class CConnect;

class CGameRejectMessage;
class CNetworkGameMessage;
class CNetworkGameStage;
// SIZE 0x6c
// VTABLE: LEMBALL 0x004985f8
class CNetworkManager : public CBaseQueueHandler {
public:
	CNetworkGameMessage* GetGameMessage(CConnect* p_connection);
	CNetworkManager(const char* p_filePeerName);
	bool Start();
	int GetnGame(CConnect* p_connection);
	int CountActiveGames();
	virtual int ProcessMsg(Message* p_message); // vtable+0x08
	virtual void Process();                     // vtable+0x0c
	virtual ~CNetworkManager();                 // vtable+0x04
	void Broadcast(const char* p_address);
	void GameProcess();
	void Kill();
	void StartBroadcast(const char* p_address);
	void Stop();

	friend class CNetworkOptionsDrawer;
	friend class CNetworkOptionsProc;
	friend class CBaseFrontendDrawer;
	friend class CAI;

private:
	CNetworkGameMessage* m_gameMessage;   // 0x10
	CNetworkGameMessage* m_gameMessages;  // 0x14
	CNetworkGameStage* m_gameStage;       // 0x18
	CGameRejectMessage* m_rejectMessage;  // 0x1c
	CConnect* m_connections[10];          // 0x20
	unsigned int m_desiredGameState;      // 0x48
	unsigned int m_observedGameState;     // 0x4c
	unsigned int m_lastGameStateSendTime; // 0x50
	unsigned int m_connectionsChanged;    // 0x54
	unsigned int m_broadcastStartTime;    // 0x58
	unsigned int m_networkInitialised;    // 0x5c
	unsigned int m_externalDriverLoaded;  // 0x60
	unsigned int m_localDriverLoaded;     // 0x64
	unsigned int m_killRequested;         // 0x68
};

extern CNetworkManager* g_pNetworkManager;

// SYNTHETIC: LEMBALL 0x00452f20
// CNetworkManager::`scalar deleting destructor'

#endif
