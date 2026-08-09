#ifndef LEMBALL_C2D_RENDER_H
#define LEMBALL_C2D_RENDER_H

struct CVSPoint;

struct CViewData {
	unsigned short m_nDirection00;
	unsigned short m_nReserved02;
	int m_nX04;
	int m_nY08;
	unsigned char m_abReserved0C[0x0c];
	int m_nState18;
	short m_nVariant1C;
	unsigned short m_nReserved1E;
	int m_nFrame20;
	void* m_pFrameSelector24;
	int m_nObjectType28;
	unsigned short m_nObjectNumber2C;
	unsigned char m_abReserved2E[0x1e];
};

struct CVSRect;

class C2D {
public:
	unsigned char m_abReserved0000[0x64];
	int m_anPlayerRemaps0064[4];
	unsigned char m_abReserved0074[0x898];
	int m_nViewRotation090C;
	unsigned char m_abReserved0910[4];
	void* m_pLevel0914;
	int m_nCameraOriginX0918;
	int m_nCameraOriginY091C;
	unsigned char m_abReserved0920[0x3c];
	CViewData* m_pViewData095C;
	unsigned char m_abReserved0960[4];
	unsigned short m_nViewDataCount0964;
	unsigned short m_nReserved0966;
	int m_nLemmingRemap0968;
	void* m_pLevelMode096C;
	void* m_pRenderQueue0970;
	unsigned char m_abReserved0974[0x1c];
	unsigned char m_RenderQueueClient0990[0xb0];
	void* m_pAnimsManager0A40;
	unsigned char m_abReserved0A44[0x1744];
	int m_nStaticEntryCount2188;
	unsigned char m_StaticEntry218C[0x68];

	void DrawBullet(CViewData& ViewData, int nUnused);
	void DrawAmmo(CViewData& ViewData, int nUnused);
	void DrawBalloonPost(CViewData& ViewData, int nPlayer);
	void DrawTimeBonus(CViewData& ViewData);
	void DrawKey(CViewData& ViewData, int nPlayer);
	void DrawBonus(CViewData& ViewData);
	void DrawHand(CViewData& ViewData);
	void DrawLemmingExternal(CViewData& ViewData, int nUseRemap);
	void DrawLemmingOnBalloon(CViewData& ViewData, int nPlayer, unsigned char fUseRemap);
	void DrawMover(CViewData& ViewData);
	void DrawPaintGun(CViewData& ViewData);
	void DrawLaser(CViewData& ViewData);
	void DrawDuplicator(CViewData& ViewData);
	void DrawSheep(CViewData& ViewData, int nUnused);
	void DrawSwitch(CViewData& ViewData);
	void DrawDemo(void);
	int ProcessMsg(short* pMessage);
	void DrawTime(void);
	void DrawScore(void);
	void NewPauseWindow(int nMessage);
	void TriggerPause(int fPause);
	void UnRegisterRemaps(void);
	void OnInside(unsigned short* pPoint);
	void OnButtonUp(void* pButtonFlags);
	void OnButtonDown(const CVSPoint& Point, int nButtonFlags);
	void Restart(void);
	void SendCursorMsg(void);
	CVSRect* BuildViewportRectSnapshot(void);
	void DrawTrampoline(CViewData& ViewData);
	void DrawLaserFire(CViewData& ViewData);
	void DrawCrate(CViewData& ViewData, int nUnused);
	void DrawBall(CViewData& ViewData);
	void DrawMine(CViewData& ViewData);
	void DrawFlag(CViewData& ViewData, int nObjectType);
	void DrawGrenade(CViewData& ViewData);
	void DrawRocket(CViewData& ViewData);
	void DrawBoobyTrap(CViewData& ViewData);
	void DrawCrusher(CViewData& ViewData);
	void DrawBurner(CViewData& ViewData);
	void DrawTeleporter(CViewData& ViewData);
	void DrawBalloon(CViewData& ViewData);
	void DrawExit(CViewData& ViewData);
	void ResetPrimitives(void);
	void DrawZBuff_Sprite(int nViewDataIndex, unsigned short nValue);
	void DrawLemmingJump(unsigned short* pViewData, int nFrameIndex);
	void DrawLemmingFall(unsigned short* pViewData, int nUnused);
	void DrawLemmingLanding(unsigned short* pViewData, int nFrameIndex);
	void DrawLemmingOnConveyor(CViewData& ViewData, int nUseRemap);
	void DrawHandHeightSampledOverlayPair(CViewData& ViewData, int nPlayer, int nUseRemap);
	void DrawItemHandBoundaryPair(CViewData& ViewData, int nPlayer);
#ifdef LEMBALL_C2D_LEMMING_FLY
	int LemmingFly(CViewData& ViewData, int& nFrame);
#endif
};

typedef char CViewDataSizeCheck[(sizeof(CViewData) == 0x4c) ? 1 : -1];

#endif
