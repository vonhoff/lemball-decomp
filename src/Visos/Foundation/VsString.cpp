#include "VsString.h"

#include <ctype.h>
#include <string.h>

#pragma intrinsic(strlen)

// 68K 0x107001ea StrCmpI__FPCcPCci
// STUB: LEMBALL 0x00406790
int StrCmpI(const char* p_left, const char* p_right, int p_maxLength)
{
	int len1 = strlen(p_left);
	int len2 = strlen(p_right);
	int maxLength = p_maxLength;

	if (len1 > maxLength) {
		len1 = maxLength;
	}
	if (len2 > maxLength) {
		len2 = maxLength;
	}

	if (len2 != len1) {
		return -1;
	}

	if (len1 < maxLength) {
		maxLength = len1;
	}
	if (len2 < maxLength) {
		maxLength = len2;
	}

	while (maxLength != 0) {
		char c1 = islower(*p_left) ? (char) toupper(*p_left) : *p_left;
		char c2 = islower(*p_right) ? (char) toupper(*p_right) : *p_right;
		int diff = (int) c1 - (int) c2;
		if (diff != 0) {
			return diff;
		}
		p_left++;
		p_right++;
		maxLength--;
	}

	return 0;
}

// 68K 0x1021429c vsLtoa__FlPci
// FUNCTION: LEMBALL 0x00458d80
void VsLtoa(long p_value, char* p_buffer, int p_radix)
{
	if (p_value < 0) {
		*p_buffer++ = '-';
		p_value = -p_value;
	}
	VsULtoa(p_value, p_buffer, p_radix);
}

// 68K 0x102142e8 vsULtoa__FUlPci
// STUB: LEMBALL 0x00458db0
char* VsULtoa(unsigned long p_value, char* p_buffer, int p_radix)
{
	static unsigned int s_maxPowers[17];
	static int s_powersInitialized = 0;

	if (s_powersInitialized == 0) {
		unsigned int r = 2;
		do {
			unsigned int pow = r;
			unsigned int lim = 0xFFFFFFFF / r;
			if (lim >= r) {
				do {
					pow *= r;
				} while (pow <= lim);
			}
			s_maxPowers[r] = pow;
			r++;
		} while ((int) r <= 16);
		s_powersInitialized = 1;
	}

	unsigned int power = s_maxPowers[p_radix];
	int hasWritten = 0;
	int written = 0;
	int i = 0;

	do {
		unsigned int digit = p_value / power;
		if (digit != 0 || hasWritten != 0) {
			p_buffer[written] = "0123456789abcdef"[digit];
			written++;
			hasWritten = 1;
		}
		p_value -= power * digit;
		power /= p_radix;
		if (power == 1) {
			break;
		}
		i++;
	} while (i < 33);

	p_buffer[written] = "0123456789abcdef"[p_value];
	p_buffer[written + 1] = '\0';
	return p_buffer;
}

// 68K 0x10212fae OkFailed__FUc
// FUNCTION: LEMBALL 0x00458f10
char* OkFailed(int p_success)
{
	return p_success ? "OK" : "Failed";
}

// 68K 0x10212fda strtol__FPcPPci
// FUNCTION: LEMBALL 0x00458f30
int Strtol(char* p_text, char** p_end, int p_base)
{
	if (p_base == 10) {
		int result = 0;
		while (*p_text >= '0' && *p_text <= '9') {
			result = result * 10 + (*p_text - '0');
			p_text++;
		}
		*p_end = p_text;
		return result;
	}
	*p_end = p_text;
	return 0;
}
