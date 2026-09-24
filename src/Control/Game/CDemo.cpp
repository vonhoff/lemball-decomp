#include "CDemo.h"

#include "../../Visos/Foundation/CBaseQueue.h"
#include "../../Visos/Foundation/Message.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Graphics/CPVWnd.h"
#include "../../Visos/Messaging/PackParam.h"
#include "../../Visos/Resources/CResBIN.h"

// FUNCTION: LEMBALL 0x004091b0
CDemo::CDemo(int p_sourceId)
{
	m_offsetY = 0;
	m_offsetX = 0;
	m_sourceId = p_sourceId;
	m_buffer = 0;
	m_window = 0;
	m_currentResourceId = 0;
	m_firstResourceId = 0;
	m_resourceCount = 0;
	m_resource = 0;
	m_filePath = 0;
	m_demoMode = 0;
	m_state48 = 0;
	m_gameOver = 0;
	m_bytesRemaining = -1;
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
	if (m_bytesRemaining == -1 && !LoadBuffer()) {
		return false;
	}
	if (*m_readCursor != (p_packetIndex & 0xff)) {
		return false;
	}
	Message message;
	message.time = CurrentQueueTimer();
	m_readCursor++;
	message.type = m_readCursor[0];
	message.type |= (unsigned short) m_readCursor[1] << 8;
	m_readCursor += 2;
	message.code = m_readCursor[0];
	message.code |= (unsigned int) m_readCursor[1] << 8;
	message.code |= (unsigned int) m_readCursor[2] << 16;
	message.code |= (unsigned int) m_readCursor[3] << 24;
	m_readCursor += 4;
	message.payload = (void*) m_readCursor[0];
	message.payload = (void*) ((unsigned int) message.payload | (unsigned int) m_readCursor[1] << 8);
	message.payload = (void*) ((unsigned int) message.payload | (unsigned int) m_readCursor[2] << 16);
	message.payload = (void*) ((unsigned int) message.payload | (unsigned int) m_readCursor[3] << 24);
	m_readCursor += 4;
	message.source = (void*) m_readCursor[0];
	message.source = (void*) ((unsigned int) message.source | (unsigned int) m_readCursor[1] << 8);
	message.source = (void*) ((unsigned int) message.source | (unsigned int) m_readCursor[2] << 16);
	message.source = (void*) ((unsigned int) message.source | (unsigned int) m_readCursor[3] << 24);
	m_readCursor += 4;
	if (m_window == 0) {
		return false;
	}
	switch ((unsigned int) message.type) {
	case 5:
	case 6:
	case 8:
	case 9: {
		CVsPoint point((short) message.code, (short) ((unsigned int) message.code >> 16));
		short zoom = (short) m_window->m_zoom;
		point.m_x *= zoom;
		point.m_y *= zoom;
		point.m_x += m_window->m_rect.m_x;
		point.m_y += m_window->m_rect.m_y;
		point.m_x += m_offsetX;
		point.m_y += m_offsetY;
		const CVsRect& bounds = m_window->m_rect;
		if (point.m_x < bounds.m_x || (short) (bounds.m_width + bounds.m_x) <= point.m_x || point.m_y < bounds.m_y ||
			(short) (bounds.m_height + bounds.m_y) <= point.m_y) {
			return false;
		}
		message.code = PackParam(point.m_x, point.m_y);
		break;
	}
	}
	message.type |= 0x8000;
	g_pMasterInputQueue->Post(message);
	return true;
}

#include "../../Visos/Foundation/VsFile.h"

// FUNCTION: LEMBALL 0x00409460
bool CDemo::LoadBuffer()
{
	if (m_filePath != 0) {
		_Filet* file = vsOpen(m_filePath, "rb");
		if (file == 0) {
			return 0;
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
	return 1;
}

#include "Visos/Foundation/CVsPoint.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Foundation/Message.h"

#include <string.h>

struct _Filet;

// FUNCTION: LEMBALL 0x00409560
void CDemo::GetUserPacket(unsigned char* p_data, unsigned long& p_size)
{
	if (m_bytesRemaining == -1) {
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
	m_bytesRemaining = -1;
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
	if (m_resource != 0) {
		m_resource->m_directUseCount--;
		m_resource->UnLoad();
		m_resource = 0;
	}
	else if (m_buffer != 0) {
		operator delete(m_buffer);
	}
	m_buffer = 0;
	m_readCursor = 0;
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
		unsigned short type = p_message->type;
		if ((type & 0x8000) != 0) {
			p_message->type = type & 0x7fff;
		}
		else {
			switch (type) {
			case 1:
			case 3:
			case 5:
				GameIsOver();
				return 1;
			case 7:
			case 15:
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
