#ifndef LEMBALL_GAME_CURSOR_RENDER_CLIENT_H
#define LEMBALL_GAME_CURSOR_RENDER_CLIENT_H

#include "ENGINE/CORE/VSMATH.H"

struct GameCursorRenderClientInterface {
    virtual void Reserved00(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual void Reserved08(void) = 0;
    virtual int PrepareForWindowOwner(void *pWindowOwner) = 0;
    virtual void HideNativeCursor(void) = 0;
    virtual void ShowNativeCursor(void) = 0;
    virtual void InitializeCursorPosition(void) = 0;
};

struct GameCursorRenderClient : public GameCursorRenderClientInterface {
    char m_abReserved04[0x0c];
    VsMathPoint2D m_Position10;
    char m_abReserved14[4];
    int m_fActive18;
    void *m_pRenderEntry1C;
    void *m_pRenderResource20;
    int m_nResourceEntry24;
    int m_nReserved28;
    int m_fPrimaryVisible2C;
    int m_fSecondaryVisible30;
    int m_nReserved34;
    int m_fMotionPending38;
    int m_fIdleRefreshPending3C;
    char m_abReserved40[0x10];
    int m_nMaximumVelocity50;
    VsMathFixed12Vector2D m_FixedPosition54;
    VsMathFixed12Vector2D m_Velocity5C;
    VsMathFixed12Vector2D m_Acceleration64;
    unsigned long m_dwLastAccelerationXTick6C;
    unsigned long m_dwLastAccelerationYTick70;
    VsMathPoint2D m_BoundsSize74;
    VsMathPoint2D m_BoundsOrigin78;

    void TickCursorRenderClientMotion(void);
    void UpdateForWindowOwner(void *pWindowOwner);
};

#endif
