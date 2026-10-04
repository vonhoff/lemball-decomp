#ifndef LEMBALL_VISOS_FOUNDATION_VSFILE_H
#define LEMBALL_VISOS_FOUNDATION_VSFILE_H

struct _Filet;
int vsClose(_Filet* p_file);
_Filet* vsOpen(char* p_path, char* p_mode);
unsigned long vsRead(_Filet* p_file, void* p_buffer, unsigned long p_size);
unsigned long vsWrite(_Filet* p_file, void* p_buffer, unsigned long p_size);
long vsSeek(_Filet* p_file, long p_offset, int p_origin);
long vsTell(_Filet* p_file);
long vsGetFileSize(_Filet* p_file);
#endif
