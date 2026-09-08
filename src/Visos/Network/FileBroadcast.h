#ifndef LEMBALL_VISOS_NETWORK_FILEBROADCAST_H
#define LEMBALL_VISOS_NETWORK_FILEBROADCAST_H

#include "../../Common.h"
#include "Broadcast.h"          // complete type
#include "FileOpenManagement.h" // complete type

// SIZE 0x1b4
// VTABLE: LEMBALL 0x0049a928 native callback
// VTABLE: LEMBALL 0x0049a930 NetworkFile view
// VTABLE: LEMBALL 0x0049a970 ReadSocket view
// VTABLE: LEMBALL 0x0049a998 WriteSocket view
// VTABLE: LEMBALL 0x0049a9c8 BaseCommonSocket view
// VTABLE: LEMBALL 0x0049a9d8 primary Broadcast view
class FileBroadcast : public FileOpenManagement, public Broadcast {
public:
	FileBroadcast();
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
	~FileBroadcast();

private:
	unsigned int m_portInfoLocked;  // 0x54
	unsigned int m_lastProcessTime; // 0x58
};

extern PortsMessage* g_pFileBroadcast;

// SYNTHETIC: LEMBALL 0x0047bb60
// FileBroadcast::`scalar deleting destructor'

#endif
