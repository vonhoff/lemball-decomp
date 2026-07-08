#include "../game.h"

// FUNCTION: LEMBALL 0x00410D00
void InitializeLevelGameMode(void *pLevelGameMode) {
    (void)pLevelGameMode;
}

// FUNCTION: LEMBALL 0x004479E0
void *ConstructMainGameVariantResourceBundle(void *pBundle, void *pPrimaryContext, unsigned short nVariantMode) {
    (void)pPrimaryContext;
    (void)nVariantMode;
    return pBundle;
}
