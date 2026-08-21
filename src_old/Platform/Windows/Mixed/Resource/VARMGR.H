#ifndef LEMBALL_GAME_VARIANT_RESOURCE_MANAGER_H
#define LEMBALL_GAME_VARIANT_RESOURCE_MANAGER_H

class GameVariantResourceEntryManager {
public:
	void PlayVariantResourceEffect(int nEffectSlot);
	void PlayVariantResourceEffectThunk(int nEffectSlot);
	void SetVariantResourceEffectsEnabledThunk(int fEnabled);
	void SetVariantResourceEffectsEnabled(int fEnabled);
	void SetVariantResourceMusicEnabled(int fEnabled);
	void SetAudioMusicAndEffectVolumeByteThunk(unsigned char nVolume);
	void SetVariantResourceEffectMasterVolumeThunk(unsigned char nVolume);
	void ClearActiveVariantResourceEntries(void);
	void PopulateVariantResourceEntriesForFlagMask(unsigned short nMask);
	int CountVariantResourceEntriesWithFlagMask(unsigned short nMask);
	void SwitchVariantResourceEntryModeThunk(unsigned short nMask, void* pBundle);
	void SwitchVariantResourceEntryMode(unsigned short nMask, void* pBundle);
};

#endif
