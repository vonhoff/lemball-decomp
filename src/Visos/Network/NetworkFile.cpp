#include "NetworkFile.h"

// 68K 0x10107856 GetLastError__12CNetworkFileFv
// STUB: LEMBALL 0x0047b8d0
int NetworkFile::GetLastError()
{
	return 0;
}

// 68K 0x1010788a GetFilename__12CNetworkFileFv
// STUB: LEMBALL 0x0047b8e0
char* NetworkFile::GetFilename()
{
	return 0;
}

// 68K 0x1010f5d0 Open__12CNetworkFileFPCcUcUc
// STUB: LEMBALL 0x0047f5b0
bool NetworkFile::Open(const char* p_filename, unsigned char p_mode, unsigned char p_create)
{
	return 0;
}

// 68K 0x1010f67e Create__12CNetworkFileFPCcUc
// STUB: LEMBALL 0x0047f640
bool NetworkFile::Create(const char* p_filename, unsigned char p_mode)
{
	return 0;
}

// 68K 0x1010f6c2 Delete__12CNetworkFileFv
// STUB: LEMBALL 0x0047f660
bool NetworkFile::Delete()
{
	return 0;
}

// 68K 0x1010f70a Delete__12CNetworkFileFPCc
// STUB: LEMBALL 0x0047f670
bool NetworkFile::Delete(const char* p_filename)
{
	return 0;
}

// 68K 0x1010f77e CheckExists__12CNetworkFileFPCc
// STUB: LEMBALL 0x0047f680
bool NetworkFile::CheckExists(const char* p_filename)
{
	return 0;
}

// 68K 0x1010f7e4 Write__12CNetworkFileFPCUci
// STUB: LEMBALL 0x0047f6c0
bool NetworkFile::Write(const unsigned char* p_data, int p_size)
{
	// STRING: LEMBALL 0x004a41f8 "Write error: "
	// STRING: LEMBALL 0x004a421c " bytes written instead of "
	// STRING: LEMBALL 0x004a420c "Write error: "
	return 0;
}

// 68K 0x1010f94c Read__12CNetworkFileFPUci
// STUB: LEMBALL 0x0047f780
bool NetworkFile::Read(unsigned char* p_data, int p_size)
{
	// STRING: LEMBALL 0x004a423c "Read error: "
	// STRING: LEMBALL 0x004a4260 " bytes read instead of "
	// STRING: LEMBALL 0x004a4250 "Read error: "
	// STRING: LEMBALL 0x004a4294 " bytes long.\n"
	// STRING: LEMBALL 0x004a4288 " which is "
	// STRING: LEMBALL 0x004a427c "in file "
	return 0;
}

// 68K 0x1010fad0 Seek__12CNetworkFileFi
// STUB: LEMBALL 0x0047f890
bool NetworkFile::Seek(int p_position)
{
	return 0;
}

// 68K 0x1010fafe Tell__12CNetworkFileFv
// STUB: LEMBALL 0x0047f8b0
int NetworkFile::Tell()
{
	return 0;
}

// 68K 0x1010fb28 Close__12CNetworkFileFv
// STUB: LEMBALL 0x0047f8c0
bool NetworkFile::Close()
{
	return 0;
}

// 68K 0x1010fb8a GetFileSize__12CNetworkFileFv
// STUB: LEMBALL 0x0047f8f0
unsigned int NetworkFile::GetFileSize()
{
	return 0;
}

// 68K 0x1010fbde Lock__12CNetworkFileFUlUl
// STUB: LEMBALL 0x0047f900
bool NetworkFile::Lock(unsigned int p_offset, unsigned int p_length)
{
	return 0;
}

// 68K 0x1010fc3c UnLock__12CNetworkFileFUlUl
// STUB: LEMBALL 0x0047f920
bool NetworkFile::UnLock(unsigned int p_offset, unsigned int p_length)
{
	return 0;
}
