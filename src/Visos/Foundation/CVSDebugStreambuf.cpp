#include "CVSDebugStreambuf.h"

#include "Visos/Foundation/CVSStreambuf.h"

#include <string.h>

// FUNCTION: LEMBALL 0x0045ad70
CVSDebugStreambuf::CVSDebugStreambuf(char* p_buffer, int p_size, void (*p_flushCallback)(char*))
	: CVSStreambuf(p_buffer, p_size)
{
	m_flushCallback = (void*) p_flushCallback;
	m_buffer = p_buffer;
	m_cursor = p_buffer;
	m_length = 0;
	m_capacity = p_size;
	memset(p_buffer, 0, p_size);
}

// FUNCTION: LEMBALL 0x0045adc0
CVSDebugStreambuf::~CVSDebugStreambuf()
{
}

// FUNCTION: LEMBALL 0x0045add0
void CVSDebugStreambuf::flush()
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
void CVSDebugStreambuf::sputc(char p_c)
{
	int c = p_c;
	switch (c) {
	case '\t':
		*m_cursor = ' ';
		*++m_cursor = '\0';
		if (m_capacity - ++m_length == 1) {
			flush();
		}
		if ((int) m_length % (int) m_tabWidth == 0) {
			return;
		}
		do {
			*m_cursor = ' ';
			*++m_cursor = '\0';
			if (m_capacity - ++m_length == 1) {
				flush();
			}
		} while ((int) m_length % (int) m_tabWidth != 0);
		return;
	case '\n':
		if (m_flushCallback != NULL) {
			*m_cursor = p_c;
			*++m_cursor = '\0';
			if (m_capacity - ++m_length == 1) {
				flush();
			}
			flush();
		}
		else {
			*m_cursor = p_c;
			*++m_cursor = '\0';
			if (m_capacity - ++m_length == 1) {
				flush();
			}
		}
		return;
	default:
		*m_cursor = p_c;
		*++m_cursor = '\0';
		if (m_capacity - ++m_length == 1) {
			flush();
		}
		return;
	}
}

#pragma intrinsic(strlen)

// FUNCTION: LEMBALL 0x0045af20
void CVSDebugStreambuf::sputs(char* p_text)
{
	int len = strlen(p_text);
	while (len > 0) {
		sputc(*p_text++);
		len--;
	}
}

// GLOBAL: LEMBALL 0x004a97b4
CVSDebugStreambuf* g_pDebugStreambuf;

// GLOBAL: LEMBALL 0x004a97b0
CVSDebugStreambuf* g_pSysStreambuf;

// GLOBAL: LEMBALL 0x004a97c0
CVSDebugStreambuf* g_pErrorStreambuf;

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
