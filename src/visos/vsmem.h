#ifndef LEMBALL_VSMEM_H
#define LEMBALL_VSMEM_H

int InitializeMasterMainRamArena(void);
void ShutdownMasterMainRamArena(void);
void *AllocateVSMemBlock(unsigned int cbBlock);
void FreeVSMemBlock(void *pvBlock);
long CalculateMemoryArenaAvailableBytes(void);

#endif
