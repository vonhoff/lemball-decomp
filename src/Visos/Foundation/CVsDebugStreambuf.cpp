#include "CVsDebugStreambuf.h"

#include "Visos/Foundation/CVsStreambuf.h"

#include <string.h>

// FUNCTION: LEMBALL 0x0045ad70
CVsDebugStreambuf::CVsDebugStreambuf(char* p_buffer, int p_size, void (*p_flushCallback)(char*))
	: CVsStreambuf(p_buffer, p_size)
{
	m_flushCallback = (void*) p_flushCallback;
	m_buffer = p_buffer;
	m_cursor = p_buffer;
	m_length = 0;
	m_capacity = p_size;
	memset(p_buffer, 0, p_size);
}

// FUNCTION: LEMBALL 0x0045adc0
CVsDebugStreambuf::~CVsDebugStreambuf()
{
}

// FUNCTION: LEMBALL 0x0045add0
void CVsDebugStreambuf::Flush()
{
	if (m_flushCallback != NULL) {
		((void (*)(char*)) m_flushCallback)(m_buffer);
	}
	m_cursor = m_buffer;
	m_length = 0;
	m_tabWidth = 8;
	memset(m_buffer, 0, m_capacity);
}

// FUNCTION: LEMBALL 0x0045ae10
void CVsDebugStreambuf::Sputc(char p_c)
{
	int c = p_c;
	switch (c) {
	case '\t':
		*m_cursor = ' ';
		*++m_cursor = '\0';
		if (m_capacity - ++m_length == 1) {
			Flush();
		}
		if ((int) m_length % (int) m_tabWidth == 0) {
			return;
		}
		do {
			*m_cursor = ' ';
			*++m_cursor = '\0';
			if (m_capacity - ++m_length == 1) {
				Flush();
			}
		} while ((int) m_length % (int) m_tabWidth != 0);
		return;
	case '\n':
		if (m_flushCallback != NULL) {
			*m_cursor = p_c;
			*++m_cursor = '\0';
			if (m_capacity - ++m_length == 1) {
				Flush();
			}
			Flush();
		}
		else {
			*m_cursor = p_c;
			*++m_cursor = '\0';
			if (m_capacity - ++m_length == 1) {
				Flush();
			}
		}
		return;
	default:
		*m_cursor = p_c;
		*++m_cursor = '\0';
		if (m_capacity - ++m_length == 1) {
			Flush();
		}
		return;
	}
}

#pragma intrinsic(strlen)

// FUNCTION: LEMBALL 0x0045af20
void CVsDebugStreambuf::Sputs(char* p_text)
{
	int len = strlen(p_text);
	while (len > 0) {
		Sputc(*p_text++);
		len--;
	}
}

// GLOBAL: LEMBALL 0x004a97b4
CVsDebugStreambuf* g_pDebugStreambuf;

// GLOBAL: LEMBALL 0x004a97b0
CVsDebugStreambuf* g_pSysStreambuf;

// GLOBAL: LEMBALL 0x004a97c0
CVsDebugStreambuf* g_pErrorStreambuf;

// GLOBAL: LEMBALL 0x004a071c
void* g_pDebugAcceleratorTable = 0;

// GLOBAL: LEMBALL 0x004a0e68
int g_nDebugInitialized = 0;

// GLOBAL: LEMBALL 0x004a0e80
int g_nDebugFileOutputEnabled = 0;

// GLOBAL: LEMBALL 0x004a0e84
int g_nAsyncDebugEnabled = 0;

// GLOBAL: LEMBALL 0x004a29f4
void* g_pDebugThread = 0;

// GLOBAL: LEMBALL 0x004a2a04
void* g_pDebugSyncEvent = 0;

// GLOBAL: LEMBALL 0x004a2a08
int g_nDebugThreadId = 0x12345678;

// GLOBAL: LEMBALL 0x004a2a0c
FILE* g_pDebugOutputFile = 0;

// GLOBAL: LEMBALL 0x004a2a10
char* g_pDebugOutputPath = "debug.out";
