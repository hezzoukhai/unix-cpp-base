/*****************************************************************************\
* Initial version: E.BENZIT
*
*
*
*
\*****************************************************************************/

#include <stdio.h>
#include <string.h>

/*****************************************************************************/
#define u32		unsigned int
#define u64		unsigned long
/*****************************************************************************/
static const u32 g_magic_A = 0x02040801;
static const u32 g_magic_B = 0x00804021;
static const u32 g_magic_C = 0xBFEF7FDF;
static const u32 g_magic_D = 0x7DFEFBFF;
/*****************************************************************************/
static u32 X0;
static u32 Y0;
static u32 V0;
static u32 W;
static u32 S;
static u32 T;
/*****************************************************************************/
static int Add(u32 a, u32 b, u32* result)
{
	(*result) = a + b;
	if( (*result) < a || (*result) < b )
	{
		return 1;
	}
	return 0;
}
/*****************************************************************************/
static int Mul_1(u32 a,u32 b, u32* result)
{
	u32 nU,nL;
	u64 nResult;
	u32 nS,nC;

	nResult = ((u64)a)*((u64)b);
	nU = (u32)(nResult>>32);
	nL = (u32)(nResult);

	nC=Add(nU,nL,&nS);
	(*result)=nS+nC;
	return 0;
}
/*****************************************************************************/
static int Mul_2(u32 a,u32 b, u32* result)
{
	u32 nU,nL;
	u64 nResult;
	u32 nD,nE,nF,nS,nC;

	nResult = ((u64)a)*((u64)b);
	nU = (u32)(nResult>>32);
	nL = (u32)(nResult);

	nE = Add(nU,nU,&nD);
	Add(nD,nE*2,&nF);
	nC=Add(nF,nL,&nS);

	(*result) = nS + (nC*2);
	return 0;
}
/*****************************************************************************/
static int Mul_2A(u32 a,u32 b, u32* result)
{
	u32 nU,nL;
	u64 nResult;
	u32 nD,nS,nC;

	nResult = ((u64)a)*((u64)b);
	nU = (u32)(nResult>>32);
	nL = (u32)(nResult);

	Add(nU,nU,&nD);
	nC=Add(nD,nL,&nS);

	(*result) = nS + (nC*2);
	return 0;
}
/*****************************************************************************/
static int BYT_PAT(u32 a,u32 b, u64* byt, unsigned char* pat)
{
	unsigned char nB[8];
	unsigned char nBResult[8];
	int i;
	u32 nResult, nP;

	nB[3]=(a&0x000000FF);
	nB[2]=((a&0x0000FF00)>>8);
	nB[1]=((a&0x00FF0000)>>16);
	nB[0]=((a&0xFF000000)>>24);

	nB[7]=(b&0x000000FF);
	nB[6]=((b&0x0000FF00)>>8);
	nB[5]=((b&0x00FF0000)>>16);
	nB[4]=((b&0xFF000000)>>24);

	nP = 0;
	for(i = 0; i < 8; i++)
	{
		nP = (nP<<1);
		if( nB[i] == 0 )
		{
			nP++;
			nBResult[i] = nP;
		}
		else if( nB[i] == 0xFF )
		{
			nP++;
			nBResult[i] = 0xFF - nP;
		}
		else
		{
			nBResult[i] = nB[i];
		}
	}
	(*pat) = nP;
	(*byt) = 0;
	for( i=0; i < 7; i++)
	{
		(*byt)|=nBResult[i];
		(*byt) = (*byt)<<8;
	}
	(*byt)|=nBResult[i];

	return 0;
}
/*****************************************************************************/
/*int CreateKeysComponents(u32 nJ, u32 nK, u32* X0, u32* Y0, u32* V0, u32* W, u32* S, u32* T)*/
int CreateKeysComponents(u32 nJ, u32 nK)
{

	u32 nJ1,nK1,nP,nQ;
	u64 nByt;
	u32 nJ12,nJ22,nJ14,nJ24,nJ16,nJ26,nJ18,nJ28;
	u32 nK12,nK22,nK14,nK24,nK15,nK25,nK17,nK27,nK19,nK29;
	u32 nH4,nH5,nH6,nH8,nH7,nH9,nH;

	nP =0;
	BYT_PAT(nJ,nK,&nByt,(char*)&nP);
	nJ1 = (u32)(nByt>>32);
	nK1 = (u32)(nByt);
	
	nQ = (1 + nP)*(1 + nP);

	Mul_1(nJ1,nJ1,&nJ12);
	Mul_2(nJ1,nJ1,&nJ22);
	
	Mul_1(nJ12,nJ12,&nJ14);
	Mul_2(nJ22,nJ22,&nJ24);
	
	Mul_1(nJ12,nJ14,&nJ16);
	Mul_2(nJ22,nJ24,&nJ26);

	Mul_1(nJ12,nJ16,&nJ18);
	Mul_2(nJ22,nJ26,&nJ28);

	nH4 = (nJ14^nJ24);
	nH6 = (nJ16^nJ26);
	nH8 = (nJ18^nJ28);


	Mul_1(nK1,nK1,&nK12);
	Mul_2(nK1,nK1,&nK22);

	Mul_1(nK12,nK12,&nK14);
	Mul_2(nK22,nK22,&nK24);

	Mul_1(nK1,nK14,&nK15);
	Mul_2(nK1,nK24,&nK25);

	Mul_1(nK12,nK15,&nK17);
	Mul_2(nK22,nK25,&nK27);

	Mul_1(nK12,nK17,&nK19);
	Mul_2(nK22,nK27,&nK29);

	nH = (nK15^nK25);
	Mul_2(nH,nQ,&nH5);
	nH7 = (nK17^nK27);
	nH9 = (nK19^nK29);

	BYT_PAT(nH4,nH5,&nByt,(char*)&nP);
	X0 = (u32)(nByt>>32);
	Y0 = (u32)(nByt);

	BYT_PAT(nH6,nH7,&nByt,(char*)&nP);
	V0 = (u32)(nByt>>32);
	W  = (u32)(nByt);

	BYT_PAT(nH8,nH9,&nByt,(char*)&nP);
	S = (u32)(nByt>>32);
	T = (u32)(nByt);

	return 0;
}
/*****************************************************************************/
/*int CalculMAA(unsigned char* buffer, int nSize, u32 X0, u32 Y0, u32 V0, u32 W, u32 S, u32 T, u32* MAA)*/
int CalculMAA(unsigned char* buffer, int nSize, u32* MAA)
{

	u32 V,E,X,Y,F,G,Z;
	u32 nextX,nextY;
	unsigned char lBuffer[4096];
	int n,lSize,i;
	u32 mi;
	u32* lpBuffer = (u32*)&lBuffer;
	u32 nBit;
	u32 lsbs;

	X = X0;
	Y = Y0;
	V = V0;
	
	memcpy(lBuffer,buffer,nSize);
	n = nSize%4;
	if( n > 0 )
		n = 4 - n;

	while( n > 0 )
	{
		lBuffer[nSize++]=0;
		n--;
	}
	
	memcpy(lBuffer + nSize,(char*)&S,4); nSize+=4;
	memcpy(lBuffer + nSize,(char*)&T,4); nSize+=4;

	n = nSize/4;
	nBit = 0;
	for(i = 0; i < n; i++ )
	{
		mi = (*lpBuffer);
		V = (V << 1) | (V >> (sizeof(V)*8 - 1));
		E = (V^W);
		X = (X^mi);
		Y = (Y^mi);
		F = E + Y;
		G = E + X;
		F = (F|g_magic_A);
		G = (G|g_magic_B);
		F = (F&g_magic_C);
		G = (G&g_magic_D);
		Mul_1(X,F,&nextX);
		Mul_2A(Y,G,&nextY);
		X = nextX;
		Y = nextY;

		lpBuffer++;
	}
	Z = (X^Y);
	(*MAA) = Z;
	return 0;

}
/*****************************************************************************/
