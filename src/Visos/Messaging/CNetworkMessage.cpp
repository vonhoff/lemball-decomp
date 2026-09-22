#include "CNetworkMessage.h"

#include "../Foundation/CBaseQueue.h"
#include "../Foundation/CBaseQueueHandler.h"
#include "../Foundation/VsTime.h"
#include "../Network/CBaseNetwork.h"
#include "BasePacketHeader.h"
#include "Visos/Foundation/Message.h"

#include <string.h>

#pragma intrinsic(strcpy, strlen, memcpy, strcmp)

// FUNCTION: LEMBALL 0x0040abd0
bool CNetworkMessage::CheckMessage(const unsigned char* p_arg0)
{
	return 1;
}

// FUNCTION: LEMBALL 0x0040abe0
bool CNetworkMessage::GetHeader()
{
	return 1;
}

// FUNCTION: LEMBALL 0x0040abf0
void CNetworkMessage::AddHeader()
{
}

void CNetworkMessage::GetData()
{
}

void CNetworkMessage::AddData()
{
}

// FUNCTION: LEMBALL 0x0045ee80
void CNetworkMessage::Initialise()
{
	m_headerEnabled = 0;
	m_pendingSendCount = 0;
	m_ownsBuffer = 0;
	m_buffer = 0;
	m_payloadCapacity = 0;
	m_openDepth = 0;
}

// FUNCTION: LEMBALL 0x0045eea0
CNetworkMessage::~CNetworkMessage()
{
	if (m_pendingSendCount != 0) {
		unsigned long start = CurrentMilliTimer();
		while (m_pendingSendCount != 0) {
			if (CurrentMilliTimer() - start >= 2000) {
				break;
			}
			if (g_pBaseNetwork != 0) {
				g_pBaseNetwork->WaitProcess();
			}
		}
	}
	if (m_ownsBuffer != 0) {
		operator delete(m_buffer);
		m_buffer = 0;
	}
}

// FUNCTION: LEMBALL 0x0045ef10
void CNetworkMessage::Add(unsigned long p_value)
{
	const unsigned char* value;

	value = (const unsigned char*) &p_value;
	*(m_writeCursor + 3) = value[0];
	*(m_writeCursor + 2) = value[1];
	*(m_writeCursor + 1) = value[2];
	*m_writeCursor = value[3];
	m_writeCursor += 4;
}

// FUNCTION: LEMBALL 0x0045ef40
void CNetworkMessage::Add(unsigned short p_value)
{
	const unsigned char* value;

	value = (const unsigned char*) &p_value;
	*(m_writeCursor + 1) = value[0];
	*m_writeCursor = value[1];
	m_writeCursor += 2;
}

// FUNCTION: LEMBALL 0x0045ef60
void CNetworkMessage::Add(unsigned char p_value)
{
	*m_writeCursor = p_value;
	++m_writeCursor;
}

// FUNCTION: LEMBALL 0x0045ef70
void CNetworkMessage::Add(const char* p_text)
{
	int length = strlen(p_text);
	strcpy((char*) m_writeCursor, p_text);
	m_writeCursor += length + 1;
}

// FUNCTION: LEMBALL 0x0045efc0
void CNetworkMessage::Add(const unsigned char* p_data, int p_length)
{
	memcpy(m_writeCursor, p_data, p_length);
	m_writeCursor += p_length;
}

// FUNCTION: LEMBALL 0x0045eff0
int CNetworkMessage::GetDWORD()
{
	unsigned long value;

	Get(value);
	return (int) value;
}

// FUNCTION: LEMBALL 0x0045f010
void CNetworkMessage::Get(unsigned long& p_value)
{
	unsigned char* target;

	target = (unsigned char*) &p_value;
	target[0] = *(m_readCursor + 3);
	target[1] = *(m_readCursor + 2);
	target[2] = *(m_readCursor + 1);
	target[3] = *m_readCursor;
	m_readCursor += 4;
}

// FUNCTION: LEMBALL 0x0045f040
void CNetworkMessage::ConvertDWORD()
{
	unsigned char byte0;
	unsigned char byte1;

	byte0 = m_readCursor[0];
	byte1 = m_readCursor[1];
	m_readCursor[0] = m_readCursor[3];
	m_readCursor[1] = m_readCursor[2];
	m_readCursor[2] = byte1;
	m_readCursor[3] = byte0;
	m_readCursor += 4;
}

