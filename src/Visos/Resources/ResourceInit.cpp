#include "ResourceChunkTypes.h"
#include "Visos/Startup/VsInit.h"
#include "Visos/Graphics/Palettes/CBasePalManager.h"
#include "Visos/Graphics/Palettes/CPaletteManager.h"
#include "Visos/Resources/ResourceTypeList.h"

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

	list = (ResourceTypeList*) operator new(sizeof(ResourceTypeList));
	if (list != NULL) {
		list->m_capacity = 2;
		list->m_currentIndex = -1;
		list->m_count = 0;
		list->m_typeCodes = (unsigned int*) operator new(list->m_capacity * sizeof(unsigned int));
	}
	else {
		list = NULL;
	}
	list->m_typeCodes[list->m_count] = RESOURCE_CHUNK_INT;
	list->m_count = list->m_count + 1;
	list->m_typeCodes[list->m_count] = RESOURCE_CHUNK_ZRLE;
	list->m_count = list->m_count + 1;
	g_pResourceTypes = list;

	list = (ResourceTypeList*) operator new(sizeof(ResourceTypeList));
	if (list != NULL) {
		list->m_capacity = 1;
		list->m_currentIndex = -1;
		list->m_count = 0;
		list->m_typeCodes = (unsigned int*) operator new(list->m_capacity * sizeof(unsigned int));
	}
	else {
		list = NULL;
	}
	list->m_typeCodes[list->m_count] = RESOURCE_CHUNK_ZRLE;
	list->m_count = list->m_count + 1;
	g_pCompressedResourceTypes = list;

	list = (ResourceTypeList*) operator new(sizeof(ResourceTypeList));
	if (list != NULL) {
		list->m_capacity = 2;
		list->m_currentIndex = -1;
		list->m_count = 0;
		list->m_typeCodes = (unsigned int*) operator new(list->m_capacity * sizeof(unsigned int));
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
		operator delete(list->m_typeCodes);
		operator delete(list);
	}
	list = g_pResourceTypes;
	if (list != NULL) {
		operator delete(list->m_typeCodes);
		operator delete(list);
	}
	list = g_pCompressedResourceTypes;
	if (list != NULL) {
		operator delete(list->m_typeCodes);
		operator delete(list);
	}
	return true;
}
