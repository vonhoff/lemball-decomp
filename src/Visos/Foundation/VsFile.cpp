#include "VsFile.h"

// 68K 0x1010c30e GetCDDir__FPCc
// STUB: LEMBALL 0x0045eda0
char* GetCdDir(const char* p_requiredFile)
{
	return 0;
}

// 68K 0x102116c8 vsOpen__FPcPc
// STUB: LEMBALL 0x00462ee0
_Filet* VsOpen(char* p_path, char* p_mode)
{
	return 0;
}

// 68K 0x102116ee vsClose__FP6_Filet
// STUB: LEMBALL 0x00462f20
int VsClose(_Filet* p_file)
{
	return 0;
}

// 68K 0x10211716 vsRead__FP6_FiletPvUl
// STUB: LEMBALL 0x00462f30
unsigned long VsRead(_Filet* p_file, void* p_buffer, unsigned long p_size)
{
	return 0;
}

// 68K 0x1021174c vsWrite__FP6_FiletPvUl
// STUB: LEMBALL 0x00462f50
unsigned long VsWrite(_Filet* p_file, void* p_buffer, unsigned long p_size)
{
	return 0;
}

// 68K 0x10211796 vsSeek__FP6_Filetli
// STUB: LEMBALL 0x00462f80
long VsSeek(_Filet* p_file, long p_offset, int p_origin)
{
	return 0;
}

// 68K 0x102117d0 vsTell__FP6_Filet
// STUB: LEMBALL 0x00462fb0
long VsTell(_Filet* p_file)
{
	return 0;
}

// 68K 0x102117f6 vsGetFileSize__FP6_Filet
// STUB: LEMBALL 0x00462fc0
long VsGetFileSize(_Filet* p_file)
{
	return 0;
}
