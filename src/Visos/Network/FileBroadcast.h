#ifndef LEMBALL_VISOS_NETWORK_FILEBROADCAST_H
#define LEMBALL_VISOS_NETWORK_FILEBROADCAST_H

#include "../../Common.h"

// SIZE 0x1b4
// VTABLE: LEMBALL 0x0049a928 native callback
// VTABLE: LEMBALL 0x0049a930 NetworkFile view
// VTABLE: LEMBALL 0x0049a970 ReadSocket view
// VTABLE: LEMBALL 0x0049a998 WriteSocket view
// VTABLE: LEMBALL 0x0049a9c8 BaseCommonSocket view
// VTABLE: LEMBALL 0x0049a9d8 primary Broadcast view
class FileBroadcast {
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
	void Closed(unsigned char p_notifyPeer);
	void InitialiseFile();
	void Setup(const char* p_peerName, const char* p_path);
	void StartListen();
	~FileBroadcast();

private:
	undefined m_unk04[0x10];    // 0x04
	unsigned int m_listen;      // 0x14
	undefined m_storage[0x19c]; // 0x18
};

extern FileBroadcast* g_pFileBroadcast;

// SYNTHETIC: LEMBALL 0x0047bb60
// FileBroadcast::`scalar deleting destructor'

#endif
