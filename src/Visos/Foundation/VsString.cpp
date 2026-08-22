#include "VsString.h"

#include <ctype.h>
#include <string.h>

// 68K 0x107001ea StrCmpI__FPCcPCci
// FUNCTION: LEMBALL 0x00406790
int StrCmpI(const char* p_left, const char* p_right, int p_maxLength)
{
	int len1 = strlen(p_left);
	if (len1 > p_maxLength) {
		len1 = p_maxLength;
	}

	int len2 = strlen(p_right);
	if (len2 > p_maxLength) {
		len2 = p_maxLength;
	}

	if (len1 != len2) {
		return -1;
	}

	int n = p_maxLength;
	if (len1 < n) {
		n = len1;
	}
	if (len2 < n) {
		n = len2;
	}

	for (; n > 0; --n, ++p_left, ++p_right) {
		char c1 = toupper((unsigned char) *p_left);
		char c2 = toupper((unsigned char) *p_right);
		if (c1 != c2) {
			return (int) c1 - (int) c2;
		}
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
// FUNCTION: LEMBALL 0x00458db0
char* VsULtoa(unsigned long p_value, char* p_buffer, int p_radix)
{
	static unsigned int s_maxPowers[17];
	static int s_powersInitialized = 0;

	if (s_powersInitialized == 0) {
		int r = 2;
		do {
			unsigned int limit = 0xFFFFFFFF / (unsigned int) r;
			unsigned int power = r;
			if (r <= limit) {
				do {
					power *= r;
				} while (power <= limit);
			}
			s_maxPowers[r] = power;
			r++;
		} while (r <= 16);
		s_powersInitialized = 1;
	}

	int nonzero = 0;
	int written = 0;
	int count = 0;
	unsigned int p = s_maxPowers[p_radix];
	unsigned int val = p_value;
	do {
		unsigned int digit = val / p;
		if (digit != 0 || nonzero != 0) {
			p_buffer[written++] = "0123456789abcdef"[digit];
			nonzero = 1;
		}
		val -= p * digit;
		p /= p_radix;
	} while (p != 1 && (++count < 33));

	p_buffer[written] = "0123456789abcdef"[val];
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
