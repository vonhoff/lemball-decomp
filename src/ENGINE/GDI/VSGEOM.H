#ifndef VISUAL_SCIENCES_GEOMETRY_OWNER_H
#define VISUAL_SCIENCES_GEOMETRY_OWNER_H

#include "ENGINE/GDI/VSWINDOW.H"

// SIZE 0x38
class GameGeometryOwnerRectState {
public:
	void SetEnabled(int fEnabled);

	void** m_pVtable00; // 0x00
	int m_fEnabled04; // 0x04
	int m_fPointerActive08; // 0x08
	GameGeometryOwnerRectState* m_pParent0C; // 0x0c
	int m_anTransient10[6]; // 0x10
	int m_fDispatchPointerMove28; // 0x28
	int m_fDispatchOutsideRelease2C; // 0x2c
	VS_XYPair m_Size30; // 0x30
	VS_XYPair m_Position34; // 0x34
};

#endif
