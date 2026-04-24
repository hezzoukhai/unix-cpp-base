#include <define.h>

void    PcrdCompress(target, source)
unsigned char    *target;
unsigned char    *source;
{
   int    i;
   unsigned char   a;
   unsigned char   b;
   unsigned char   c;

   for(i = 0; i < 8; i++)
   {
      a = *(source + 2 * i);
      b = *(source + 2 * i + 1);
      c = 0x00;

      if (a == 0x41) a = 0x0A;
      else
         if (a == 0x42) a = 0x0B;
         else
            if (a == 0x43) a = 0x0C;
            else
               if (a == 0x44) a = 0x0D;
               else
                  if (a == 0x45) a = 0x0E;
                  else
                     if (a == 0x46) a = 0x0F;

      if (b == 0x41) b = 0x0A;
      else
         if (b == 0x42) b = 0x0B;
         else
            if (b == 0x43) b = 0x0C;
            else
               if (b == 0x44) b = 0x0D;
               else
                  if (b == 0x45) b = 0x0E;
                  else
                     if (b == 0x46) b = 0x0F;
      c = (unsigned char) ((a << 4) & 0xF0) | (b & 0x0F);
      *(target +  i)  = c;
   }
   return;
}


unsigned char Tab_InitialTr[64] = {
   58 , 50 , 42 , 34 , 26 , 18 , 10 ,  2,
   60 , 52 , 44 , 36 , 28 , 20 , 12 ,  4,
   62 , 54 , 46 , 38 , 30 , 22 , 14 ,  6,
   64 , 56 , 48 , 40 , 32 , 24 , 16 ,  8,
   57 , 49 , 41 , 33 , 25 , 17 ,  9 ,  1,
   59 , 51 , 43 , 35 , 27,  19 , 11 ,  3,
   61 , 53 , 45 , 37 , 29 , 21 , 13 ,  5,
   63 , 55 , 47 , 39 , 31 , 23 , 15 ,  7    };

unsigned char Tab_FinalTr[64]   =  {
   40 ,  8 , 48 , 16 , 56 , 24 , 64 , 32 ,
   39 ,  7 , 47 , 15 , 55 , 23 , 63 , 31 ,
   38 ,  6 , 46 , 14 , 54 , 22 , 62 , 30 ,
   37 ,  5 , 45 , 13 , 53 , 21 , 61 , 29 ,
   36 ,  4 , 44 , 12 , 52 , 20 , 60 , 28 ,
   35 ,  3 , 43 , 11 , 51 , 19 , 59 , 27 ,
   34 ,  2 , 42 , 10 , 50 , 18 , 58 , 26 ,
   33 ,  1 , 41 ,  9 , 49 , 17 , 57 , 25    };

unsigned char Tab_Swap[64]      = {
   33 , 34 , 35 , 36 , 37 , 38 , 39 , 40 ,
   41 , 42 , 43 , 44 , 45 , 46 , 47 , 48 ,
   49 , 50 , 51 , 52 , 53 , 54 , 55 , 56 ,
   57 , 58 , 59 , 60 , 61 , 62 , 63 , 64 ,
   1 ,  2 ,  3 ,  4 ,  5 ,  6 ,  7 ,  8 ,
   9 , 10 , 11 , 12 , 13 , 14 , 15 , 16 ,
   17 , 18 , 19 , 20 , 21 , 22 , 23 , 24 ,
   25 , 26 , 27 , 28 , 29 , 30 , 31 , 32    };

unsigned char Tab_KeyTr1[64]    = {
   57 , 49 , 41 , 33 , 25 , 17 ,  9 ,  1 ,
   58 , 50 , 42 , 34 , 26 , 18 , 10 ,  2 ,
   59 , 51 , 43 , 35 , 27 , 19 , 11 ,  3 ,
   60 , 52 , 44 , 36 , 63 , 55 , 47 , 39 ,
   31 , 23 , 15 ,  7 , 62 , 54 , 46 , 38 ,
   30 , 22 , 14 ,  6 , 61 , 53 , 45 , 37 ,
   29 , 21Key ( Key );
   Split        ( Rn , Ln , A );
   cmpt   = 0;
   if   (Funct == -1)
      i = 15;
   else i = 0;
   while (cmpt < 16)
   {
      memcpy( Buff , Rn , 32 );
      F ( i , Rn , X );
      for(j=0;j<32;++j) Rn[j] = ( (Ln[j] + X[j]) == 1 );
      memcpy( Ln , Buff , 32 );
      ++cmpt;
      if   (Funct == -1)
         --i;
      else ++i;
   }
   Expand    ( A , Rn , Ln );
   Transpose ( A , Tab_Swap , 64 );
   Transpose ( A , Tab_FinalTr , 64);
   memcpy( CipherText , A , 64 );
   return(0);
}

