#ifndef LEMBALL_SCAFFOLD_VISOS_VSINIT_H
#define LEMBALL_SCAFFOLD_VISOS_VSINIT_H

#include "../Common.h"

bool StrmInit();
bool StrmQuit();
void InpInit();
bool InpQuit();
void InitSubSystems();
void InitQuitSubSystems();
bool InitCheckOptions(char* p_arg0);
char* InitCmdLine(char* p_arg0);
int InitMain(char* p_arg0);
bool StatInit();
bool StatQuit();
bool ResInit();
bool ResQuit();
bool TimeInit();
bool TimeQuit();
bool GdiInit();
bool GdiQuit();
bool MemInit();
bool MemQuit();
bool VsFNetInit();
bool VsFNetQuit();
bool VsNetInit();
bool VsNetQuit();
void InitPreInit();
bool DbgInit();
bool DbgQuit(unsigned char p_force);
#endif
