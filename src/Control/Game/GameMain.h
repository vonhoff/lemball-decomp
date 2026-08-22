#ifndef LEMBALL_CONTROL_GAME_GAMEMAIN_H
#define LEMBALL_CONTROL_GAME_GAMEMAIN_H

#include "../../Common.h"

PreInit* VsPreInit(PreInit* p_preInit);
void SetGameDefaults();
void DisplayHelp();
int Vsmain(int p_argc, char** p_argv);
unsigned char DoCommandLine(int p_argc, char** p_argv);
#endif
