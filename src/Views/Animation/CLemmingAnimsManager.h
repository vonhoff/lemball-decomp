#ifndef LEMBALL_VIEWS_ANIMATION_CLEMMINGANIMSMANAGER_H
#define LEMBALL_VIEWS_ANIMATION_CLEMMINGANIMSMANAGER_H

#include "../../Visos/Animation/CAnimsManager.h"
#include "../../Visos/Graphics/CDrawingMark.h"
#include "../../Visos/Graphics/CPrimitive.h"
#include "../Sound/CLoadUpdate.h"

class CAI;
class CCDLoadAnim;
class CFrames;
class CGDI;
class CMain2DDisplay;
class CRemap;
class CResFONT;
extern unsigned int g_groundBlox1ResourceId;
extern unsigned int g_groundBlox2ResourceId;
extern unsigned int g_groundBlox3ResourceId;
extern unsigned int g_groundBlox4ResourceId;
extern unsigned int g_groundBlox5ResourceId;
extern unsigned int g_groundBlox6ResourceId;
extern unsigned int g_groundBlox7ResourceId;

// SIZE 0xb4
// VTABLE: LEMBALL 0x00496cdc CLoadUpdate
// VTABLE: LEMBALL 0x00496ce0 CAnimsManager
class CLemmingAnimsManager : public CAnimsManager, public CLoadUpdate {
public:
	CLemmingAnimsManager(CGDI* p_gdi, CMain2DDisplay* p_display, CAI* p_ai);
	void Draw();
	void DrawAnimOnGdi(CGDI* p_gdi,
					   short p_x,
					   short p_y,
					   unsigned long p_resourceId,
					   unsigned long p_animIndex,
					   CRemap* p_remap);
	void DrawAnim(short p_x,
				  short p_y,
				  unsigned long p_resourceId,
				  unsigned long p_animIndex,
				  unsigned long p_time,
				  CRemap* p_remap);
	void Load(int p_groundStyle);
	void LoadAnimation(unsigned long p_firstResourceId, unsigned long p_lastResourceId, int p_animType);
	void LoadAnimation(unsigned long p_resourceId, int p_animType);
	void LoadMainRammed();
	void LoadVrammed();
	void SetupStyleSensitive();
	void UnLoadAnimation(unsigned long p_firstResourceId, unsigned long p_lastResourceId);
	void UnLoadAnimation(unsigned long p_resourceId);
	void Unload();
	virtual void UpdateNonCacheLoad(); // vtable+0x00
	~CLemmingAnimsManager();
	friend class C2D;

private:
	CFrames** m_animFrames;       // 0x74
	CCDLoadAnim* m_loadAnim;      // 0x78
	CMain2DDisplay* m_display;    // 0x7c
	CGDI* m_gdi;                  // 0x80
	unsigned int m_loaded;        // 0x84
	CDrawingMark m_primitive;     // 0x88
	int m_nonCacheState;          // 0x8c
	int m_loadProgress;           // 0x90
	unsigned int m_countingLoads; // 0x94
	CAI* m_ai;                    // 0x98
	CResFONT* m_interfaceFonts[3]; // 0x9c
	unsigned int m_groundStyle;    // 0xa8
	unsigned int m_drawFlags;      // 0xac
	short m_drawOffsetX;           // 0xb0
	short m_drawOffsetY;           // 0xb2
};

#endif
