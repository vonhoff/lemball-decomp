#include "Visos/Foundation/VsInit.h"
#include "Visos/Graphics/CBasePalManager.h"
#include "Visos/Graphics/CPaletteManager.h"
#include "Visos/Resources/ResourceTypeList.h"

#include <new.h>

// FUNCTION: LEMBALL 0x0045b900
bool _RES_Init()
{
	ResourceTypeList* list;

	list = (ResourceTypeList*) operator new(sizeof(ResourceTypeList));
	if (list != 0) {
		list->m_capacity = 2;
		list->m_currentIndex = -1;
		list->m_count = 0;
		list->m_typeCodes = (unsigned int*) operator new(list->m_capacity * sizeof(unsigned int));
	}
	else {
		list = 0;
	}
	list->m_typeCodes[list->m_count] = 0x494e5420;
	list->m_count = list->m_count + 1;
	list->m_typeCodes[list->m_count] = 0x5a524c45;
	list->m_count = list->m_count + 1;
	g_pResourceTypes = list;

	list = (ResourceTypeList*) operator new(sizeof(ResourceTypeList));
	if (list != 0) {
		list->m_capacity = 1;
		list->m_currentIndex = -1;
		list->m_count = 0;
		list->m_typeCodes = (unsigned int*) operator new(list->m_capacity * sizeof(unsigned int));
	}
	else {
		list = 0;
	}
	list->m_typeCodes[list->m_count] = 0x5a524c45;
	list->m_count = list->m_count + 1;
	g_pCompressedResourceTypes = list;

	list = (ResourceTypeList*) operator new(sizeof(ResourceTypeList));
	if (list != 0) {
		list->m_capacity = 2;
		list->m_currentIndex = -1;
		list->m_count = 0;
		list->m_typeCodes = (unsigned int*) operator new(list->m_capacity * sizeof(unsigned int));
	}
	else {
		list = 0;
	}
	list->m_typeCodes[list->m_count] = 0x53545247;
	list->m_count = list->m_count + 1;
	list->m_typeCodes[list->m_count] = 0x494e5420;
	list->m_count = list->m_count + 1;
	g_pPreloadedResourceTypes = list;

	g_pBasePalManager = new CPaletteManager(0x20);
	return 1;
}

// FUNCTION: LEMBALL 0x0045ba50
bool _RES_Quit()
{
	ResourceTypeList* list;

	delete g_pBasePalManager;
	list = g_pPreloadedResourceTypes;
	if (list != 0) {
		operator delete(list->m_typeCodes);
		operator delete(list);
	}
	list = g_pResourceTypes;
	if (list != 0) {
		operator delete(list->m_typeCodes);
		operator delete(list);
	}
	list = g_pCompressedResourceTypes;
	if (list != 0) {
		operator delete(list->m_typeCodes);
		operator delete(list);
	}
	return 1;
}
