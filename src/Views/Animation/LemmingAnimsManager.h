#ifndef LEMBALL_VIEWS_ANIMATION_LEMMINGANIMSMANAGER_H
#define LEMBALL_VIEWS_ANIMATION_LEMMINGANIMSMANAGER_H

#include "../../Common.h"
#include "../../Visos/Animation/AnimsManager.h" // complete type
#include "../../Visos/Graphics/DrawingMark.h"  // complete type
#include "../../Visos/Graphics/Primitive.h"    // complete type

// SIZE 0xb4
// VTABLE: LEMBALL 0x00496ce0 eight-entry primary
// VTABLE: LEMBALL 0x00496cdc load-progress interface at +0x70
class LemmingAnimsManager : public AnimsManager {
public:
	LemmingAnimsManager(Gdi* p_arg0, Main2DDisplay* p_arg1, Ai* p_arg2);
	void Draw();
	void DrawAnim(short p_x,
				  short p_y,
				  unsigned long p_resourceId,
				  unsigned long p_animIndex,
				  unsigned long p_time,
				  Remap* p_remap);
	void Load(int p_groundStyle);
	void LoadAnimation(unsigned long p_firstResourceId, unsigned long p_lastResourceId, int p_animType);
	void LoadAnimation(unsigned long p_resourceId, int p_animType);
	void LoadMainRammed();
	void LoadVrammed();
	void SetupStyleSensitive();
	void UnLoadAnimation(unsigned long p_firstResourceId, unsigned long p_lastResourceId);
	void UnLoadAnimation(unsigned long p_resourceId);
	void Unload();
	void UpdateNonCacheLoad();
	~LemmingAnimsManager();

private:
	void* m_loadVtable;           // 0x70
	void** m_animFrames;          // 0x74
	CdLoadAnim* m_loadAnim;       // 0x78
	Main2DDisplay* m_display;     // 0x7c
	Gdi* m_gdi;                   // 0x80
	unsigned int m_loaded;        // 0x84
	DrawingMark m_primitive;      // 0x88
	unsigned int m_nonCacheState; // 0x8c
	unsigned int m_loadProgress;  // 0x90
	unsigned int m_countingLoads; // 0x94
	Ai* m_ai;                     // 0x98
	unsigned int m_groundStyle;   // 0xa8
	unsigned int m_reservedac;    // 0xac
	short m_drawOffsetX;          // 0xb0
	short m_drawOffsetY;          // 0xb2
};

#endif
