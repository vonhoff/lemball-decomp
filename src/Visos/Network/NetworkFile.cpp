#include "NetworkFile.h"

#include "../Foundation/VsOStream.h"

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

// 68K 0x10107856 GetLastError__12CNetworkFileFv
// FUNCTION: LEMBALL 0x0047b8d0
int NetworkFile::GetLastError()
{
	return (int) ::GetLastError();
}

// 68K 0x1010788a GetFilename__12CNetworkFileFv
// FUNCTION: LEMBALL 0x0047b8e0
char* NetworkFile::GetFilename()
{
	return m_filename;
}

// 68K 0x1010f5d0 Open__12CNetworkFileFPCcUcUc
// FUNCTION: LEMBALL 0x0047f5b0
bool NetworkFile::Open(const char* p_filename, unsigned char p_mode, unsigned char p_create)
{
	unsigned int length;
	char* copy;
	unsigned int creation;
	void* handle;

	(void) p_mode;
	length = strlen(p_filename) + 1;
	copy = (char*) operator new(length);
	m_filename = copy;
	memcpy(copy, p_filename, length);
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

// 68K 0x1010f67e Create__12CNetworkFileFPCcUc
// FUNCTION: LEMBALL 0x0047f640
bool NetworkFile::Create(const char* p_filename, unsigned char p_mode)
{
	m_position = 0;
	return Open(p_filename, p_mode, 1);
}

// 68K 0x1010f6c2 Delete__12CNetworkFileFv
// FUNCTION: LEMBALL 0x0047f660
bool NetworkFile::Delete()
{
	return DeleteFileA(m_filename) != 0;
}

// 68K 0x1010f70a Delete__12CNetworkFileFPCc
// FUNCTION: LEMBALL 0x0047f670
bool NetworkFile::Delete(const char* p_filename)
{
	return DeleteFileA(p_filename) != 0;
}

// 68K 0x1010f77e CheckExists__12CNetworkFileFPCc
// FUNCTION: LEMBALL 0x0047f680
bool NetworkFile::CheckExists(const char* p_filename)
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

// 68K 0x1010f7e4 Write__12CNetworkFileFPCUci
// FUNCTION: LEMBALL 0x0047f6c0
bool NetworkFile::Write(const unsigned char* p_data, int p_size)
{
	int success;
	unsigned int written;

	success = WriteFile(m_handle, p_data, (unsigned int) p_size, &written, 0);
	if (success == 0) {
		// STRING: LEMBALL 0x004a41f8 "Write error: "
		*g_pErrorOutput << "Write error: " << (int) ::GetLastError() << "\n";
		return 0;
	}

	m_position += written;
	if ((unsigned int) p_size != written) {
		// STRING: LEMBALL 0x004a41f8 "Write error: "
		// STRING: LEMBALL 0x004a421c " bytes written instead of "
		*g_pErrorOutput << "Write error: " << (unsigned long) written << " bytes written instead of " << p_size
						<< "\n";
		return 0;
	}
	return 1;
}

// 68K 0x1010f94c Read__12CNetworkFileFPUci
// FUNCTION: LEMBALL 0x0047f780
bool NetworkFile::Read(unsigned char* p_data, int p_size)
{
	int success;
	unsigned int got;
	unsigned int error;

	success = ReadFile(m_handle, p_data, (unsigned int) p_size, &got, 0);
	if (success == 0) {
		error = ::GetLastError();
		if (error != 0x21) {
			// STRING: LEMBALL 0x004a423c "Read error: "
			*g_pErrorOutput << "Read error: " << (int) error << "\n";
		}
		return 0;
	}

	m_position += got;
	if (got != (unsigned int) p_size) {
		// STRING: LEMBALL 0x004a423c "Read error: "
		// STRING: LEMBALL 0x004a4260 " bytes read instead of "
		// STRING: LEMBALL 0x004a427c "in file "
		// STRING: LEMBALL 0x004a4288 " which is "
		// STRING: LEMBALL 0x004a4294 " bytes long.\n"
		*g_pErrorOutput << "Read error: " << (unsigned long) got << " bytes read instead of " << p_size << "in file "
						<< m_filename << " which is " << (unsigned long) GetFileSize() << " bytes long.\n";
		return 0;
	}
	return 1;
}

// 68K 0x1010fad0 Seek__12CNetworkFileFi
// FUNCTION: LEMBALL 0x0047f890
bool NetworkFile::Seek(int p_position)
{
	m_position = (unsigned int) p_position;
	return SetFilePointer(m_handle, p_position, 0, 0) != 0xffffffff;
}

// 68K 0x1010fafe Tell__12CNetworkFileFv
// FUNCTION: LEMBALL 0x0047f8b0
int NetworkFile::Tell()
{
	return (int) m_position;
}

// 68K 0x1010fb28 Close__12CNetworkFileFv
// FUNCTION: LEMBALL 0x0047f8c0
bool NetworkFile::Close()
{
	int closed;

	closed = 1;
	if (m_closed == 0) {
		closed = CloseHandle(m_handle);
		if (closed != 0) {
			m_closed = 1;
		}
	}
	return closed != 0;
}

// 68K 0x1010fb8a GetFileSize__12CNetworkFileFv
// FUNCTION: LEMBALL 0x0047f8f0
unsigned int NetworkFile::GetFileSize()
{
	return ::GetFileSize(m_handle, 0);
}

// 68K 0x1010fbde Lock__12CNetworkFileFUlUl
// FUNCTION: LEMBALL 0x0047f900
bool NetworkFile::Lock(unsigned int p_offset, unsigned int p_length)
{
	return LockFile(m_handle, p_offset, 0, p_length, 0) != 0;
}

// 68K 0x1010fc3c UnLock__12CNetworkFileFUlUl
// FUNCTION: LEMBALL 0x0047f920
bool NetworkFile::UnLock(unsigned int p_offset, unsigned int p_length)
{
	return UnlockFile(m_handle, p_offset, 0, p_length, 0) != 0;
}
