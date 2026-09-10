#include "CdLoadAnimProgress.h"

#include "../../Control/Game/GameMain.h"
#include "../../Platform/Windows/Entry.h"
#include "../../Views/Display/Main2DDisplay.h"
#include "../../Visos/Foundation/ChangeList.h"
#include "../../Visos/Foundation/Fixed.h"
#include "../../Visos/Foundation/Vector.h"
#include "../../Visos/Foundation/VsPoint.h"
#include "../../Visos/Foundation/VsRect.h"
#include "../../Visos/Foundation/VsTrig.h"
#include "../../Visos/Graphics/Cursor.h"
#include "../../Visos/Graphics/GWnd.h"
#include "../../Visos/Graphics/Gdi.h"
#include "../../Visos/Graphics/PvGWnd.h"
#include "../../Visos/Graphics/VsGdi.h"
#include "../../Visos/Resources/Manifest.h"
#include "../../Visos/Resources/ResBitmap.h"
#include "../../Visos/Resources/ResPalette.h"

#include <new.h>

// 68K 0x108028fe Draw__11CCDLoadAnimFs
// FUNCTION: LEMBALL 0x0044b340
void CdLoadAnimProgress::Draw(short p_progress)
{
	TargetSynchronizeLoadProgress();
	m_draw.m_progress = p_progress;
	m_draw.m_display->RefreshView();
}
