#include <stdio.h>

char szCb2aMsgType  [5];
char sFileNumber [7];/*LBN20150119 - #44968*/
int  nFileNumber;/*LBN20150119 - #44968*/
int  nPrevFileNumber;/*LBN20150119 - #44968*/
int  nRemittanceSeq;/*LBN20150119 - #44968*/
char sCurrentRemittance [7];/*LBN20150119 - #44968*/
int  nPreviousRemittance;
int  nCurrentRemittance;
int  nPreviousTrxRecord;
int  nCurrentTrxRecord;
char sRemittanceCurrency  [4];

FILE *fIn, *fOut;
FILE *fTmpCRR ,*fCRR;

char szLine [256];
char protocol_version[4+1];/*LBN20120417 */