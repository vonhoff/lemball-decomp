#include "NetworkMessage.h"

#include "../Foundation/BaseQueue.h"
#include "../Foundation/BaseQueueHandler.h"
#include "../Foundation/VsMem.h"
#include "../Network/BaseNetwork.h"
#include "BasePacketHeader.h"

#include <string.h>

#pragma intrinsic(strcpy, strlen, memcpy, strcmp)

// 68K 0x101077ce CheckMessage__15CNetworkMessageFPCUc
// FUNCTION: LEMBALL 0x0040abd0
bool NetworkMessage::CheckMessage(const unsigned char* p_arg0)
{
	return 1;
}

// 68K 0x10107800 GetHeader__15CNetworkMessageFv
// FUNCTION: LEMBALL 0x0040abe0
bool NetworkMessage::GetHeader()
{
	return 1;
}

// 68K 0x1010782c AddHeader__15CNetworkMessageFv
// FUNCTION: LEMBALL 0x0040abf0
void NetworkMessage::AddHeader()
{
}

void NetworkMessage::GetData()
{
}

void NetworkMessage::AddData()
{
}

// 68K 0x1020a4ce __dt__15CNetworkMessageFv
NetworkMessage::~NetworkMessage()
{
}

// 68K 0x1020a488 Initialise__15CNetworkMessageFv
// FUNCTION: LEMBALL 0x0045ee80
void NetworkMessage::Initialise()
{
	m_headerEnabled = 0;
	m_pendingSendCount = 0;
	m_ownsBuffer = 0;
	m_buffer = 0;
	m_payloadCapacity = 0;
	m_openDepth = 0;
}

// 68K 0x1020a574 Add__15CNetworkMessageFUl
// FUNCTION: LEMBALL 0x0045ef10
void NetworkMessage::Add(unsigned long p_value)
{
	const unsigned char* value;

	value = (const unsigned char*) &p_value;
	*(m_writeCursor + 3) = value[0];
	*(m_writeCursor + 2) = value[1];
	*(m_writeCursor + 1) = value[2];
	*m_writeCursor = value[3];
	m_writeCursor += 4;
}

// 68K 0x1020a5ac Add__15CNetworkMessageFUs
// FUNCTION: LEMBALL 0x0045ef40
void NetworkMessage::Add(unsigned short p_value)
{
	const unsigned char* value;

	value = (const unsigned char*) &p_value;
	*(m_writeCursor + 1) = value[0];
	*m_writeCursor = value[1];
	m_writeCursor += 2;
}

// 68K 0x1020a5e4 Add__15CNetworkMessageFUc
// FUNCTION: LEMBALL 0x0045ef60
void NetworkMessage::Add(unsigned char p_value)
{
	*m_writeCursor = p_value;
	++m_writeCursor;
}

// 68K 0x1020a618 Add__15CNetworkMessageFPCc
// FUNCTION: LEMBALL 0x0045ef70
void NetworkMessage::Add(const char* p_text)
{
	int length = strlen(p_text);
	strcpy((char*) m_writeCursor, p_text);
	m_writeCursor += length + 1;
}

// 68K 0x1020a670 Add__15CNetworkMessageFPCUci
// FUNCTION: LEMBALL 0x0045efc0
void NetworkMessage::Add(const unsigned char* p_data, int p_length)
{
	memcpy(m_writeCursor, p_data, p_length);
	m_writeCursor += p_length;
}

// 68K 0x1020a6c6 GetDWORD__15CNetworkMessageFv
// FUNCTION: LEMBALL 0x0045eff0
int NetworkMessage::GetDword()
{
	unsigned long value;

	Get(value);
	return (int) value;
}

// 68K 0x1020a700 Get__15CNetworkMessageFRUl
// FUNCTION: LEMBALL 0x0045f010
void NetworkMessage::Get(unsigned long& p_value)
{
	unsigned char* target;

	target = (unsigned char*) &p_value;
	target[0] = *(m_readCursor + 3);
	target[1] = *(m_readCursor + 2);
	target[2] = *(m_readCursor + 1);
	target[3] = *m_readCursor;
	m_readCursor += 4;
}

// 68K 0x1020a73c ConvertDWORD__15CNetworkMessageFv
// FUNCTION: LEMBALL 0x0045f040
void NetworkMessage::ConvertDword()
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

