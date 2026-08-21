#ifndef LEMBALL_GAMEMENUACTIONFRAMEPOINTSINK_H
#define LEMBALL_GAMEMENUACTIONFRAMEPOINTSINK_H

// Menu action-frame render sink. Consolidated from per-TU duplicate definitions
// (MENUSEL.CPP, MENUSELSINK.CPP). NOTE: GameMenuActionFramePointSinkVtableData is NOT
// consolidated — it differs between TUs (member-pointer vs free-fn-pointer Delete slot).

struct GameMenuActionFramePointSink {
	void** m_pVtable00;
	short m_nReserved04;
	short m_nReserved06;
	unsigned char m_abReserved08[0x10];
	int m_iFrame18;
	int* m_pResource1C;

	void* Delete(unsigned int fDelete);
	void BindToContext(void* pContext);
	void DispatchToRenderObject(void* pContext);
};

#endif
