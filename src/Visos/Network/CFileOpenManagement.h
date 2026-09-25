#ifndef LEMBALL_VISOS_NETWORK_CFILEOPENMANAGEMENT_H
#define LEMBALL_VISOS_NETWORK_CFILEOPENMANAGEMENT_H

#include "CFileRWSocket.h"
#include "COpenCount.h"

// SIZE 0x34
// VTABLE: LEMBALL 0x0049a9f4 CFileCommonSocket
// VTABLE: LEMBALL 0x0049a9f8 CNetworkFile
// VTABLE: LEMBALL 0x0049aa38 CBaseSocket's `CReadSocket
// VTABLE: LEMBALL 0x0049aa60 CBaseSocket's `CWriteSocket
// VTABLE: LEMBALL 0x0049aa90 CFileOpenManagement
#pragma warning(disable : 4250)
class CFileOpenManagement : public virtual CFileRWSocket {
public:
	bool DecOpenCount();
	bool IncOpenCount();
	int SysCloseSocket();
	friend class CFileConnect;

private:
	friend class CFileBroadcast;
	COpenCount m_message; // 0x04
};
#pragma warning(default : 4250)

// SYNTHETIC: LEMBALL 0x0047b9d0
// CFileOpenManagement::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x0047bb10 SYMBOL
// ??_ECFileOpenManagement@@$4PPPPPPPM@KI@AEPAXI@Z

#endif
