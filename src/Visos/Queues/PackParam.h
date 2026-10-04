#ifndef LEMBALL_VISOS_MESSAGING_PACKPARAM_H
#define LEMBALL_VISOS_MESSAGING_PACKPARAM_H

enum {
	PACK_PARAM_HIGH_WORD_SHIFT = 16
};

int PackParam(short p_low, short p_high);
#endif
