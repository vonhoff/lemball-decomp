#ifndef LEMBALL_GAME_CURSOR_RENDER_CLIENT_H
#define LEMBALL_GAME_CURSOR_RENDER_CLIENT_H

#include "VISOS/VSMATH.H"

struct GameCursorRenderClientInterface {
    virtual void Reserved00(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual void Reserved08(void) = 0;
    virtual void Reserved0C(void) = 0;
    virtual void Reserved10(void) = 0;
    virtual void RefreshIdleState(void) = 0;
};

struct GameCursorRenderClient : public GameCursorRenderClientInterface {
    char m_abReserved04[0x0c];
    VsMathPoint2D m_Position10;
    char m_abReserved14[4];
    void *m_pRenderResource18;
    char m_abReserved1C[0x10];
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
};

#endif
