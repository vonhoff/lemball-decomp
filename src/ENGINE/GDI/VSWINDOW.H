#ifndef LEMBALL_GAME_WINDOW_OWNER_H
#define LEMBALL_GAME_WINDOW_OWNER_H

#include "ENGINE/CORE/WIN32.H"

// SIZE 0x04
struct VS_XYPair {
    short m_x; // 0x00
    short m_y; // 0x02

    VS_XYPair &operator=(const VS_XYPair &other);
};

struct VS_RootGeometryOwnerChildNode;

// VTABLE: LEMBALL 0x00499478
// SIZE 0x3c
class VS_RootGeometryOwner {
public:
    VS_RootGeometryOwner(void);
    ~VS_RootGeometryOwner(void);
    void SetWindowOwnerScaleFactor(int nScaleFactor);

    virtual void RefreshWindowOwnerMenuState(void); // vtable+0x00
    virtual void InitializeWindowOwnerFromRect(short *paRect,
                                                void *pWindowOwner,
                                                const char *pszTitle) = 0; // vtable+0x04
    virtual void StoreRootGeometrySourceRect(const VS_XYPair *pRect); // vtable+0x08
    virtual int QueryRootGeometryState(void *pFirst, void *pSecond); // vtable+0x0c
    virtual void SetWindowOwnerMenuDefinition(const unsigned int *pnMenuId,
                                               void *pMenuDefinition); // vtable+0x10
    virtual void EnsureRootGeometryDispatchHelper(void); // vtable+0x14
    virtual void RetainRootGeometryOwner(void); // vtable+0x18
    virtual void ReleaseRootGeometryOwner(void); // vtable+0x1c
    virtual void CanonicalizeRootGeometrySubrect(void); // vtable+0x20
    virtual void OffsetRootGeometryPosition(unsigned int uPackedDelta); // vtable+0x24
    virtual void CanonicalizeRootGeometrySubrectAlt(void); // vtable+0x28
    virtual void SetRootGeometryScaleFactor(int nScaleFactor); // vtable+0x2c
    virtual void SetRootGeometryRect(const VS_XYPair *pRect); // vtable+0x30
    virtual void FinalizeRootGeometryRect(const VS_XYPair *pPoint); // vtable+0x34
    virtual void MoveWindowOwnerOrigin(const VS_XYPair *pPoint) = 0; // vtable+0x38
    virtual void ReservedWindowOwnerSlot15(void) = 0; // vtable+0x3c
    virtual void ReservedWindowOwnerSlot16(void) = 0; // vtable+0x40
    virtual void ReservedWindowOwnerSlot17(void) = 0; // vtable+0x44
    virtual void ReservedWindowOwnerSlot18(void) = 0; // vtable+0x48
    virtual void ReservedWindowOwnerSlot19(void) = 0; // vtable+0x4c
    virtual void ReservedWindowOwnerSlot20(void) = 0; // vtable+0x50
    virtual void ReservedWindowOwnerSlot21(void) = 0; // vtable+0x54
    virtual void ReservedWindowOwnerSlot22(int nValue) = 0; // vtable+0x58
    virtual void ReservedWindowOwnerSlot23(void); // vtable+0x5c
    virtual int IsCurrentWindowOwnerContext(void); // vtable+0x60
    virtual unsigned int GetWindowOwnerFlags(void) = 0; // vtable+0x64
    virtual int GetWindowOwnerState(void); // vtable+0x68
    virtual void SetWindowOwnerState(int nState); // vtable+0x6c
    virtual void InvalidateWindowOwnerRect(const VS_XYPair *pExtent) = 0; // vtable+0x70
    virtual void DestroyWindowOwnerHwnd(void) = 0; // vtable+0x74
    virtual void AppendQueuedRenderSinkValueNode(void *pValue); // vtable+0x78
    virtual void RemoveQueuedRenderSinkValueNode(void *pValue); // vtable+0x7c
    virtual void ReservedWindowOwnerSlot32(void); // vtable+0x80
    virtual void ReservedWindowOwnerSlot33(int nValue); // vtable+0x84
    virtual void ReservedWindowOwnerSlot34(int nValue); // vtable+0x88

protected:
    int m_cRetains04;                                  // 0x04
    VS_XYPair m_Size08;                                // 0x08
    VS_XYPair m_Position0C;                            // 0x0c
    VS_XYPair m_SourceSize10;                          // 0x10
    VS_XYPair m_SourcePosition14;                      // 0x14
    VS_XYPair m_Origin18;                              // 0x18
    void *m_pGeometryDispatchHelper1C;                 // 0x1c
    VS_RootGeometryOwner *m_pParent20;                 // 0x20
    VS_RootGeometryOwnerChildNode *m_pChildren24;      // 0x24
    VS_RootGeometryOwnerChildNode *m_pChildrenTail28;  // 0x28
    int m_cChildren2C;                                 // 0x2c
    int m_nWindowOwnerState30;                         // 0x30
    int m_fGeometryEnabled34;                          // 0x34
    int m_nScaleFactor38;                              // 0x38
};

