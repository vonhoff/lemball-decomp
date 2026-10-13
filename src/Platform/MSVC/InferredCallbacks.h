#ifndef LEMBALL_PLATFORM_MSVC_INFERREDCALLBACKS_H
#define LEMBALL_PLATFORM_MSVC_INFERREDCALLBACKS_H

unsigned int CGameObjectReturnZero();
unsigned int CNodeReturnOne();
unsigned int __stdcall CPaintGunReturnZero1(unsigned int p_arg0, unsigned int p_arg1);
unsigned int __stdcall CPaintGunReturnZero2(unsigned int p_arg0, unsigned int p_arg1);
unsigned int __stdcall CPaintGunManagerReturnZero1(unsigned int p_arg0, unsigned int p_arg1);
unsigned int __stdcall CPaintGunManagerReturnZero2(unsigned int p_arg0, unsigned int p_arg1);
unsigned int __stdcall CIceReturnZero(unsigned int p_arg0, unsigned int p_arg1);
unsigned int __stdcall CMoverReturnZero(unsigned int p_arg0, unsigned int p_arg1);
void C2DNoOp1();
void C2DNoOp2();
void CPanelNoOp();
void CCDLoadAnimNoOp1();
void CGunControllerNoOp();
void CHiliteControllerNoOp();
void __stdcall NoOpPop4(unsigned int p_argument);

#endif
