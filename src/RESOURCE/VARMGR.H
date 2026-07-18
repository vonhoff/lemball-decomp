#ifndef LEMBALL_GAME_VARIANT_RESOURCE_MANAGER_H
#define LEMBALL_GAME_VARIANT_RESOURCE_MANAGER_H

class GameVariantResourceEntryManager {
public:
    void PlayVariantResourceEffect(int nEffectSlot);
    void PlayVariantResourceEffectThunk(int nEffectSlot);
    void SetVariantResourceEffectsEnabledThunk(int fEnabled);
    void SetVariantResourceEffectsEnabled(int fEnabled);
    void SetVariantResourceMusicEnabled(int fEnabled);
    void ClearActiveVariantResourceEntries(void);
    void PopulateVariantResourceEntriesForFlagMask(unsigned short nMask);
    int CountVariantResourceEntriesWithFlagMask(unsigned short nMask);
    void SwitchVariantResourceEntryMode(unsigned short nMask, void *pBundle);
};

#endif
