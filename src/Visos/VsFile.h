#ifndef LEMBALL_SCAFFOLD_VISOS_VSFILE_H
#define LEMBALL_SCAFFOLD_VISOS_VSFILE_H

#include "../Common.h"

char* GetCdDir(const char* p_requiredFile);
_Filet* VsOpen(char* p_path, char* p_mode);
int VsClose(_Filet* p_file);
unsigned long VsRead(_Filet* p_file, void* p_buffer, unsigned long p_size);
unsigned long VsWrite(_Filet* p_file, void* p_buffer, unsigned long p_size);
long VsSeek(_Filet* p_file, long p_offset, int p_origin);
long VsTell(_Filet* p_file);
long VsGetFileSize(_Filet* p_file);
#endif
