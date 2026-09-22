#ifndef LEMBALL_VIEWS_INPUT_CCURSORMOTION_H
#define LEMBALL_VIEWS_INPUT_CCURSORMOTION_H

#include "../../Visos/Graphics/CClipRect.h"  // complete type
#include "../../Visos/Graphics/CSolidRect.h" // complete type

class CAi;
class CBaseQueue;
class CLemmingAnimsManager;
class CMap;
struct CVsPoint;
// SIZE 0x78
class CCursorMotion {
public:
	CCursorMotion(CLemmingAnimsManager* p_anims, CAi* p_ai, CMap* p_map);
	~CCursorMotion();
	void PostPosition();
	void Process();
	void Draw(unsigned int p_unused);
	void DrawAt(unsigned int p_unused, const CVsPoint& p_position);
	void SetPosition(const CVsPoint& p_position);
	CVsPoint GetPosition();
	void StopVertical();
	void StopHorizontal();
	void StartHorizontal(unsigned int p_positive);
	void StartVertical(unsigned int p_positive);
	void SetDrawOffset(unsigned int p_aux0, unsigned int p_aux1);

private:
	unsigned int m_unknown00;        // 0x00
	CMap* m_map;                     // 0x04
	CAi* m_ai;                       // 0x08
	CBaseQueue* m_aiQueue;           // 0x0c
	CLemmingAnimsManager* m_anims;   // 0x10
	CClipRect m_clipRects[2];        // 0x14
	int m_fixedX;                    // 0x34
	int m_fixedY;                    // 0x38
	int m_velocityX;                 // 0x3c
	int m_velocityY;                 // 0x40
	int m_accelerationX;             // 0x44
	int m_accelerationY;             // 0x48
	unsigned int m_lastTickX;        // 0x4c
	unsigned int m_lastTickY;        // 0x50
	unsigned int m_horizontalActive; // 0x54
	unsigned int m_verticalActive;   // 0x58
	unsigned int m_positionDirty;    // 0x5c
	CSolidRect m_marker;             // 0x60
	unsigned int m_aux0;             // 0x70
	unsigned int m_aux1;             // 0x74
};

#endif