// FUNCTION: LEMBALL 0x0045f070
unsigned short CNetworkMessage::GetWORD()
{
	unsigned short value;

	Get(value);
	return value;
}

// FUNCTION: LEMBALL 0x0045f090
void CNetworkMessage::Get(unsigned short& p_value)
{
	unsigned char* target;

	target = (unsigned char*) &p_value;
	target[0] = *(m_readCursor + 1);
	target[1] = *m_readCursor;
	m_readCursor += 2;
}

// FUNCTION: LEMBALL 0x0045f0b0
void CNetworkMessage::ConvertWORD()
{
	unsigned char byte0;
	unsigned char byte1;

	byte1 = m_readCursor[1];
	byte0 = m_readCursor[0];
	m_readCursor[1] = byte0;
	m_readCursor[0] = byte1;
	m_readCursor += 2;
}

// FUNCTION: LEMBALL 0x0045f0d0
unsigned char CNetworkMessage::GetBYTE()
{
	unsigned char value;

	Get(value);
	return value;
}

// FUNCTION: LEMBALL 0x0045f0f0
void CNetworkMessage::Get(unsigned char& p_value)
{
	p_value = *m_readCursor;
	++m_readCursor;
}

// FUNCTION: LEMBALL 0x0045f110
void CNetworkMessage::ConvertBYTE()
{
	++m_readCursor;
}

// FUNCTION: LEMBALL 0x0045f120
void CNetworkMessage::Get(const char*& p_text)
{
	p_text = (const char*) m_readCursor;
	m_readCursor += (int) strlen(p_text) + 1;
}

// FUNCTION: LEMBALL 0x0045f140
void CNetworkMessage::Get(const unsigned char*& p_data, int p_length)
{
	p_data = m_readCursor;
	m_readCursor += p_length;
}

// FUNCTION: LEMBALL 0x0045f160
void CNetworkMessage::GetCopy(char* p_buffer)
{
	strcpy(p_buffer, (char*) m_readCursor);
	m_readCursor += strlen(p_buffer) + 1;
}

// FUNCTION: LEMBALL 0x0045f1b0
void CNetworkMessage::GetCopy(unsigned char* p_buffer, int p_length)
{
	memcpy(p_buffer, m_readCursor, p_length);
	m_readCursor += p_length;
}

// FUNCTION: LEMBALL 0x0045f1e0
void CNetworkMessage::OpenDataStream()
{
	if (m_openDepth++ == 0) {
		unsigned int allocSize = m_payloadCapacity + sizeof(BasePacketHeader);
		if (m_ownsBuffer == 0) {
			m_buffer = (unsigned char*) operator new(allocSize);
			m_ownsBuffer = 1;
			m_writeCursor = m_buffer + sizeof(BasePacketHeader);
			m_bufferEnd = m_writeCursor + m_payloadCapacity;
		}
		else {
			m_writeCursor = m_buffer + sizeof(BasePacketHeader);
		}
		AddHeader();
		AddData();
	}
}

// FUNCTION: LEMBALL 0x0045f240
void CNetworkMessage::CloseDataStream()
{
	--m_openDepth;
}

// FUNCTION: LEMBALL 0x0045f250
void CNetworkMessage::CopyDataStream(unsigned char* p_buffer, int p_offset)
{
	unsigned char* end;

	m_buffer = p_buffer;
	end = p_buffer + p_offset;
	m_writeCursor = end;
	m_bufferEnd = end + m_payloadCapacity;
	AddHeader();
	AddData();
}

// FUNCTION: LEMBALL 0x0045f280
bool CNetworkMessage::Set(unsigned char* p_data)
{
	m_readCursor = p_data;
	if (GetHeader() != 0) {
		GetData();
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0045f2b0
void CNetworkMessage::Send(CConnect* p_connection)
{
	Message message;

	if (p_connection != 0) {
		message.type = 0xb;
		message.code = 1;
		message.payload = this;
		message.source = p_connection;
		OpenDataStream();
		m_pendingSendCount = 1;
		g_pNetworkStatusQueue->Post(message);
		g_pBaseNetwork->ForceProcess();
	}
}
