#ifndef LEMBALL_VISOS_FOUNDATION_VSFILE_H
#define LEMBALL_VISOS_FOUNDATION_VSFILE_H

#include "../../Common.h"

#include "../../Common.h"
int VsClose(_Filet* p_file);
_Filet* VsOpen(char* p_path, char* p_mode);
unsigned long VsRead(_Filet* p_file, void* p_buffer, unsigned long p_size);
unsigned long VsWrite(_Filet* p_file, void* p_buffer, unsigned long p_size);
long VsSeek(_Filet* p_file, long p_offset, int p_origin);
long VsTell(_Filet* p_file);
long VsGetFileSize(_Filet* p_file);
#endif
