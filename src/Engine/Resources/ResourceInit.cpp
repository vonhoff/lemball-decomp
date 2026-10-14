#include "ResourceInit.h"

#include "Engine/Graphics/Palettes/CBasePalManager.h"
#include "Engine/Graphics/Palettes/CPaletteManager.h"
#include "Engine/Resources/ResourceTypeList.h"
#include "ResourceChunkTypes.h"

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

	list = new ResourceTypeList(2);
	list->m_typeCodes[list->m_count] = RESOURCE_CHUNK_INT;
	list->m_count = list->m_count + 1;
	list->m_typeCodes[list->m_count] = RESOURCE_CHUNK_ZRLE;
	list->m_count = list->m_count + 1;
	g_pResourceTypes = list;

	list = new ResourceTypeList(1);
	list->m_typeCodes[list->m_count] = RESOURCE_CHUNK_ZRLE;
	list->m_count = list->m_count + 1;
	g_pCompressedResourceTypes = list;

	list = new ResourceTypeList(2);
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
	delete g_pBasePalManager;
	delete g_pPreloadedResourceTypes;
	delete g_pResourceTypes;
	delete g_pCompressedResourceTypes;
	return true;
}
