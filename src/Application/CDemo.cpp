#include "CDemo.h"

struct _Filet;

#include "Engine/Queues/CBaseQueue.h"
#include "Engine/Queues/Message.h"
#include "Engine/Queues/PackParam.h"
#include "Engine/Resources/Types/CResBIN.h"
#include "Engine/Time/VsTime.h"
#include "Platform/Windows/Windowing/CPVWnd.h"

#include <stddef.h>

enum {
	DEMO_PACKET_INDEX_BYTE_MASK = 0xff,
	DEMO_MESSAGE_RECORDED_FLAG = 0x8000,
	DEMO_MESSAGE_TYPE_MASK = 0x7fff,
	DEMO_BUFFER_NEEDS_LOAD = -1
};

// FUNCTION: LEMBALL 0x00409150
void _DEMO_Init(int p_sourceId)
{
	g_pDemo = new CDemo(p_sourceId);
}

// FUNCTION: LEMBALL 0x00409180
void _DEMO_Quit()
{
	if (g_pDemo != NULL) {
		delete g_pDemo;
		g_pDemo = NULL;
	}
}

// FUNCTION: LEMBALL 0x004091b0
CDemo::CDemo(int p_sourceId)
{
	m_offsetY = 0;
	m_offsetX = 0;
	m_sourceId = p_sourceId;
	m_buffer = NULL;
	m_window = NULL;
	m_currentResourceId = 0;
	m_firstResourceId = 0;
	m_resourceCount = 0;
	m_resource = NULL;
	m_filePath = NULL;
	m_demoMode = 0;
	m_state48 = 0;
	m_gameOver = 0;
	m_bytesRemaining = DEMO_BUFFER_NEEDS_LOAD;
	m_state54 = 0;
	g_pMasterInputQueue->Attach(this, -100);
	Reset();
}

// FUNCTION: LEMBALL 0x00409220
CDemo::~CDemo()
{
	CleanUp();
	g_pMasterInputQueue->Detach(this, -100);
}

// FUNCTION: LEMBALL 0x00409250
bool CDemo::SendNextPacket(int p_packetIndex)
{
	if (m_bytesRemaining == DEMO_BUFFER_NEEDS_LOAD && !LoadBuffer()) {
		return false;
	}
	if (*m_readCursor != (p_packetIndex & DEMO_PACKET_INDEX_BYTE_MASK)) {
		return false;
	}
	Message message;
	message.m_time = CurrentQueueTimer();
	m_readCursor++;
	message.m_type = m_readCursor[0];
	message.m_type |= (unsigned short) m_readCursor[1] << 8;
	m_readCursor += 2;
	message.m_code = m_readCursor[0];
	message.m_code |= (unsigned int) m_readCursor[1] << 8;
	message.m_code |= (unsigned int) m_readCursor[2] << 16;
	message.m_code |= (unsigned int) m_readCursor[3] << 24;
	m_readCursor += 4;
	unsigned int payload = m_readCursor[0];
	payload |= (unsigned int) m_readCursor[1] << 8;
	payload |= (unsigned int) m_readCursor[2] << 16;
	payload |= (unsigned int) m_readCursor[3] << 24;
	message.m_payload = (void*) payload;
	m_readCursor += 4;
	unsigned int source = m_readCursor[0];
	source |= (unsigned int) m_readCursor[1] << 8;
	source |= (unsigned int) m_readCursor[2] << 16;
	source |= (unsigned int) m_readCursor[3] << 24;
	message.m_source = (void*) source;
	m_readCursor += 4;
	if (m_window == NULL) {
		return false;
	}
	switch ((unsigned int) message.m_type) {
	case MESSAGE_MOUSE_BUTTON_UP:
	case MESSAGE_MOUSE_BUTTON_DOWN:
	case MESSAGE_CURSOR_BUTTON_DOWN:
	case MESSAGE_CURSOR_BUTTON_UP: {
		int zoom;
		CVSPoint position((short) message.m_code,
						  (short) ((unsigned int) message.m_code >> PACK_PARAM_HIGH_WORD_SHIFT));
		CVSPoint& point = position;
		zoom = m_window->m_zoom;
		point.m_x = (short) (zoom * point.m_x);
		point.m_y = (short) (zoom * point.m_y);
		point.m_x += m_window->m_rect.m_x;
		point.m_y += m_window->m_rect.m_y;
		position.m_x += m_offsetX;
		position.m_y += m_offsetY;
		const CPVWnd* window = m_window;
		if (point.m_x < window->m_rect.m_x || (short) (window->m_rect.m_width + window->m_rect.m_x) <= point.m_x ||
			point.m_y < window->m_rect.m_y || (short) (window->m_rect.m_height + window->m_rect.m_y) <= point.m_y) {
			return false;
		}
		message.m_code = PackParam(point.m_x, point.m_y);
		break;
	}
	}
	message.m_type |= DEMO_MESSAGE_RECORDED_FLAG;
	g_pMasterInputQueue->Post(message);
	return true;
}

