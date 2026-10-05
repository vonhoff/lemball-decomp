#include "ResourceChunkTypes.h"
#include "Engine/Startup/VsInit.h"
#include "Engine/Graphics/Palettes/CBasePalManager.h"
#include "Engine/Graphics/Palettes/CPaletteManager.h"
#include "Engine/Resources/ResourceTypeList.h"

#include <stddef.h>

namespace
{
enum {
	BASE_PALETTE_MANAGER_CAPACITY = 32
};
}

// FUNCTION: LEMBALL 0x0045b900
bool _RES_Init()
{
	ResourceTypeList* list;

	list = new ResourceTypeList;
	if (list != NULL) {
		list->m_capacity = 2;
		list->m_currentIndex = -1;
		list->m_count = 0;
		list->m_typeCodes = new unsigned int[list->m_capacity];
	}
	else {
		list = NULL;
	}
	list->m_typeCodes[list->m_count] = RESOURCE_CHUNK_INT;
	list->m_count = list->m_count + 1;
	list->m_typeCodes[list->m_count] = RESOURCE_CHUNK_ZRLE;
	list->m_count = list->m_count + 1;
	g_pResourceTypes = list;

	list = new ResourceTypeList;
	if (list != NULL) {
		list->m_capacity = 1;
		list->m_currentIndex = -1;
		list->m_count = 0;
		list->m_typeCodes = new unsigned int[list->m_capacity];
	}
	else {
		list = NULL;
	}
	list->m_typeCodes[list->m_count] = RESOURCE_CHUNK_ZRLE;
	list->m_count = list->m_count + 1;
	g_pCompressedResourceTypes = list;

	list = new ResourceTypeList;
	if (list != NULL) {
		list->m_capacity = 2;
		list->m_currentIndex = -1;
		list->m_count = 0;
		list->m_typeCodes = new unsigned int[list->m_capacity];
	}
	else {
		list = NULL;
	}
	list->m_typeCodes[list->m_count] = RESOURCE_CHUNK_STRING;
	list->m_count = list->m_count + 1;
	list->m_typeCodes[list->m_count] = RESOURCE_CHUNK_INT;
	list->m_count = list->m_count + 1;
	g_pPreloadedResourceTypes = list;

	g_pBasePalManager = new CPaletteManager(BASE_PALETTE_MANAGER_CAPACITY);
	return true;
}

// FUNCTION: LEMBALL 0x0045ba50
bool _RES_Quit()
{
	ResourceTypeList* list;

	delete g_pBasePalManager;
	list = g_pPreloadedResourceTypes;
	if (list != NULL) {
		delete[] list->m_typeCodes;
		delete list;
	}
	list = g_pResourceTypes;
	if (list != NULL) {
		delete[] list->m_typeCodes;
		delete list;
	}
	list = g_pCompressedResourceTypes;
	if (list != NULL) {
		delete[] list->m_typeCodes;
		delete list;
	}
	return true;
}
