#ifndef LEMBALL_VISOS_TARGET_TARGETDIRECTDRAWERROR_H
#define LEMBALL_VISOS_TARGET_TARGETDIRECTDRAWERROR_H

char* FormatUnknownDirectDrawError(long p_result);
void TargetDirectDrawCreateFailureHook(int p_context, long p_result);

#endif
