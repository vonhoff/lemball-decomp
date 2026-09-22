#include "VsFile.h"

#include <stddef.h>

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

// FUNCTION: LEMBALL 0x00462ee0
_Filet* vsOpen(char* p_path, char* p_mode)
{
	return fopen(p_path, p_mode);
}

// FUNCTION: LEMBALL 0x00462f20
int vsClose(_Filet* p_file)
{
	return fclose(p_file);
}

// FUNCTION: LEMBALL 0x00462f30
unsigned long vsRead(_Filet* p_file, void* p_buffer, unsigned long p_size)
{
	return fread(p_buffer, 1, p_size, p_file);
}

// FUNCTION: LEMBALL 0x00462f50
unsigned long vsWrite(_Filet* p_file, void* p_buffer, unsigned long p_size)
{
	unsigned long written = fwrite(p_buffer, 1, p_size, p_file);
	fflush(p_file);
	return written;
}

// FUNCTION: LEMBALL 0x00462f80
long vsSeek(_Filet* p_file, long p_offset, int p_origin)
{
	fseek(p_file, p_offset, p_origin);
	return ftell(p_file);
}

// FUNCTION: LEMBALL 0x00462fb0
long vsTell(_Filet* p_file)
{
	return ftell(p_file);
}

// FUNCTION: LEMBALL 0x00462fc0
long vsGetFileSize(_Filet* p_file)
{
	long offset = vsTell(p_file);
	long size = vsSeek(p_file, 0, 2);
	vsSeek(p_file, offset, 0);
	return size;
}
