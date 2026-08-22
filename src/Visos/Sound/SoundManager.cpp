#include "SoundManager.h"

// 68K 0x10218192 __ct__13CSoundManagerFUcUcUciP4CWnd
// STUB: LEMBALL 0x0045af80
SoundManager::SoundManager(unsigned char p_arg0, unsigned char p_arg1, unsigned char p_arg2, int p_arg3, Wnd* p_arg4)
{
}

// 68K 0x10218368 __dt__13CSoundManagerFv
// STUB: LEMBALL 0x0045b110
SoundManager::~SoundManager()
{
}

// 68K 0x1021846a SetResId__13CSoundManagerFUl
// STUB: LEMBALL 0x0045b190
void SoundManager::SetResId(unsigned long p_resourceId)
{
}

// 68K 0x102184d0 Background__13CSoundManagerFv
// STUB: LEMBALL 0x0045b1d0
void SoundManager::Background()
{
}

// 68K 0x10218532 Foreground__13CSoundManagerFv
// STUB: LEMBALL 0x0045b210
void SoundManager::Foreground()
{
}

// 68K 0x10218602 PrepareMusic__13CSoundManagerFUlUc
// STUB: LEMBALL 0x0045b2c0
unsigned long SoundManager::PrepareMusic(unsigned long p_resourceId, unsigned char p_flags)
{
	return 0;
}

// 68K 0x102186b8 PlayMusic__13CSoundManagerFUl
// STUB: LEMBALL 0x0045b2e0
void SoundManager::PlayMusic(unsigned long p_handle)
{
}

// 68K 0x10218680 StreamOut__13CSoundManagerFR10CVSOStream
// STUB: LEMBALL 0x0045b320
VsOStream& SoundManager::StreamOut(VsOStream& p_stream)
{
	return *(VsOStream*) 0;
}

// 68K 0x10218714 ProcessMusic__13CSoundManagerFUl
// STUB: LEMBALL 0x0045b330
void SoundManager::ProcessMusic(unsigned long p_handle)
{
}

// 68K 0x1021875e StopMusic__13CSoundManagerFUl
// STUB: LEMBALL 0x0045b370
void SoundManager::StopMusic(unsigned long p_handle)
{
}

// 68K 0x102187b4 FreeMusic__13CSoundManagerFUl
// STUB: LEMBALL 0x0045b3b0
void SoundManager::FreeMusic(unsigned long p_handle)
{
}

// 68K 0x10218810 PrepareEffect__13CSoundManagerFUl
// STUB: LEMBALL 0x0045b3f0
unsigned long SoundManager::PrepareEffect(unsigned long p_resourceId)
{
	return 0;
}

// 68K 0x102188c0 PlayEffect__13CSoundManagerFUl
// STUB: LEMBALL 0x0045b460
void SoundManager::PlayEffect(unsigned long p_effectId)
{
}

// 68K 0x10218924 PlayEffect__13CSoundManagerFUlUc
// STUB: LEMBALL 0x0045b490
void SoundManager::PlayEffect(unsigned long p_effectId, unsigned char p_channel)
{
}

// 68K 0x10218978 FreeEffect__13CSoundManagerFUl
// STUB: LEMBALL 0x0045b4f0
void SoundManager::FreeEffect(unsigned long p_effectId)
{
}

// 68K 0x102189c2 SetVolumes__13CSoundManagerFii
// STUB: LEMBALL 0x0045b510
void SoundManager::SetVolumes(int p_effectVolume, int p_musicVolume)
{
}

// 68K 0x10218a5e GetEffectVolume__13CSoundManagerFv
// STUB: LEMBALL 0x0045b560
unsigned char SoundManager::GetEffectVolume()
{
	return 0;
}

// 68K 0x10218aaa SetEffectsWnd__13CSoundManagerFP4CWnd
// STUB: LEMBALL 0x0045b580
void SoundManager::SetEffectsWnd(Wnd* p_window)
{
}

// 68K 0x10218af2 SetMusicWnd__13CSoundManagerFP4CWnd
// STUB: LEMBALL 0x0045b5a0
void SoundManager::SetMusicWnd(Wnd* p_window)
{
}

// 68K 0x10218b40 UseMusicCD__13CSoundManagerFUc
// STUB: LEMBALL 0x0045b5f0
void SoundManager::UseMusicCd(unsigned char p_enabled)
{
}
