#ifndef LEMBALL_FRONTEND_DRAWERS_SUCCFAILDRAWERFIELDVIEWS_H
#define LEMBALL_FRONTEND_DRAWERS_SUCCFAILDRAWERFIELDVIEWS_H

#include "../../Visos/Foundation/tagPRIMS.h"
#include "../../Visos/Graphics/CBitmapRes.h"

struct SuccFailDrawerFieldViews {
	struct CBitmap {
		short m_x;
		short m_y;
		short m_width;
		short m_height;
		short m_sourceX;
		short m_sourceY;
		CResBITMAP* m_resource;
		unsigned int m_flags;
		CRemap* m_remap;
		unsigned int m_unknown1c;
		short m_unknown20;
		short m_unknown22;
		unsigned char m_objectTail[4];
	};

	struct CPrimitive {
		CBitmap m_primitive;
		unsigned char m_remaining[sizeof(tagPRIMS) - sizeof(CBitmap)];
	};

	struct DrawerPrims {
		CBitmap m_primary;
		CBitmap m_secondary;
	};
};

#endif
