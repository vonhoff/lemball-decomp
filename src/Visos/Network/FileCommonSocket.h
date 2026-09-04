#ifndef LEMBALL_VISOS_NETWORK_FILECOMMONSOCKET_H
#define LEMBALL_VISOS_NETWORK_FILECOMMONSOCKET_H

#include "../../Common.h"
#include "BaseCommonSocket.h" // complete type
#include "NetworkFile.h"      // complete type

// SIZE 0x4c
// FileReadSocket/FileWriteSocket/FileRwSocket place the next virtual base four
// bytes after this class's vbptr, while their generated FileCommonSocket
// adjustors retain the original 0x20 BaseCommonSocket displacement. Together,
// those x86 constraints require one unknown word and no hidden vtordisp here.
#pragma vtordisp(off)
class FileCommonSocket : public virtual NetworkFile, public virtual BaseCommonSocket {
public:
	virtual void InitialiseFile() = 0;
	bool CreateSocket(const char* p_path);
	virtual int SysCloseSocket();
	virtual void SocketError();
	virtual ~FileCommonSocket();

private:
	undefined4 m_unk0x08; // 0x08
};
#pragma vtordisp(on)

// SYNTHETIC: LEMBALL 0x0047b910
// FileCommonSocket::`scalar deleting destructor'

#endif
