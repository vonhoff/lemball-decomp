#ifndef LEMBALL_VISOS_NETWORK_CFILEBROADCAST_H
#define LEMBALL_VISOS_NETWORK_CFILEBROADCAST_H

#include "CBroadcast.h"
#include "CFileOpenManagement.h"

class CPortsMessage;

// SIZE 0x1b4
// VTABLE: LEMBALL 0x0049a928 CFileCommonSocket
// VTABLE: LEMBALL 0x0049a930 CNetworkFile
// VTABLE: LEMBALL 0x0049a970 CBaseSocket's `CReadSocket
// VTABLE: LEMBALL 0x0049a998 CBaseSocket's `CWriteSocket
// VTABLE: LEMBALL 0x0049a9c8 CFileBroadcast
// VTABLE: LEMBALL 0x0049a9d8 CBroadcast
#pragma warning(disable : 4250)
class CFileBroadcast : public CFileOpenManagement, public CBroadcast {
public:
	CFileBroadcast();
	bool ReadPortInfo();
	bool Start(const char* p_name);
	bool WritePortInfo();
	virtual short FindPort(const unsigned char* p_data); // vtable+0x00
	virtual void GetSpecificAddr(const char* p_name);    // vtable+0x08
	virtual void Process();                              // vtable+0x10
	virtual void ResetPort(short p_port);                // vtable+0x04
	virtual void StopListen();                           // vtable+0x18
	void Closed(int p_notifyPeer);
	void InitialiseFile();
	static void Setup(const char* p_peerName, const char* p_path);
	void StartListen();
	~CFileBroadcast();

private:
	unsigned int m_portInfoLocked;  // 0x54
	unsigned int m_lastProcessTime; // 0x58
};
#pragma warning(default : 4250)

extern CPortsMessage* g_pFileBroadcast;

// SYNTHETIC: LEMBALL 0x0047bb60
// CFileBroadcast::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x0047bc50 SYMBOL
// ??_ECFileBroadcast@@$4PPPPPPPM@KI@AEPAXI@Z

// GLOBAL: LEMBALL 0x0049a860
// CFileBroadcast::`vbtable'{for `CFileRWSocket'}

// GLOBAL: LEMBALL 0x0049a888
// CFileBroadcast::`vbtable'{for `CFileWriteSocket'}

// GLOBAL: LEMBALL 0x0049a8a0
// CFileBroadcast::`vbtable'{for `CFileReadSocket'}

// GLOBAL: LEMBALL 0x0049a8b8
// CFileBroadcast::`vbtable'{for `CFileCommonSocket'}

// GLOBAL: LEMBALL 0x0049a8c8
// CFileBroadcast::`vbtable'{for `CRwSocket'}

// GLOBAL: LEMBALL 0x0049a8d8
// CFileBroadcast::`vbtable'{for `CReadSocket'}

// GLOBAL: LEMBALL 0x0049a8e0
// CFileBroadcast::`vbtable'{for `CWriteSocket'}

// GLOBAL: LEMBALL 0x0049a8e8
// CFileBroadcast::`vbtable'

// GLOBAL: LEMBALL 0x0049a900
// CFileBroadcast::`vbtable'{for `CFileOpenManagement'}

#endif
