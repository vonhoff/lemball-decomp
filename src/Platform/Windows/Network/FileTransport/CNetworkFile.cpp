#include "Platform/Windows/Network/FileTransport/CNetworkFile.h"

#include <string.h>

#include "Platform/Windows/WindowsTypes.h"
#include <winsock.h>

#include "Engine/Streams/CVSOStream.h"

enum eSetFilePointerResult {
	SET_FILE_POINTER_FAILURE = 0xffffffffUL
};

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
	unsigned long creation;
	HANDLE handle;

	(void) p_mode;
	length = strlen(p_filename) + 1;
	m_filename = new char[length];
	strcpy(m_filename, p_filename);
	if (p_create == 0) {
		creation = OPEN_EXISTING;
	}
	else {
		creation = CREATE_NEW;
	}
	handle = CreateFileA(p_filename,
						 GENERIC_READ | GENERIC_WRITE,
						 FILE_SHARE_READ | FILE_SHARE_WRITE,
						 NULL,
						 creation,
						 FILE_ATTRIBUTE_NORMAL,
						 NULL);
	m_handle = handle;
	if (handle == INVALID_HANDLE_VALUE) {
		return false;
	}

	m_closed = 0;
	m_position = 0;
	return true;
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
	HANDLE handle;

	handle = CreateFileA(p_filename,
						 GENERIC_READ | GENERIC_WRITE,
						 FILE_SHARE_READ | FILE_SHARE_WRITE,
						 NULL,
						 OPEN_EXISTING,
						 FILE_ATTRIBUTE_NORMAL,
						 NULL);
	m_handle = handle;
	if (handle == INVALID_HANDLE_VALUE) {
		return false;
	}

	CloseHandle(handle);
	return true;
}

// FUNCTION: LEMBALL 0x0047f6c0
bool CNetworkFile::Write(const unsigned char* p_data, int p_size)
{
	int success;
	unsigned long written;

	success = WriteFile(m_handle, p_data, p_size, &written, NULL);
	if (success == 0) {
		int error = GetLastError();
		*g_pErrorOutput << "Write error: " << error << "\n";
		return false;
	}

	m_position += written;
	if (written != (unsigned int) p_size) {
		*g_pErrorOutput << "Write error: " << written << " bytes written instead of " << p_size << "\n";
		return false;
	}
	return true;
}

// FUNCTION: LEMBALL 0x0047f780
bool CNetworkFile::Read(unsigned char* p_data, int p_size)
{
	int success;
	unsigned long got;
	int error;

	success = ReadFile(m_handle, p_data, p_size, &got, NULL);
	if (success == 0) {
		error = GetLastError();
		if (error != ERROR_LOCK_VIOLATION) {
			*g_pErrorOutput << "Read error: " << error << "\n";
		}
		return false;
	}

	m_position += got;
	if (got != (unsigned int) p_size) {
		*g_pErrorOutput << "Read error: " << got << " bytes read instead of " << p_size << "\n";
		*g_pErrorOutput << "in file " << m_filename << " which is " << GetFileSize() << " bytes long.\n";
		return false;
	}
	return true;
}

// FUNCTION: LEMBALL 0x0047f890
bool CNetworkFile::Seek(int p_position)
{
	m_position = p_position;
	return SetFilePointer(m_handle, p_position, NULL, FILE_BEGIN) != SET_FILE_POINTER_FAILURE;
}

// FUNCTION: LEMBALL 0x0047f8b0
int CNetworkFile::Tell()
{
	return m_position;
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
unsigned long CNetworkFile::GetFileSize()
{
	return ::GetFileSize(m_handle, NULL);
}

// FUNCTION: LEMBALL 0x0047f900
bool CNetworkFile::Lock(unsigned long p_offset, unsigned long p_length)
{
	return LockFile(m_handle, p_offset, 0, p_length, 0);
}

// FUNCTION: LEMBALL 0x0047f920
bool CNetworkFile::UnLock(unsigned long p_offset, unsigned long p_length)
{
	return UnlockFile(m_handle, p_offset, 0, p_length, 0);
}
