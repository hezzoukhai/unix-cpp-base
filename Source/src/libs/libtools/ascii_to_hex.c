#include<stdio.h>
unsigned char Ascii_To_Hex(c)
char c;
{
   if ( (c>= '0') && (c<='9'))
      return (c-'0');

   if ( (c>= 'A') && (c<='F'))
      return (c-'A'+10);

   if ( (c>= 'a') && (c<='f'))
      return (c-'a'+10);
   return(0x00);
}

int AsciiToHex (const unsigned char *sSrc, unsigned char *sDst, int nLng)
{
   int  i, j;
   char cLeft, cRight;
   char sWorkSrc [256];
   char sWorkDst [256];

   memset(sWorkSrc, '0', sizeof(sWorkSrc));
   memset(sWorkDst, '\0', sizeof(sWorkDst));

   if (nLng % 2){
      memcpy(sWorkSrc+1, sSrc, nLng);
      nLng += 1;
   }
   else
      memcpy(sWorkSrc, sSrc, nLng);

   for (i=0,j=0; i<nLng; i++){
      cLeft  = Ascii_To_Hex(sWorkSrc[j++]) << 4;
      cRight = Ascii_To_Hex(sWorkSrc[j++]) & 0x0F ;
      sWorkDst[i] = cLeft | cRight;
   }

   memcpy(sDst, sWorkDst, i/2);

   return(i/2);            /* Number of Bytes */
}
int AsciiToBcd (const unsigned char *sSrc, unsigned char *sDst, int nLng)
{
   int  i, j;
   char cLeft, cRight;
   char sWorkSrc [256];
   char sWorkDst [256];

   memset(sWorkSrc, '0', sizeof(sWorkSrc));
   memset(sWorkDst, '\0', sizeof(sWorkDst));

   if (nLng % 2){
      memcpy(sWorkSrc+1, sSrc, nLng);
      nLng += 1;
   }
   else
      memcpy(sWorkSrc, sSrc, nLng);

   for (i=0,j=0; i<nLng; i++){
      cLeft  = (sWorkSrc[j++] - '0') << 4;
      cRight =  sWorkSrc[j++] - '0';
      sWorkDst[i] = cLeft | cRight;
   }

   memcpy(sDst, sWorkDst, i/2);

   return(i/2);            /* Number of Bytes */
}
main(int argc, char **argv)
{
char sWork[512];
char sDest[512];
int nLen = 0;
int i = 0;

memset(sWork, 0, sizeof(sWork));
memset(sDest, 0, sizeof(sDest));


sWork[0] = 0X86;
printf("\n sWork : %s",sWork);
sWork[1] = 0X82;
printf("\n sWork : %s",sWork);
sWork[2] = 0XB1;
printf("\n sWork : %s",sWork);

memcpy(sWork, argv[1],strlen(argv[1]));

nLen = AsciiToHex(sWork, sDest, strlen(argv[1]));
printf("\n Dest : ");
for(i=0;i<nLen;i++) printf(" %02X",sDest[i]);
printf("\n");
}
