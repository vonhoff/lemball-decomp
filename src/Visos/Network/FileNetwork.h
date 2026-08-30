#ifndef LEMBALL_VISOS_NETWORK_FILENETWORK_H
#define LEMBALL_VISOS_NETWORK_FILENETWORK_H

#include "../../Common.h"
#include "BaseNetwork.h" // complete type

// SIZE 0x7c
// VTABLE: LEMBALL 0x0049a02c native window shell
// VTABLE: LEMBALL 0x00499ff8 BaseNetwork subobject at +0x10
class FileNetwork {
public:
	FileNetwork();
	int Process();
	virtual FileBroadcast* GetNewBroadcast();           // vtable+0x28
	virtual FileConnect* GetNewConnect();               // vtable+0x24
	virtual FileNetworkAddress* GetNewNetworkAddress(); // vtable+0x2c
	virtual void AfterDestroyConnections();             // vtable+0x1c
	virtual void BeforeDestroyConnections();            // vtable+0x18
	virtual void ForceProcess();                        // vtable+0x20
	virtual void Initialise();                          // vtable+0x0c
	virtual void UnInitialise();                        // vtable+0x10
	virtual ~FileNetwork();                             // vtable+0x04
	void ResetTimer(unsigned int p_interval);
	void Setup(const char* p_peerName, const char* p_path);

private:
	void* m_windowHandle;          // 0x04
	undefined m_windowPad[8];      // 0x08
	BaseNetwork m_base;            // 0x10
	unsigned int m_timerId;        // 0x74
	unsigned int m_alternateTimer; // 0x78
};

extern unsigned int g_dwFileNetworkThreadId;
extern void* g_hFileNetworkThread;
extern void* g_pFileBroadcastData;

// SYNTHETIC: LEMBALL 0x0046f970
// FileNetwork::`scalar deleting destructor'

#endif
