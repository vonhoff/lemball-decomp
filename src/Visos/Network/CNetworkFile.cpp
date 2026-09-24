#include "CNetworkFile.h"

#include "../Foundation/CVSOStream.h"

#include <string.h>

extern "C" __declspec(dllimport) void* __stdcall CreateFileA(const char* p_name,
															 unsigned int p_access,
															 unsigned int p_share,
															 void* p_security,
															 unsigned int p_creation,
															 unsigned int p_flags,
															 void* p_template);
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* p_handle);
extern "C" __declspec(dllimport) int __stdcall DeleteFileA(const char* p_name);
extern "C" __declspec(dllimport) int __stdcall WriteFile(void* p_handle,
														 const void* p_buffer,
														 unsigned int p_bytes,
														 unsigned int* p_written,
														 void* p_overlapped);
extern "C" __declspec(dllimport) int __stdcall ReadFile(void* p_handle,
														void* p_buffer,
														unsigned int p_bytes,
														unsigned int* p_read,
														void* p_overlapped);
extern "C" __declspec(dllimport) unsigned int __stdcall SetFilePointer(void* p_handle,
																	   int p_distance,
																	   int* p_distanceHigh,
																	   unsigned int p_method);
extern "C" __declspec(dllimport) unsigned int __stdcall GetFileSize(void* p_handle, unsigned int* p_sizeHigh);
extern "C" __declspec(dllimport) int __stdcall LockFile(void* p_handle,
														unsigned int p_offsetLow,
														unsigned int p_offsetHigh,
														unsigned int p_lengthLow,
														unsigned int p_lengthHigh);
extern "C" __declspec(dllimport) int __stdcall UnlockFile(void* p_handle,
														  unsigned int p_offsetLow,
														  unsigned int p_offsetHigh,
														  unsigned int p_lengthLow,
														  unsigned int p_lengthHigh);
extern "C" __declspec(dllimport) unsigned int __stdcall GetLastError();
extern "C" int __stdcall WSAGetLastError();

// FUNCTION: LEMBALL 0x0047b8d0
int CNetworkFile::GetLastError()
{
	return WSAGetLastError();
}

// FUNCTION: LEMBALL 0x0047b8e0
char* CNetworkFile::GetFilename()
{
	return m_filename;
}

// FUNCTION: LEMBALL 0x0047f5b0
bool CNetworkFile::Open(const char* p_filename, unsigned char p_mode, int p_create)
{
	unsigned int length;
	unsigned int creation;
	void* handle;

	(void) p_mode;
	length = strlen(p_filename) + 1;
	m_filename = (char*) operator new(length);
	strcpy(m_filename, p_filename);
	if (p_create == 0) {
		creation = 3;
	}
	else {
		creation = 1;
	}
	handle = CreateFileA(p_filename, 0xc0000000, 3, 0, creation, 0x80, 0);
	m_handle = handle;
	if (handle == (void*) -1) {
		return 0;
	}

	m_closed = 0;
	m_position = 0;
	return 1;
}

// FUNCTION: LEMBALL 0x0047f640
bool CNetworkFile::Create(const char* p_filename, unsigned char p_mode)
{
	m_position = 0;
	return Open(p_filename, p_mode, 1);
}

// FUNCTION: LEMBALL 0x0047f660
bool CNetworkFile::Delete()
{
	return DeleteFileA(m_filename);
}

// FUNCTION: LEMBALL 0x0047f670
bool CNetworkFile::Delete(const char* p_filename)
{
	return DeleteFileA(p_filename);
}

// FUNCTION: LEMBALL 0x0047f680
bool CNetworkFile::CheckExists(const char* p_filename)
{
	void* handle;

	handle = CreateFileA(p_filename, 0xc0000000, 3, 0, 3, 0x80, 0);
	m_handle = handle;
	if (handle == (void*) -1) {
		return 0;
	}

	CloseHandle(handle);
	return 1;
}

// FUNCTION: LEMBALL 0x0047f6c0
bool CNetworkFile::Write(const unsigned char* p_data, int p_size)
{
	int success;
	unsigned int written;

	success = WriteFile(m_handle, p_data, (unsigned int) p_size, &written, 0);
	if (success == 0) {
		int error = (int) GetLastError();
		*g_pErrorOutput << "Write error: " << error << "\n";
		return 0;
	}

	m_position += written;
	if (written != (unsigned int) p_size) {
		*g_pErrorOutput << "Write error: " << (unsigned long) written << " bytes written instead of " << p_size << "\n";
		return 0;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x0047f780
bool CNetworkFile::Read(unsigned char* p_data, int p_size)
{
	int success;
	unsigned int got;
	unsigned int error;

	success = ReadFile(m_handle, p_data, (unsigned int) p_size, &got, 0);
	if (success == 0) {
		error = GetLastError();
		if (error != 0x21) {
			*g_pErrorOutput << "Read error: " << (int) error << "\n";
		}
		return 0;
	}

	m_position += got;
	if (got != (unsigned int) p_size) {
		*g_pErrorOutput << "Read error: " << (unsigned long) got << " bytes read instead of " << p_size << "\n";
		*g_pErrorOutput << "in file " << m_filename << " which is " << (unsigned long) GetFileSize()
						<< " bytes long.\n";
		return 0;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x0047f890
bool CNetworkFile::Seek(int p_position)
{
	m_position = (unsigned int) p_position;
	return SetFilePointer(m_handle, p_position, 0, 0) != 0xffffffff;
}

// FUNCTION: LEMBALL 0x0047f8b0
int CNetworkFile::Tell()
{
	return (int) m_position;
}

// FUNCTION: LEMBALL 0x0047f8c0
bool CNetworkFile::Close()
{
	int closed;

	closed = 1;
	if (m_closed == 0) {
		closed = CloseHandle(m_handle);
		if (closed != 0) {
			m_closed = 1;
		}
	}
	return closed;
}

// FUNCTION: LEMBALL 0x0047f8f0
unsigned int CNetworkFile::GetFileSize()
{
	return ::GetFileSize(m_handle, 0);
}

// FUNCTION: LEMBALL 0x0047f900
bool CNetworkFile::Lock(unsigned int p_offset, unsigned int p_length)
{
	return LockFile(m_handle, p_offset, 0, p_length, 0);
}

// FUNCTION: LEMBALL 0x0047f920
bool CNetworkFile::UnLock(unsigned int p_offset, unsigned int p_length)
{
	return UnlockFile(m_handle, p_offset, 0, p_length, 0);
}
