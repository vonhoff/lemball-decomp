#ifndef LEMBALL_VISOS_FOUNDATION_VSDEBUGSTREAMBUF_H
#define LEMBALL_VISOS_FOUNDATION_VSDEBUGSTREAMBUF_H

#include "../../Common.h"
#include "VsStreambuf.h" // complete type

// SIZE 0x1c
// VTABLE: LEMBALL 0x00498968
class VsDebugStreambuf : public VsStreambuf {
public:
	VsDebugStreambuf(char* p_buffer, int p_size, void (*p_flushCallback)(char*));
	virtual ~VsDebugStreambuf();      // vtable+0x00
	virtual void Flush();             // vtable+0x04
	virtual void Sputc(char p_arg0);  // vtable+0x08
	virtual void Sputs(char* p_arg0); // vtable+0x0c

	void* m_flushCallback; // 0x18
};

extern VsDebugStreambuf* g_pDebugStreambuf;
extern VsDebugStreambuf* g_pSysStreambuf;
extern VsDebugStreambuf* g_pErrorStreambuf;
extern void* g_pDebugAcceleratorTable;
extern int g_nDebugInitialized;
extern int g_nDebugFileOutputEnabled;
extern int g_nAsyncDebugEnabled;
extern void* g_pDebugThread;
extern void* g_pDebugSyncEvent;
extern int g_nDebugThreadId;
extern FILE* g_pDebugOutputFile;
extern char* g_pDebugOutputPath;

// SYNTHETIC: LEMBALL 0x0045af60
// VsDebugStreambuf::`scalar deleting destructor'

#endif