#include "Engine/Files/VsFile.h"

// FUNCTION: LEMBALL 0x00409460
bool CDemo::LoadBuffer()
{
	if (m_filePath != NULL) {
		_Filet* file = vsOpen(m_filePath, "rb");
		if (file == NULL) {
			return false;
		}
		unsigned long size = vsGetFileSize(file);
		m_buffer = new unsigned char[size];
		unsigned long bytesRead = vsRead(file, m_buffer, size);
		vsClose(file);
		m_bytesRemaining = bytesRead;
	}
	else {
		m_resource = CResBIN::Load(m_currentResourceId);
		CResBIN* resource = m_resource;
		if (resource->m_loaded != 0) {
			resource->m_age = 0;
		}
		else {
			resource->LoadData();
		}
		++resource->m_directUseCount;
		m_buffer = m_resource->GetData();
		m_bytesRemaining = m_resource->m_dataSize;
		++m_currentResourceId;
		if (m_firstResourceId + m_resourceCount <= m_currentResourceId) {
			m_currentResourceId = m_firstResourceId;
		}
	}

	unsigned char* cursor = m_buffer;
	m_readCursor = cursor;
	m_bytesRemaining = cursor[0];
	m_bytesRemaining |= (unsigned int) cursor[1] << 8;
	m_bytesRemaining |= (unsigned int) cursor[2] << 16;
	m_bytesRemaining |= (unsigned int) cursor[3] << 24;
	m_readCursor = cursor + 4;
	return true;
}

#include "Engine/Math/CVSPoint.h"
#include "Engine/Math/CVSRect.h"

#include <string.h>

// FUNCTION: LEMBALL 0x00409560
void CDemo::GetUserPacket(unsigned char* p_data, unsigned long& p_size)
{
	if (m_bytesRemaining == DEMO_BUFFER_NEEDS_LOAD) {
		LoadBuffer();
	}
	p_size = m_readCursor[0];
	p_size |= (unsigned long) m_readCursor[1] << 8;
	p_size |= (unsigned long) m_readCursor[2] << 16;
	p_size |= (unsigned long) m_readCursor[3] << 24;
	m_readCursor += 4;
	m_bytesRemaining -= p_size + 4;
	memcpy(p_data, m_readCursor, p_size);
	m_readCursor += p_size;
}

// FUNCTION: LEMBALL 0x004095e0
void CDemo::Reset()
{
	m_bytesRemaining = DEMO_BUFFER_NEEDS_LOAD;
	m_packetIndex = 0;
	m_gameOver = 0;
	m_duration = 0;
}

// FUNCTION: LEMBALL 0x00409600
void CDemo::SetDemoMode(int p_enabled)
{
	m_demoMode = p_enabled;
	if (p_enabled != 0) {
		Reset();
	}
	else {
		CleanUp();
	}
}

// FUNCTION: LEMBALL 0x00409620
void CDemo::Process()
{
	if (m_gameOver == 0 && m_demoMode != 0) {
		unsigned long elapsed = CurrentMilliTimer() - m_startTime;
		if (m_duration != 0 && m_duration <= elapsed) {
			GameIsOver();
			return;
		}
		m_packetIndex++;
		bool more;
		do {
			more = SendNextPacket(m_packetIndex);
		} while (more);
	}
}

// FUNCTION: LEMBALL 0x00409660
void CDemo::CleanUp()
{
	if (m_resource != NULL) {
		m_resource->m_directUseCount--;
		m_resource->UnLoad();
		m_resource = NULL;
	}
	else if (m_buffer != NULL) {
		operator delete(m_buffer);
	}
	m_buffer = NULL;
	m_readCursor = NULL;
}

// FUNCTION: LEMBALL 0x004096a0
void CDemo::GameIsOver()
{
	m_gameOver = 1;
	CleanUp();
}

// FUNCTION: LEMBALL 0x004096b0
int CDemo::ProcessMsg(Message* p_message)
{
	if (m_demoMode != 0) {
		unsigned short type = p_message->m_type;
		if ((type & DEMO_MESSAGE_RECORDED_FLAG) != 0) {
			p_message->m_type = type & DEMO_MESSAGE_TYPE_MASK;
		}
		else {
			switch (type) {
			case MESSAGE_RAW_KEY_UP:
			case MESSAGE_KEY_UP:
			case MESSAGE_MOUSE_BUTTON_UP:
				GameIsOver();
				return 1;
			case MESSAGE_MOUSE_MOVED:
			case MESSAGE_WINDOW_COMMAND:
				break;
			default:
				return 1;
			}
		}
	}
	return 0;
}

// GLOBAL: LEMBALL 0x004a62a4
int g_nDemoMode = 0;

// GLOBAL: LEMBALL 0x004a62a8
char g_szDemoFilePath[80];

// GLOBAL: LEMBALL 0x004a6408
CDemo* g_pDemo;
