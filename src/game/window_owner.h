#ifndef LEMBALL_GAME_WINDOW_OWNER_H
#define LEMBALL_GAME_WINDOW_OWNER_H

#include "platform/win32.h"

struct GAME_XYPair {
    short m_x;
    short m_y;

    GAME_XYPair &operator=(const GAME_XYPair &other);
};

struct GAME_RootGeometryOwnerChildNode;

// VTABLE: LEMBALL 0x00499478
class GAME_RootGeometryOwner {
public:
    GAME_RootGeometryOwner(void);
    ~GAME_RootGeometryOwner(void);
    void SetWindowOwnerScaleFactor(int nScaleFactor);

    virtual void RefreshWindowOwnerMenuState(void);
    virtual void InitializeWindowOwnerFromRect(short *paRect,
                                                void *pWindowOwner,
                                                void *pContext) = 0;
    virtual void StoreRootGeometrySourceRect(const GAME_XYPair *pRect);
    virtual int QueryRootGeometryState(void *pFirst, void *pSecond);
    virtual void SetWindowOwnerMenuDefinition(const unsigned int *pnMenuId,
                                               void *pMenuDefinition);
    virtual void EnsureRootGeometryDispatchHelper(void);
    virtual void RetainRootGeometryOwner(void);
    virtual void ReleaseRootGeometryOwner(void);
    virtual void CanonicalizeRootGeometrySubrect(void);
    virtual void OffsetRootGeometryPosition(unsigned int uPackedDelta);
    virtual void CanonicalizeRootGeometrySubrectAlt(void);
    virtual void SetRootGeometryScaleFactor(int nScaleFactor);
    virtual void SetRootGeometryRect(const GAME_XYPair *pRect);
    virtual void FinalizeRootGeometryRect(const GAME_XYPair *pPoint);
    virtual void MoveWindowOwnerOrigin(const GAME_XYPair *pPoint) = 0;
    virtual void ReservedWindowOwnerSlot15(void) = 0;
    virtual void ReservedWindowOwnerSlot16(void) = 0;
    virtual void ReservedWindowOwnerSlot17(void) = 0;
    virtual void ReservedWindowOwnerSlot18(void) = 0;
    virtual void ReservedWindowOwnerSlot19(void) = 0;
    virtual void ReservedWindowOwnerSlot20(void) = 0;
    virtual void ReservedWindowOwnerSlot21(void) = 0;
    virtual void ReservedWindowOwnerSlot22(int nValue) = 0;
    virtual void ReservedWindowOwnerSlot23(void);
    virtual int IsCurrentWindowOwnerContext(void);
    virtual unsigned int GetWindowOwnerFlags(void) = 0;
    virtual int GetWindowOwnerState(void);
    virtual void SetWindowOwnerState(int nState);
    virtual void InvalidateWindowOwnerRect(const GAME_XYPair *pExtent) = 0;
    virtual void DestroyWindowOwnerHwnd(void) = 0;
    virtual void AppendQueuedRenderSinkValueNode(void *pValue);
    virtual void RemoveQueuedRenderSinkValueNode(void *pValue);
    virtual void ReservedWindowOwnerSlot32(void);
    virtual void ReservedWindowOwnerSlot33(int nValue);
    virtual void ReservedWindowOwnerSlot34(int nValue);

protected:
    int m_cRetains04;
    GAME_XYPair m_Size08;
    GAME_XYPair m_Position0C;
    GAME_XYPair m_SourceSize10;
    GAME_XYPair m_SourcePosition14;
    GAME_XYPair m_Origin18;
    void *m_pGeometryDispatchHelper1C;
    GAME_RootGeometryOwner *m_pParent20;
    GAME_RootGeometryOwnerChildNode *m_pChildren24;
    GAME_RootGeometryOwnerChildNode *m_pChildrenTail28;
    int m_cChildren2C;
    int m_nWindowOwnerState30;
    int m_fGeometryEnabled34;
    int m_nScaleFactor38;
};

// VTABLE: LEMBALL 0x004993D0
class GAME_WindowOwnerBase : public GAME_RootGeometryOwner {
public:
    GAME_WindowOwnerBase(void);
    ~GAME_WindowOwnerBase(void);

    virtual void RefreshWindowOwnerMenuState(void);
    virtual void InitializeWindowOwnerFromRect(short *paRect,
                                                void *pWindowOwner,
                                                void *pContext);
    virtual void SetWindowOwnerMenuDefinition(const unsigned int *pnMenuId,
                                               void *pMenuDefinition);
    virtual void SetRootGeometryScaleFactor(int nScaleFactor);
    virtual void SetRootGeometryRect(const GAME_XYPair *pRect);
    virtual void FinalizeRootGeometryRect(const GAME_XYPair *pPoint);
    virtual void MoveWindowOwnerOrigin(const GAME_XYPair *pPoint);
    virtual void ReservedWindowOwnerSlot15(void);
    virtual void ReservedWindowOwnerSlot16(void);
    virtual void ReservedWindowOwnerSlot17(void);
    virtual void ReservedWindowOwnerSlot18(void);
    virtual void ReservedWindowOwnerSlot19(void);
    virtual void ReservedWindowOwnerSlot20(void);
    virtual void ReservedWindowOwnerSlot21(void);
    virtual void ReservedWindowOwnerSlot22(int nValue);
    virtual void ReservedWindowOwnerSlot23(void);
    virtual int IsCurrentWindowOwnerContext(void);
    virtual unsigned int GetWindowOwnerFlags(void);
    virtual void InvalidateWindowOwnerRect(const GAME_XYPair *pExtent);
    virtual void DestroyWindowOwnerHwnd(void);
    virtual void ReservedWindowOwnerSlot34(int nValue);
    virtual void PropagateWindowOwnerPosition(const GAME_XYPair *pPoint);
    virtual void CacheCurrentContextWindow(void);
    virtual void ReservedWindowOwnerSlot37(void);
    virtual void ReservedWindowOwnerSlot38(void);
    virtual void ReservedWindowOwnerSlot39(void);
    virtual LRESULT DispatchWindowMessage(UINT uMessage,
                                          WPARAM wParam,
                                          LPARAM lParam);

protected:
    void *m_pMenuDefinition3C;
    unsigned int m_nMenuResourceId40;
    HWND m_hWindow44;
    void *m_pWindowContext48;
};

typedef char GAME_RootGeometryOwner_size_must_be_0x3c[
    sizeof(GAME_RootGeometryOwner) == 0x3c ? 1 : -1];
typedef char GAME_WindowOwnerBase_size_must_be_0x4c[
    sizeof(GAME_WindowOwnerBase) == 0x4c ? 1 : -1];

extern int g_nLiveWindowOwnerBaseCount;
extern void *g_pRootZrleGeometryOwnerRegistry;

int PackEventXYWords(unsigned int uLowWord, int nHighWord);
void LEMBALL_STDCALL PollCursorPositionEvent(void);

#endif
