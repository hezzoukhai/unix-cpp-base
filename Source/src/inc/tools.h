#ifndef HPS_TOOLS_H
#define HPS_TOOLS_H
#include <define.h>

int HexToAscii (const BYTE *sSrc, BYTE *sDst, int nLng);
int AsciiToHex (const BYTE *sSrc, BYTE *sDst, int nLng);
int BcdToAscii (const BYTE *sSrc, BYTE *sDst, int nLng);
int AsciiToBcd (const BYTE *sSrc, BYTE *sDst, int nLng);
void AsciiToEbcdic(BYTE *sSrc, BYTE *sDst, int nLng);
void AsciiToEbcdic2(BYTE *sSrc, BYTE *sDst, int nLng);
void EbcdicToAscii(BYTE *sSrc, BYTE *sDst, int nLng);
int SetBit ( char *sBitMap, int nBitNbr);
int ClearBit ( char *sBitMap, int nBitNbr);
int CheckBit( char *cPtr, int nBitNbr );
int FieldPresent( char *cPtr, int nFldNbr);
int ClearAllBits (char *sBitMap);
void szcpy (BYTE *sDst, const BYTE *sSrc, int nLen);
char *RightAdjust(char *sSrc, int nLen);
#endif /** HPS_TOOLS_H **/
