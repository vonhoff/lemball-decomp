#ifndef LEMBALL_VISOS_FOUNDATION_CVSDEBUGSTREAMBUF_H
#define LEMBALL_VISOS_FOUNDATION_CVSDEBUGSTREAMBUF_H

#include "CVSStreambuf.h" // complete type

struct FILE;

// SIZE 0x1c
// VTABLE: LEMBALL 0x00498968
class CVSDebugStreambuf : public CVSStreambuf {
public:
	CVSDebugStreambuf(char* p_buffer, int p_size, void (*p_flushCallback)(char*));
	virtual ~CVSDebugStreambuf();     // vtable+0x00
	virtual void flush();             // vtable+0x04
	virtual void sputc(char p_arg0);  // vtable+0x08
	virtual void sputs(char* p_arg0); // vtable+0x0c

	void* m_flushCallback; // 0x18
};

extern CVSDebugStreambuf* g_pDebugStreambuf;
extern CVSDebugStreambuf* g_pSysStreambuf;
extern CVSDebugStreambuf* g_pErrorStreambuf;
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
// CVSDebugStreambuf::`scalar deleting destructor'

#endif
