#ifndef LEMBALL_NETWORK_GAME_NETWORKMANAGER_H
#define LEMBALL_NETWORK_GAME_NETWORKMANAGER_H

#include "../../Visos/Foundation/BaseQueueHandler.h" // complete type
#include "../../Visos/Network/Connect.h"

class Ai;
class BaseFrontendDrawer;
class Broadcast;
class GameRejectMessage;
class NetworkGameMessage;
class NetworkGameStage;
class NetworkOptionsDrawer;
class NetworkOptionsProc;
struct Message;
// SIZE 0x6c
// VTABLE: LEMBALL 0x004985f8
class NetworkManager : public BaseQueueHandler {
public:
	NetworkGameMessage* GetGameMessage(CConnect* p_connection);
	NetworkManager(const char* p_arg0);
	bool Start();
	int GetnGame(CConnect* p_connection);
	int CountActiveGames();
	virtual int ProcessMsg(Message* p_message); // vtable+0x08
	virtual void Process();                     // vtable+0x0c
	virtual ~NetworkManager();                  // vtable+0x04
	void Broadcast(const char* p_address);
	void GameProcess();
	void Kill();
	void StartBroadcast(const char* p_address);
	void Stop();

	friend class NetworkOptionsDrawer;
	friend class NetworkOptionsProc;
	friend class BaseFrontendDrawer;
	friend class Ai;

private:
	NetworkGameMessage* m_gameMessage;    // 0x10
	NetworkGameMessage* m_gameMessages;   // 0x14
	NetworkGameStage* m_gameStage;        // 0x18
	GameRejectMessage* m_rejectMessage;   // 0x1c
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

extern NetworkManager* g_pNetworkManager;

// SYNTHETIC: LEMBALL 0x00452f20
// NetworkManager::`scalar deleting destructor'

#endif
