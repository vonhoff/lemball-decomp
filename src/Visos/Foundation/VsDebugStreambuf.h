#ifndef LEMBALL_VISOS_FOUNDATION_VSDEBUGSTREAMBUF_H
#define LEMBALL_VISOS_FOUNDATION_VSDEBUGSTREAMBUF_H

#include "../../Common.h"
#include "VsStreambuf.h" // complete type

// SIZE 0x1c
// VTABLE: LEMBALL 0x00498968
class VsDebugStreambuf : public VsStreambuf {
public:
	VsDebugStreambuf(char* p_arg0,
					 int p_arg1,
					 undefined4* p_arg2,
					 char* p_arg3,
					 undefined4 p_arg4,
					 unsigned char p_arg5);
	virtual int Sputc(char p_arg0);   // vtable+0x08
	virtual void Flush();             // vtable+0x04
	virtual void Sputs(char* p_arg0); // vtable+0x0c
	virtual ~VsDebugStreambuf();      // vtable+0x00

private:
	unsigned int m_length;   // 0x10
	unsigned int m_tabWidth; // 0x14
	void* m_flushCallback;   // 0x18
};

// Confirmed class-scoped globals.
extern VsDebugStreambuf* g_pDebugStreambuf;
extern VsDebugStreambuf* g_pSysStreambuf;
extern VsDebugStreambuf* g_pErrorStreambuf;
#endif
