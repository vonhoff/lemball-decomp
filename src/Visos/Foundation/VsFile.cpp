#include "VsFile.h"

extern "C"
{
	_Filet* fopen(const char* p_path, const char* p_mode);
	int fclose(_Filet* p_file);
	size_t fread(void* p_buffer, size_t p_size, size_t p_count, _Filet* p_file);
	size_t fwrite(const void* p_buffer, size_t p_size, size_t p_count, _Filet* p_file);
	int fflush(_Filet* p_file);
	int fseek(_Filet* p_file, long p_offset, int p_origin);
	long ftell(_Filet* p_file);
}

// 68K 0x102116c8 vsOpen__FPcPc
// FUNCTION: LEMBALL 0x00462ee0
_Filet* VsOpen(char* p_path, char* p_mode)
{
	return fopen(p_path, p_mode);
}

// 68K 0x102116ee vsClose__FP6_Filet
// FUNCTION: LEMBALL 0x00462f20
int VsClose(_Filet* p_file)
{
	return fclose(p_file);
}

// 68K 0x10211716 vsRead__FP6_FiletPvUl
// FUNCTION: LEMBALL 0x00462f30
unsigned long VsRead(_Filet* p_file, void* p_buffer, unsigned long p_size)
{
	return fread(p_buffer, 1, p_size, p_file);
}

// 68K 0x1021174c vsWrite__FP6_FiletPvUl
// FUNCTION: LEMBALL 0x00462f50
unsigned long VsWrite(_Filet* p_file, void* p_buffer, unsigned long p_size)
{
	unsigned long written = fwrite(p_buffer, 1, p_size, p_file);
	fflush(p_file);
	return written;
}

// 68K 0x10211796 vsSeek__FP6_Filetli
// FUNCTION: LEMBALL 0x00462f80
long VsSeek(_Filet* p_file, long p_offset, int p_origin)
{
	fseek(p_file, p_offset, p_origin);
	return ftell(p_file);
}

// 68K 0x102117d0 vsTell__FP6_Filet
// FUNCTION: LEMBALL 0x00462fb0
long VsTell(_Filet* p_file)
{
	return ftell(p_file);
}

// 68K 0x102117f6 vsGetFileSize__FP6_Filet
// FUNCTION: LEMBALL 0x00462fc0
long VsGetFileSize(_Filet* p_file)
{
	long offset = VsTell(p_file);
	long size = VsSeek(p_file, 0, 2);
	VsSeek(p_file, offset, 0);
	return size;
}
