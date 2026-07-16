#ifndef VISUAL_SCIENCES_GEOMETRY_OWNER_H
#define VISUAL_SCIENCES_GEOMETRY_OWNER_H

#include "ENGINE/GDI/VSWINDOW.H"

// SIZE 0x38
class GameGeometryOwnerRectState {
public:
    void SetEnabled(int fEnabled);

    void **m_pVtable00;
    int m_fEnabled04;
    int m_fPointerActive08;
    GameGeometryOwnerRectState *m_pParent0C;
    int m_anTransient10[6];
    int m_fDispatchPointerMove28;
    int m_fDispatchOutsideRelease2C;
    VS_XYPair m_Size30;
    VS_XYPair m_Position34;
};

#endif