/**********************************************************************/

DES_Chr ( Funct , PlainText , Key , CipherText )
int   Funct;
unsigned char PlainText[8+1];
unsigned char Key[8+1];
unsigned char CipherText[8+1];
{
   unsigned char P[64+1], K[64+1], C[64+1];

   Binary    ( PlainText , P );
   Binary    ( Key       , K );
   DES_Bin   ( Funct, P , K , C );
   Binary_1  ( C , CipherText);
   return(0);
}

/**********************************************************************/

DES_Hex ( Funct , PlainText , Key , CipherText )
int  Funct;
unsigned char PlainText[16+1];
unsigned char Key[16+1];
unsigned char CipherText[16+1];
{

   unsigned char P[8+1], K[8+1], C[8+1];

   HexaDecimal_1 ( PlainText , P );

   HexaDecimal_1 ( Key       , K );
   DES_Chr       ( Funct , P , K , C );
   HexaDecimal   ( C , CipherText );
   return(0);
}

/**********************************************************************/

DES ( Funct , Format , PlainText , Key , CipherText )
/* ^ B = Binary      */
/*   H = Hexadecimal */
/*   C = Character   */
int   Funct;
unsigned char Format;
unsigned char PlainText[64+1];
unsigned char Key[64+1];
unsigned char CipherText[64+1];
{
   switch (Format)
   {
   case 'H' :
   case 'h' :
      DES_Hex  ( Funct , PlainText , Key , CipherText );
      break;
   case 'B' :
   case 'b' :
      DES_Bin  ( Funct , PlainText , Key , CipherText );
      break;
   default  :
      DES_Chr  ( Funct , PlainText , Key , CipherText );
   }


   return(0);
}

/**********************************************************************/

Encrypte ( Source , Key )
unsigned char Source[], Key[];
{
   DES ( 1 , 'C' , Source , Key , Source );
   return(0);
}

/**********************************************************************/

Decrypt_hex ( Source , Key )
unsigned char Source[], Key[];
{
   DES ( -1 , 'H' , Source , Key , Source );
   return(0);
}

Encrypte_hex ( Source , Key )
unsigned char Source[], Key[];
{
   DES ( 1 , 'H' , Source , Key , Source );
   return(0);
}

/**********************************************************************/

Decrypte ( Source , Key )
unsigned char Source[], Key[];
{
   DES ( -1 , 'C' , Source , Key , Source );
   return(0);
}

/**********************************************************************/

KCV_hex ( Source )
unsigned char Source[];
{
   unsigned char T_Key[16];

   memset ( T_Key , '0' , 16 );
   DES ( 1 , 'H' , Source , T_Key , Source );
   return(0);
}

/**********************************************************************/

KCV ( Source )
unsigned char Source[];
{
   unsigned char T_Key[8];

   memset ( T_Key , 0 , 8 );
   DES ( 1 , 'C' , Source , T_Key , Source );
   return(0);
}

/**********************************************************************/

KCV_1 ( Source )
unsigned char *Source;
{
   unsigned char T_Key[8];

   memset ( T_Key , 0 , 8 );
   DES ( -1 , 'C' , Source , T_Key , Source );
   return(0);
}

/**********************************************************************/
main(int argc, char **argv)
{

char sSource[17];
char sKey[17];

    memset(sSource, 0, sizeof(sSource));
    memset(sKey, 0, sizeof(sKey));
    memcpy(sSource, argv[1], strlen(argv[1]));
    memcpy(sKey, argv[2], strlen(argv[2]));
    Encrypte_hex ( sSource , sKey );
    /*Decrypt_hex ( sSource , sKey );*/

    printf(" Result is %s \n", sSource);
   

}


