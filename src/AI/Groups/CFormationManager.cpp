#include "CFormationManager.h"

#include "../../Visos/Foundation/VSTrig.h"
#include "Visos/Foundation/CFixed.h"
#include "Visos/Foundation/CVector.h"

#include <new.h>
#include <stddef.h>
#define FORMATION_VECTOR_COUNT 8
// FUNCTION: LEMBALL 0x0041a140
CFormationManager::CFormationManager()
{
	CVector* formation;
	CVector* source;
	int formationCount;
	int vectorCount;
	int y;
	int x;

	source = m_sourceVectors;
	formation = (CVector*) g_aFormationTemplates;
	formationCount = 3;
	do {
		vectorCount = FORMATION_VECTOR_COUNT;
		do {
			y = formation->m_yFixed;
			x = formation->m_xFixed;
			x <<= 12;
			source++;
			y <<= 12;
			formation++;
			vectorCount--;
			source[-1].m_xFixed = x;
			source[-1].m_yFixed = y;
		} while (vectorCount != 0);
		formationCount--;
	} while (formationCount != 0);
}

// FUNCTION: LEMBALL 0x0041a1b0
void CFormationManager::Restart()
{
	m_restartState = 0;
}

// FUNCTION: LEMBALL 0x0041a1c0
CFormationManager::~CFormationManager()
{
}

// FUNCTION: LEMBALL 0x0041a1d0
void CFormationManager::TransformFormation(int p_formationIndex, int p_angle)
{
	int angle = p_angle;
	CVector* source;
	CVector* transformed;
	int remaining;

	source = m_sourceVectors + p_formationIndex * FORMATION_VECTOR_COUNT;
	transformed = m_transformedVectors;
	remaining = FORMATION_VECTOR_COUNT;
	do {
		VSTrig* trig = g_pVSTrig;
		int sine;
		if (angle < 0) {
			sine = -trig->m_sine[(-angle) % TRIG_ANGLE_FULL_TURN].m_value;
		}
		else {
			sine = g_pVSTrig->m_sine[angle % TRIG_ANGLE_FULL_TURN].m_value;
		}
		CFixed sin(sine);
		unsigned int cosStorage;
		if (angle + TRIG_ANGLE_QUARTER_TURN < 0) {
			new (&cosStorage)
				CFixed(-g_pVSTrig->m_sine[(-TRIG_ANGLE_QUARTER_TURN - angle) % TRIG_ANGLE_FULL_TURN].m_value);
		}
		else {
			new (&cosStorage)
				CFixed(g_pVSTrig->m_sine[(angle + TRIG_ANGLE_QUARTER_TURN) % TRIG_ANGLE_FULL_TURN].m_value);
		}
		CFixed& cos = *static_cast<CFixed*>(static_cast<void*>(&cosStorage));
		CVector rotated = trig->Rotate(*source, sin, cos);
		int x = rotated.m_xFixed;
		int y = rotated.m_yFixed;
		transformed->m_xFixed = x;
		transformed->m_yFixed = y;
		source++;
		transformed++;
		remaining--;
	} while (remaining != 0);
}

// FUNCTION: LEMBALL 0x0041a2e0
CVector* CFormationManager::GetFirstVector()
{
	m_restartState = 0;
	return m_transformedVectors;
}

// FUNCTION: LEMBALL 0x0041a300
CVector* CFormationManager::GetNextVector()
{
	int index = ++m_restartState;
	if (index >= FORMATION_VECTOR_COUNT) {
		return NULL;
	}
	return &m_transformedVectors[index];
}

// FUNCTION: LEMBALL 0x0041a320
CVector* CFormationManager::GetAVector(int p_index)
{
	if (p_index >= FORMATION_VECTOR_COUNT) {
		p_index -= FORMATION_VECTOR_COUNT;
	}
	return &m_transformedVectors[p_index];
}

// GLOBAL: LEMBALL 0x004a7834
CFormationManager* g_pGenericGroupFormationManager;