// VTABLE: LEMBALL 0x004993D0
// SIZE 0x4c
class VS_WindowOwnerBase : public VS_RootGeometryOwner {
public:
    VS_WindowOwnerBase(void);
    ~VS_WindowOwnerBase(void);

    virtual void RefreshWindowOwnerMenuState(void);
    virtual void InitializeWindowOwnerFromRect(short *paRect,
                                                void *pWindowOwner,
                                                const char *pszTitle);
    virtual void SetWindowOwnerMenuDefinition(const unsigned int *pnMenuId,
                                               void *pMenuDefinition);
    virtual void SetRootGeometryScaleFactor(int nScaleFactor);
    virtual void SetRootGeometryRect(const VS_XYPair *pRect);
    virtual void FinalizeRootGeometryRect(const VS_XYPair *pPoint);
    virtual void MoveWindowOwnerOrigin(const VS_XYPair *pPoint);
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
    virtual void InvalidateWindowOwnerRect(const VS_XYPair *pExtent);
    virtual void DestroyWindowOwnerHwnd(void);
    virtual void ReservedWindowOwnerSlot34(int nValue);
    virtual void PropagateWindowOwnerPosition(const VS_XYPair *pPoint);
    virtual void CacheCurrentContextWindow(void);
    virtual void ReservedWindowOwnerSlot37(void);
    virtual void ReservedWindowOwnerSlot38(void);
    virtual void ReservedWindowOwnerSlot39(void);
    virtual LRESULT DispatchWindowMessage(UINT uMessage,
                                          WPARAM wParam,
                                          LPARAM lParam);

protected:
    void *m_pMenuDefinition3C;       // 0x3c
    unsigned int m_nMenuResourceId40; // 0x40
    HWND m_hWindow44;                 // 0x44
    void *m_pWindowContext48;         // 0x48
};

extern int g_nLiveWindowOwnerBaseCount;
extern void *g_pRootZrleGeometryOwnerRegistry;

int PackEventXYWords(unsigned int uLowWord, int nHighWord);
void LEMBALL_STDCALL PollCursorPositionEvent(void);
void LEMBALL_FASTCALL UpdateWindowOwnerRootGeometrySourceRect(
    void *pOwner, int nUnused, const VS_XYPair *pRect);
void LEMBALL_FASTCALL RefreshWindowOwnerRenderScale(
    void *pOwner, int nUnused, int nPreviousScaleFactor);
void LEMBALL_FASTCALL ReleaseWindowOwnerRenderResources(void *pOwner);
void LEMBALL_FASTCALL CanonicalizeWindowOwnerRenderSubrect(void *pOwner);
void LEMBALL_FASTCALL PopulateWindowOwnerRenderAnchor(void *pOwner);
void LEMBALL_FASTCALL SetWindowOwnerRenderAnchorFromPoint(
    void *pOwner, int nUnused, const VS_XYPair *pPoint);
void LEMBALL_FASTCALL ForwardNestedGeometryRectUpdate(
    void *pOwner, int nUnused, short *paRect);
LRESULT LEMBALL_FASTCALL DispatchWindowOwnerRenderContextMessage(
    void *pOwner, int nUnused, UINT uMessage, WPARAM wParam, LPARAM lParam);
void LEMBALL_FASTCALL PresentWindowOwnerRenderContext(void *pOwner);

#endif
