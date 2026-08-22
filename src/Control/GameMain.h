#ifndef LEMBALL_SCAFFOLD_CONTROL_GAMEMAIN_H
#define LEMBALL_SCAFFOLD_CONTROL_GAMEMAIN_H

#include "../Common.h"

PreInit* VsPreInit(PreInit* p_preInit);
void SetGameDefaults();
void DisplayHelp();
int Vsmain(int p_argc, char** p_argv);
unsigned char DoCommandLine(int p_argc, char** p_argv);
#endif
