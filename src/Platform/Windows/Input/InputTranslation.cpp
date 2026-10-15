#include "Platform/Windows/Input/InputTranslation.h"

#include <stddef.h>

#include "Engine/Input/CMasterInput.h"
#include "Engine/Queues/tagMESSAGE.h"
#include "Platform/Windows/WindowsTypes.h"

enum eWindowsVirtualKey {
	WINDOWS_VK_OEM_COMMA = 0xbc,
	WINDOWS_VK_OEM_PERIOD = 0xbe
};

// GLOBAL: LEMBALL 0x004a27a0
unsigned int g_dwInputQuitRequested = 0;

// GLOBAL: LEMBALL 0x004a2808
InputTranslationEntry g_dwInputTranslationPairs[61] = {{VK_SPACE, INPUT_KEY_SPACE},
													   {WINDOWS_VK_OEM_PERIOD, INPUT_KEY_PERIOD},
													   {WINDOWS_VK_OEM_COMMA, INPUT_KEY_COMMA},
													   {VK_F4, INPUT_KEY_F4},
													   {VK_ESCAPE, INPUT_KEY_ESCAPE},
													   {'A', INPUT_KEY_A},
													   {'B', INPUT_KEY_B},
													   {'C', INPUT_KEY_C},
													   {'D', INPUT_KEY_D},
													   {'E', INPUT_KEY_E},
													   {'F', INPUT_KEY_F},
													   {'G', INPUT_KEY_G},
													   {'H', INPUT_KEY_H},
													   {'I', INPUT_KEY_I},
													   {'J', INPUT_KEY_J},
													   {'K', INPUT_KEY_K},
													   {'L', INPUT_KEY_L},
													   {'M', INPUT_KEY_M},
													   {'N', INPUT_KEY_N},
													   {'O', INPUT_KEY_O},
													   {'P', INPUT_KEY_P},
													   {'Q', INPUT_KEY_Q},
													   {'R', INPUT_KEY_R},
													   {'S', INPUT_KEY_S},
													   {'T', INPUT_KEY_T},
													   {'U', INPUT_KEY_U},
													   {'V', INPUT_KEY_V},
													   {'W', INPUT_KEY_W},
													   {'X', INPUT_KEY_X},
													   {'Y', INPUT_KEY_Y},
													   {'Z', INPUT_KEY_Z},
													   {'0', INPUT_KEY_0},
													   {'1', INPUT_KEY_1},
													   {'2', INPUT_KEY_2},
													   {'3', INPUT_KEY_3},
													   {'4', INPUT_KEY_4},
													   {'5', INPUT_KEY_5},
													   {'6', INPUT_KEY_6},
													   {'7', INPUT_KEY_7},
													   {'8', INPUT_KEY_8},
													   {'9', INPUT_KEY_9},
													   {VK_NUMPAD0, INPUT_KEY_0},
													   {VK_NUMPAD1, INPUT_KEY_1},
													   {VK_NUMPAD2, INPUT_KEY_2},
													   {VK_NUMPAD3, INPUT_KEY_3},
													   {VK_NUMPAD4, INPUT_KEY_4},
													   {VK_NUMPAD5, INPUT_KEY_5},
													   {VK_NUMPAD6, INPUT_KEY_6},
													   {VK_NUMPAD7, INPUT_KEY_7},
													   {VK_NUMPAD8, INPUT_KEY_8},
													   {VK_NUMPAD9, INPUT_KEY_9},
													   {VK_UP, INPUT_KEY_UP},
													   {VK_DOWN, INPUT_KEY_DOWN},
													   {VK_LEFT, INPUT_KEY_LEFT},
													   {VK_RIGHT, INPUT_KEY_RIGHT},
													   {VK_RETURN, INPUT_KEY_RETURN},
													   {VK_DELETE, INPUT_KEY_DELETE},
													   {VK_DELETE, INPUT_KEY_DELETE},
													   {VK_BACK, INPUT_KEY_BACKSPACE},
													   {VK_SHIFT, INPUT_KEY_SHIFT},
													   {VK_LSHIFT, INPUT_KEY_LEFT_SHIFT}};

// FUNCTION: LEMBALL 0x00456660
bool InitInput()
{
	g_pMasterInput->m_state = g_pMasterInput->m_state | MASTER_INPUT_ACTIVE_STATE_MASK;
	return true;
}

// FUNCTION: LEMBALL 0x00456670
bool QuitInput()
{
	g_pMasterInput->m_state = g_pMasterInput->m_state & ~MASTER_INPUT_ACTIVE_STATE_MASK;
	return true;
}

// FUNCTION: LEMBALL 0x00472220
bool __stdcall HandleInputQuitEvent(const tagMESSAGE* p_event)
{
	switch ((unsigned int) p_event->m_type) {
	case MESSAGE_KEY_UP:
		if (p_event->m_payload == NULL && (p_event->m_code == INPUT_KEY_ACTIVATE || p_event->m_code == VK_DELETE)) {
			g_dwInputQuitRequested = 1;
			return false;
		}
		break;
	case MESSAGE_MOUSE_BUTTON_UP:
		g_dwInputQuitRequested = 1;
		break;
	}
	return false;
}
