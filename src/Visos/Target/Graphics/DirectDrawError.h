#ifndef LEMBALL_VISOS_TARGET_GRAPHICS_DIRECTDRAWERROR_H
#define LEMBALL_VISOS_TARGET_GRAPHICS_DIRECTDRAWERROR_H

enum {
	DIRECT_DRAW_ERROR_CODE_MASK = 0x0fff
};

char* FormatUnknownDirectDrawError(long p_result);
void OnDirectDrawCreateFailure(int p_context, long p_result);

#endif