// 68K 0x1020a772 GetWORD__15CNetworkMessageFv
// FUNCTION: LEMBALL 0x0045f070
unsigned short NetworkMessage::GetWord()
{
	unsigned short value;

	Get(value);
	return value;
}

// 68K 0x1020a7ac Get__15CNetworkMessageFRUs
// FUNCTION: LEMBALL 0x0045f090
void NetworkMessage::Get(unsigned short& p_value)
{
	unsigned char* target;

	target = (unsigned char*) &p_value;
	target[0] = *(m_readCursor + 1);
	target[1] = *m_readCursor;
	m_readCursor += 2;
}

// 68K 0x1020a7e8 ConvertWORD__15CNetworkMessageFv
// FUNCTION: LEMBALL 0x0045f0b0
void NetworkMessage::ConvertWord()
{
	unsigned char byte0;
	unsigned char byte1;

	byte1 = m_readCursor[1];
	byte0 = m_readCursor[0];
	m_readCursor[1] = byte0;
	m_readCursor[0] = byte1;
	m_readCursor += 2;
}

// 68K 0x1020a81c GetBYTE__15CNetworkMessageFv
// FUNCTION: LEMBALL 0x0045f0d0
unsigned char NetworkMessage::GetByte()
{
	unsigned char value;

	Get(value);
	return value;
}

// 68K 0x1020a856 Get__15CNetworkMessageFRUc
// FUNCTION: LEMBALL 0x0045f0f0
void NetworkMessage::Get(unsigned char& p_value)
{
	p_value = *m_readCursor;
	++m_readCursor;
}

// 68K 0x1020a88e ConvertBYTE__15CNetworkMessageFv
// FUNCTION: LEMBALL 0x0045f110
void NetworkMessage::ConvertByte()
{
	++m_readCursor;
}

// 68K 0x1020a8c2 Get__15CNetworkMessageFRPCc
// FUNCTION: LEMBALL 0x0045f120
void NetworkMessage::Get(const char*& p_text)
{
	p_text = (const char*) m_readCursor;
	m_readCursor += (int) strlen(p_text) + 1;
}

// 68K 0x1020a90e Get__15CNetworkMessageFRPCUci
// FUNCTION: LEMBALL 0x0045f140
void NetworkMessage::Get(const unsigned char*& p_data, int p_length)
{
	p_data = m_readCursor;
	m_readCursor += p_length;
}

// 68K 0x1020a94a GetCopy__15CNetworkMessageFPc
// FUNCTION: LEMBALL 0x0045f160
void NetworkMessage::GetCopy(char* p_buffer)
{
	strcpy(p_buffer, (char*) m_readCursor);
	m_readCursor += strlen(p_buffer) + 1;
}

// 68K 0x1020a9a0 GetCopy__15CNetworkMessageFPUci
// FUNCTION: LEMBALL 0x0045f1b0
void NetworkMessage::GetCopy(unsigned char* p_buffer, int p_length)
{
	memcpy(p_buffer, m_readCursor, p_length);
	m_readCursor += p_length;
}

// 68K 0x1020a9f8 OpenDataStream__15CNetworkMessageFv
// FUNCTION: LEMBALL 0x0045f1e0
void NetworkMessage::OpenDataStream()
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

// 68K 0x1020aaa4 CloseDataStream__15CNetworkMessageFv
// FUNCTION: LEMBALL 0x0045f240
void NetworkMessage::CloseDataStream()
{
	--m_openDepth;
}

// 68K 0x1020aadc CopyDataStream__15CNetworkMessageFPUci
// FUNCTION: LEMBALL 0x0045f250
void NetworkMessage::CopyDataStream(unsigned char* p_buffer, int p_offset)
{
	unsigned char* end;

	m_buffer = p_buffer;
	end = p_buffer + p_offset;
	m_writeCursor = end;
	m_bufferEnd = end + m_payloadCapacity;
	AddHeader();
	AddData();
}

// 68K 0x1020ab56 Set__15CNetworkMessageFPUc
// FUNCTION: LEMBALL 0x0045f280
bool NetworkMessage::Set(unsigned char* p_data)
{
	m_readCursor = p_data;
	if (GetHeader() != 0) {
		GetData();
		return 1;
	}
	return 0;
}

// 68K 0x1020ab3a Send__15CNetworkMessageFP8CConnect
// FUNCTION: LEMBALL 0x0045f2b0
void NetworkMessage::Send(Connect* p_connection)
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
