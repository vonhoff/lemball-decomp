#ifndef LEMBALL_FRONTEND_DRAWERS_SUCCFAILDRAWERFIELDVIEWS_H
#define LEMBALL_FRONTEND_DRAWERS_SUCCFAILDRAWERFIELDVIEWS_H

#include "../../Visos/Foundation/Prims.h"
#include "../../Visos/Graphics/BitmapRes.h"

// Views begin at BitmapRes::m_x; vtable pointer omitted. Sizes retain original
// BitmapRes/Prims element strides while keeping reconstructed field names.
struct SuccFailDrawerFieldViews {
	struct Bitmap {
		short m_x;
		short m_y;
		short m_width;
		short m_height;
		short m_sourceX;
		short m_sourceY;
		ResBitmap* m_resource;
		unsigned int m_flags;
		Remap* m_remap;
		unsigned int m_unknown1c;
		short m_unknown20;
		short m_unknown22;
		unsigned char m_objectTail[4];
	};

	struct Primitive {
		Bitmap m_primitive;
		unsigned char m_remaining[sizeof(Prims) - sizeof(Bitmap)];
	};

	struct DrawerPrims {
		Bitmap m_primary;
		Bitmap m_secondary;
	};
};

#endif
