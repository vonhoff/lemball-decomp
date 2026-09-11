#include "Demo.h"

#include "../../Visos/Foundation/BaseQueue.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Resources/ResBin.h"

// 68K 0x10700a70 __ct__5CDemoFi
// FUNCTION: LEMBALL 0x004091b0
Demo::Demo(int p_arg0)
{
	m_offsetY = 0;
	m_offsetX = 0;
	m_sourceId = p_arg0;
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

// 68K 0x10700b20 __dt__5CDemoFv
// FUNCTION: LEMBALL 0x00409220
Demo::~Demo()
{
	CleanUp();
	g_pMasterInputQueue->Detach(this, -100);
}

// 68K 0x10700b8a SendNextPacket__5CDemoFi
// STUB: LEMBALL 0x00409250
bool Demo::SendNextPacket(int p_packetIndex)
{
	return 0;
}

#include "../../Visos/Foundation/VsFile.h"

// 68K 0x10700e38 LoadBuffer__5CDemoFv
// FUNCTION: LEMBALL 0x00409460
bool Demo::LoadBuffer()
{
	if (m_filePath != 0) {
		_Filet* file = VsOpen(m_filePath, "rb");
		if (file == 0) {
			return 0;
		}
		unsigned long size = VsGetFileSize(file);
		m_buffer = new unsigned char[size];
		unsigned long bytesRead = VsRead(file, m_buffer, size);
		VsClose(file);
		m_bytesRemaining = bytesRead;
	}
	else {
		ResBin* resource = ResBin::Load(m_currentResourceId);
		m_resource = resource;
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

// 68K 0x10700f82 GetUserPacket__5CDemoFPUcRUl
// STUB: LEMBALL 0x00409560
void Demo::GetUserPacket(unsigned char* p_data, unsigned long& p_size)
{
}

// 68K 0x10701032 Reset__5CDemoFv
// FUNCTION: LEMBALL 0x004095e0
void Demo::Reset()
{
	m_bytesRemaining = -1;
	m_packetIndex = 0;
	m_gameOver = 0;
	m_duration = 0;
}

// 68K 0x10701062 SetDemoMode__5CDemoFUc
// FUNCTION: LEMBALL 0x00409600
void Demo::SetDemoMode(int p_enabled)
{
	m_demoMode = p_enabled;
	if (p_enabled != 0) {
		Reset();
	}
	else {
		CleanUp();
	}
}

// 68K 0x107010ac Process__5CDemoFv
// FUNCTION: LEMBALL 0x00409620
void Demo::Process()
{
	if (m_gameOver == 0 && m_demoMode != 0) {
		unsigned long current = CurrentMilliTimer();
		if (m_duration != 0 && m_duration <= current - m_startTime) {
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

// 68K 0x10701116 CleanUp__5CDemoFv
// FUNCTION: LEMBALL 0x00409660
void Demo::CleanUp()
{
	if (m_resource != 0) {
		m_resource->m_directUseCount--;
		m_resource->UnLoad();
		m_resource = 0;
		m_buffer = 0;
		m_readCursor = 0;
		return;
	}
	if (m_buffer != 0) {
		operator delete(m_buffer);
	}
	m_buffer = 0;
	m_readCursor = 0;
}

// 68K 0x10701178 GameIsOver__5CDemoFv
// FUNCTION: LEMBALL 0x004096a0
void Demo::GameIsOver()
{
	m_gameOver = 1;
	CleanUp();
}

// 68K 0x107011aa ProcessMsg__5CDemoFP10tagMESSAGE
// FUNCTION: LEMBALL 0x004096b0
int Demo::ProcessMsg(Message* p_message)
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
Demo* g_pDemo;
