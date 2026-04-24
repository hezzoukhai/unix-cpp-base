#include <stdio.h>
#include <stdlib.h>

unsigned char parity_table[]={
  0x01, 0x01, 0x02, 0x02, 0x04, 0x04, 0x07, 0x07, 0x08, 0x08, 0x0B, 0x0B, 0x0D, 0x0D, 0x0E, 0x0E
, 0x10, 0x10, 0x13, 0x13, 0x15, 0x15, 0x16, 0x16, 0x19, 0x19, 0x1A, 0x1A, 0x1C, 0x1C, 0x1F, 0x1F
, 0x20, 0x20, 0x23, 0x23, 0x25, 0x25, 0x26, 0x26, 0x29, 0x29, 0x2A, 0x2A, 0x2C, 0x2C, 0x2F, 0x2F
, 0x31, 0x31, 0x32, 0x32, 0x34, 0x34, 0x37, 0x37, 0x38, 0x38, 0x3B, 0x3B, 0x3D, 0x3D, 0x3E, 0x3E
, 0x40, 0x40, 0x43, 0x43, 0x45, 0x45, 0x46, 0x46, 0x49, 0x49, 0x4A, 0x4A, 0x4C, 0x4C, 0x4F, 0x4F
, 0x51, 0x51, 0x52, 0x52, 0x54, 0x54, 0x57, 0x57, 0x58, 0x57, 0x5B, 0x5B, 0x5D, 0x5D, 0x5E, 0x5E
, 0x61, 0x61, 0x62, 0x62, 0x64, 0x64, 0x67, 0x67, 0x68, 0x68, 0x6B, 0x6B, 0x6D, 0x6D, 0x6E, 0x6E
, 0x70, 0x70, 0x73, 0x73, 0x75, 0x75, 0x76, 0x76, 0x79, 0x79, 0x7A, 0x7A, 0x7C, 0x7C, 0x7F, 0x7F
, 0x80, 0x80, 0x83, 0x83, 0x85, 0x85, 0x86, 0x86, 0x89, 0x89, 0x8A, 0x8A, 0x8C, 0x8C, 0x8F, 0x8F
, 0x91, 0x91, 0x92, 0x92, 0x94, 0x94, 0x97, 0x97, 0x98, 0x98, 0x9B, 0x9B, 0x9D, 0x9D, 0x9E, 0x9E
, 0xA1, 0xA1, 0xA2, 0xA2, 0xA4, 0xA4, 0xA7, 0xA7, 0xA8, 0xA8, 0xAB, 0xAB, 0xAD, 0xAD, 0xAE, 0xAE
, 0xB0, 0xB0, 0xB3, 0xB3, 0xB5, 0xB5, 0xB6, 0xB6, 0xB9, 0xB9, 0xBA, 0xBA, 0xBC, 0xBC, 0xBF, 0xBF
, 0xC1, 0xC1, 0xC2, 0xC2, 0xC4, 0xC4, 0xC7, 0xC7, 0xC8, 0xC8, 0xCB, 0xCB, 0xCD, 0xCD, 0xCE, 0xCE
, 0xD0, 0xD0, 0xD3, 0xD3, 0xD5, 0xD5, 0xD6, 0xD6, 0xD9, 0xD9, 0xDA, 0xDA, 0xDC, 0xDC, 0xDF, 0xDF
, 0xE0, 0xE0, 0xE3, 0xE3, 0xE5, 0xE5, 0xE6, 0xE6, 0xE9, 0xE9, 0xEA, 0xEA, 0xEC, 0xEC, 0xEF, 0xEF
, 0xF1, 0xF1, 0xF2, 0xF2, 0xF4, 0xF4, 0xF7, 0xF7, 0xF8, 0xF8, 0xFB, 0xFB, 0xFD, 0xFD, 0xFE, 0xFE
};

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
          29 , 21 , 13 ,  5 , 28 , 20 , 12 ,  4 ,
           0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0    };

unsigned char Tab_KeyTr2[64]    = {
          14 , 17 , 11 , 24 ,  1 ,  5 ,  3 , 28 ,
          15 ,  6 , 21 , 10 , 23 , 19 , 12 ,  4 ,
          26 ,  8 , 16 ,  7 , 27 , 20 , 13 ,  2 ,
          41 , 52 , 31 , 37 , 47 , 55 , 30 , 40 ,
          51 , 45 , 33 , 48 , 44 , 49 , 39 , 56 ,
          34 , 53 , 46 , 42 , 50 , 36 , 29 , 32 ,
           0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
           0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0    };

unsigned char Tab_Etr[64]       = {
          32 ,  1 ,  2 ,  3 ,  4 ,  5 ,  4 ,  5 ,
           6 ,  7 ,  8 ,  9 ,  8 ,  9 , 10 , 11 ,
          12 , 13 , 12 , 13 , 14 , 15 , 16 , 17 ,
          16 , 17 , 18 , 19 , 20 , 21 , 20 , 21 ,
          22 , 23 , 24 , 25 , 24 , 25 , 26 , 27 ,
          28 , 29 , 28 , 29 , 30 , 31 , 32 ,  1 ,
           0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
           0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0    };

unsigned char Tab_Ptr[64]       = {
          16 ,  7 , 20 , 21 , 29 , 12 , 28 , 17 ,
           1 , 15 , 23 , 26 ,  5 , 18 , 31 , 10 ,
           2 ,  8 , 24 , 14 , 32 , 27 ,  3 ,  9 ,
          19 , 13 , 30 ,  6 , 22 , 11 ,  4 , 25 ,
           0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
           0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
           0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
           0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0    };

unsigned char Tab_S[512]        = {
          14 ,  4 , 13 ,  1 ,  2 , 15 , 11 ,  8 ,  3 , 10 ,
           6 , 12 ,  5 ,  9 ,  0 ,  7 ,  0 , 15 ,  7 ,  4 ,
          14 ,  2 , 13 ,  1 , 10 ,  6 , 12 , 11 ,  9 ,  5 ,
           3 ,  8 ,  4 ,  1 , 14 ,  8 , 13 ,  6 ,  2 , 11 ,
          15 , 12 ,  9 ,  7 ,  3 , 10 ,  5 ,  0 , 15 , 12 ,
           8 ,  2 ,  4 ,  9 ,  1 ,  7 ,  5 , 11 ,  3 , 14 ,
          10 ,  0 ,  6 , 13 , 15 ,  1 ,  8 , 14 ,  6 , 11 ,
           3 ,  4 ,  9 ,  7 ,  2 , 13 , 12 ,  0 ,  5 , 10 ,
           3 , 13 ,  4 ,  7 , 15 ,  2 ,  8 , 14 , 12 ,  0 ,
           1 , 10 ,  6 ,  9 , 11 ,  5 ,  0 , 14 ,  7 , 11 ,
          10 ,  4 , 13 ,  1 ,  5 ,  8 , 12 ,  6 ,  9 ,  3 ,
           2 , 15 , 13 ,  8 , 10 ,  1 ,  3 , 15 ,  4 ,  2 ,
          11 ,  6 ,  7 , 12 ,  0 ,  5 , 14 ,  9 , 10 ,  0 ,
           9 , 14 ,  6 ,  3 , 15 ,  5 ,  1 , 13 , 12 ,  7 ,
          11 ,  4 ,  2 ,  8 , 13 ,  7 ,  0 ,  9 ,  3 ,  4 ,
           6 , 10 ,  2 ,  8 ,  5 , 14 , 12 , 11 , 15 ,  1 ,
          13 ,  6 ,  4 ,  9 ,  8 , 15 ,  3 ,  0 , 11 ,  1 ,
           2 , 12 ,  5 , 10 , 14 ,  7 ,  1 , 10 , 13 ,  0 ,
           6 ,  9 ,  8 ,  7 ,  4 , 15 , 14 ,  3 , 11 ,  5 ,
           2 , 12 ,  7 , 13 , 14 ,  3 ,  0 ,  6 ,  9 , 10 ,
           1 ,  2 ,  8 ,  5 , 11 , 12 ,  4 , 15 , 13 ,  8 ,
          11 ,  5 ,  6 , 15 ,  0 ,  3 ,  4 ,  7 ,  2 , 12 ,
           1 , 10 , 14 ,  9 , 10 ,  6 ,  9 ,  0 , 12 , 11 ,
           7 , 13 , 15 ,  1 ,  3 , 14 ,  5 ,  2 ,  8 ,  4 ,
           3 , 15 ,  0 ,  6 , 10 ,  1 , 13 ,  8 ,  9 ,  4 ,
           5 , 11 , 12 ,  7 ,  2 , 14 ,  2 , 12 ,  4 ,  1 ,
           7 , 10 , 11 ,  6 ,  8 ,  5 ,  3 , 15 , 13 ,  0 ,
          14 ,  9 , 14 , 11 ,  2 , 12 ,  4 ,  7 , 13 ,  1 ,
           5 ,  0 , 15 , 10 ,  3 ,  9 ,  8 ,  6 ,  4 ,  2 ,
           1 , 11 , 10 , 13 ,  7 ,  8 , 15 ,  9 , 12 ,  5 ,
           6 ,  3 ,  0 , 14 , 11 ,  8 , 12 ,  7 ,  1 , 14 ,
           2 , 13 ,  6 , 15 ,  0 ,  9 , 10 ,  4 ,  5 ,  3 ,
          12 ,  1 , 10 , 15 ,  9 ,  2 ,  6 ,  8 ,  0 , 13 ,
           3 ,  4 , 14 ,  7 ,  5 , 11 , 10 , 15 ,  4 ,  2 ,
           7 , 12 ,  9 ,  5 ,  6 ,  1 , 13 , 14 ,  0 , 11 ,
           3 ,  8 ,  9 , 14 , 15 ,  5 ,  2 ,  8 , 12 ,  3 ,
           7 ,  0 ,  4 , 10 ,  1 , 13 , 11 ,  6 ,  4 ,  3 ,
           2 , 12 ,  9 ,  5 , 15 , 10 , 11 , 14 ,  1 ,  7 ,
           6 ,  0 ,  8 , 13 ,  4 , 11 ,  2 , 14 , 15 ,  0 ,
           8 , 13 ,  3 , 12 ,  9 ,  7 ,  5 , 10 ,  6 ,  1 ,
          13 ,  0 , 11 ,  7 ,  4 ,  9 ,  1 , 10 , 14 ,  3 ,
           5 , 12 ,  2 , 15 ,  8 ,  6 ,  1 ,  4 , 11 , 13 ,
          12 ,  3 ,  7 , 14 , 10 , 15 ,  6 ,  8 ,  0 ,  5 ,
           9 ,  2 ,  6 , 11 , 13 ,  8 ,  1 ,  4 , 10 ,  7 ,
           9 ,  5 ,  0 , 15 , 14 ,  2 ,  3 , 12 , 13 ,  2 ,
           8 ,  4 ,  6 , 15 , 11 ,  1 , 10 ,  9 ,  3 , 14 ,
           5 ,  0 , 12 ,  7 ,  1 , 15 , 13 ,  8 , 10 ,  3 ,
           7 ,  4 , 12 ,  5 ,  6 , 11 ,  0 , 14 ,  9 ,  2 ,
           7 , 11 ,  4 ,  1 ,  9 , 12 , 14 ,  2 ,  0 ,  6 ,
          10 , 13 , 15 ,  3 ,  5 ,  8 ,  2 ,  1 , 14 ,  7 ,
           4 , 10 ,  8 , 13 , 15 , 12 ,  9 ,  0 ,  3 ,  5 ,
           6 , 11                                            };

unsigned char Tab_Rots[16] = {
           1 ,  1 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,
           1 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  1    };


unsigned char S[8][64];

unsigned char Rots[16+1];

unsigned char I_Key[16][64];

unsigned char Rn[32+1], Ln[32+1];


/**********************************************************************/
/*       odd(x) donne 1 si son argument est impair et 0 sinon         */
/**********************************************************************/

int odd(x)
int x;
{
  return(x & 1);
}

/**********************************************************************/
/*       ByteBin(c,s) remplit le deuxieme argument qui est une chaine */
/*       par la representation binaire du premier argument            */
/**********************************************************************/

ByteBin(c,s)
unsigned char c;
unsigned char s[9];
{
  unsigned char i;

  s[8] = 0;
  for(i=0;i<8;++i)
  {
    s[7-i] = (c & 1) + 48;
    c=(c >> 1);
  }
  return(0);
}

/**********************************************************************/
/*       BinByte(s) convertit en decimal la chaine de caracteres s    */
/*                  representant un nombre binaire                    */
/**********************************************************************/

unsigned char BinByte(s)
unsigned char s[9];
{
  unsigned char i, r=0;

  for(i=0;i<8;++i)
  {
    r=2*r+(s[i]-48);
  }
  return(r);
}

/**********************************************************************/
/*       pos(s,c) donne la premiere occurence du caractere c          */
/*                dans la chaine de caracteres s                      */
/**********************************************************************/

unsigned char pos(s,c)
unsigned char s[];
unsigned char c;
{
  unsigned char i;

  i=0;
  while ((s[i] != c) && (s[i] != '\0') && (i<255))
  {
    i++;
  }
  return(i);
}

/**********************************************************************/
/*       HexaByte(s) convertit en decimal la chaine de caracteres s   */
/*                  representant un nombre hexadecimal                */
/**********************************************************************/

unsigned char HexaByte(s)
unsigned char s[3];
{
unsigned char Hexa[17];

  strcpy( (char *)Hexa, "0123456789ABCDEF" );
  return( (unsigned char) 16 * ( pos(Hexa,s[0]) ) + ( pos(Hexa,s[1]) ) );
}

/**********************************************************************/

Transpose ( Data, T, n )
unsigned char Data[64+1];
unsigned char T[64+1];
int n;
{
unsigned char X[64+1];
int i; 

  memcpy( X , Data , 64 );
  for(i=0;i<n;++i) Data[i] = X[T[i]-1];
  return(0);
}

/**********************************************************************/

Rotate ( Key )
unsigned char Key[64+1];
{
unsigned char i, x;

  x = Key[0];
  for(i=0;i<55;i++) Key[i] = Key[i+1];
  Key [55] = Key[27];
  Key [27] = x;
  return(0);
}

/**********************************************************************/

Calcul_I_Key ( xKey )
unsigned char xKey[64+1];
{
unsigned char Key[64+1];
unsigned char i, j;

  memcpy( Key, xKey, 64 );
  for(i=0;i<=15;++i)
  {
    for(j=1;j<=Rots[i];++j) Rotate( Key );
    memcpy( I_Key[i] , Key , 64 );
    Transpose ( I_Key[i], Tab_KeyTr2, 48 );
  }
  return(0);
}

/**********************************************************************/

Split ( Rn, Ln, A )
unsigned char Rn[32+1];
unsigned char Ln[32+1];
unsigned char A[64+1];
{
unsigned char i;

  for(i=0;i<32;++i)   Ln[i]    = A[i];
  for(i=32;i<64;++i)  Rn[i-32] = A[i];
  return(0);
}

/**********************************************************************/

Expand(A,Rn,Ln)
unsigned char Rn[32+1];
unsigned char Ln[32+1];
unsigned char A[64+1];
{
unsigned char i;

  for(i=0;i<32;++i)   A[i] = Ln[i];
  for(i=32;i<64;++i)  A[i] = Rn[i-32];
  return(0);
}

/**********************************************************************/

F ( i, RnLn, X )
int   i;
unsigned char RnLn[32+1];
unsigned char X[64+1];
{
  unsigned char E[64+1], IKey[64+1], Y[64+1], R, K, J, JJ;

  for(JJ=0;JJ<32;++JJ) E[JJ] = RnLn[JJ];
  Transpose ( E , Tab_Etr , 48 );
  memcpy( IKey , I_Key[i] , 64 );
  for(J=0;J<48;++J)  Y[J] = ( (E[J] + IKey[J]) == 1 );
  for(K=1;K<=8;++K)
  {
    R =    32 * Y [6 * K - 6]  +  16 * Y [6 * K - 1]
         +  8 * Y [6 * K - 5]  +   4 * Y [6 * K - 4]
         +  2 * Y [6 * K - 3]  +       Y [6 * K - 2]+1 ;
    X [4 * K - 4] = ( odd ( S [K-1][R-1] >> 3 ));
    X [4 * K - 3] = ( odd ( S [K-1][R-1] >> 2 ));
    X [4 * K - 2] = ( odd ( S [K-1][R-1] >> 1 ));
    X [4 * K - 1] = ( odd ( S [K-1][R-1] >> 0 ));
  }
  Transpose ( X, Tab_Ptr, 32 );
  return(0);
}

/**********************************************************************/

Binary ( VarDecR,  ArrayBinR )
unsigned char VarDecR[8+1];
unsigned char ArrayBinR[64+1];
{
unsigned char i;
unsigned char j;
unsigned char k;
unsigned char S[9];

  k = 0;
  for(i=0;i<8;++i)
  {
    ByteBin ( VarDecR[i], S );
    for(j=0;j<8;++j)
    {
      ArrayBinR[k] = S[j] - 48;
      ++k;
    }
  }
  return(0);
}

/**********************************************************************/

Binary_1 ( ArrayBinR , VarDecR )
unsigned char VarDecR[8+1];
unsigned char ArrayBinR[64+1];
{
unsigned char i, j, k, S[9];

  k = 0;
  for(i=0;i<8;++i)
  {
    S[8] = 0;
    for(j=0;j<8;++j)
    {
      S[j] = ArrayBinR[k] + 48;
      ++k;
    }
    VarDecR[i] = BinByte(S);
  }
  return(0);
}

/**********************************************************************/

HexaDecimal ( VarDecR , VarHexR )
unsigned char VarDecR[8+1];
unsigned char VarHexR[16+1];
{
unsigned char Arr[16+1], i, temp;

  strcpy( (char *)Arr, "0123456789ABCDEF" );
  for(i=0;i<8;++i)
  {
    VarHexR[i*2+0] = Arr[ VarDecR[i] >> 4]; /* Div */
    temp = VarDecR[i];
    VarHexR[i*2+1] = Arr[ temp-16*(temp >> 4) ]; /* Mod */
  }
  return(0);
}

/**********************************************************************/

HexaDecimal_1 ( VarHexR  , VarDecR )
unsigned char VarHexR[16+1];
unsigned char VarDecR[8+1];
{
unsigned char i, TempStr[3];

  for(i=0;i<8;++i)
  {
    TempStr[0] = VarHexR[i*2+0];
    TempStr[1] = VarHexR[i*2+1];
    TempStr[2] = '\0';
    VarDecR[i] = HexaByte ( TempStr );
  }
  return(0);
}

/**********************************************************************/

DES_Bin( Funct , PlainText , Key , CipherText )
int   Funct;
unsigned char PlainText[64+1];
unsigned char Key[64+1];
unsigned char CipherText[64+1];
{
  unsigned char cmpt, i, j, A[64+1],/* B[64+1], */ X[64+1], Buff[32+1];

  memcpy ( S , Tab_S , 512 );
  memcpy ( Rots , Tab_Rots , 16 );
  memcpy ( A , PlainText , 64 );
  Transpose ( A   , Tab_InitialTr , 64 );
  Transpose ( Key , Tab_KeyTr1 , 56 );
  Calcul_I_Key ( Key );
  Split        ( Rn , Ln , A );
  cmpt   = 0;
  if   (Funct == -1)
       i = 15;
  else i = 0;
  while (cmpt < 16)
  {
    memcpy( Buff , Rn , 32 );
    F ( (int)i , Rn , X );
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
  DES ( 1 , (unsigned char)'C' , Source , Key , Source );
  return(0);
}

/**********************************************************************/

Decrypt_hex ( Source , Key )
unsigned char Source[], Key[]; 
{
  DES ( -1 , (unsigned char)'H' , Source , Key , Source );
  return(0);
}

Encrypte_hex ( Source , Key )
unsigned char Source[], Key[]; 
{
  DES ( 1 , (unsigned char)'H' , Source , Key , Source );
  return(0);
}

/**********************************************************************/

Decrypte ( Source , Key )
unsigned char Source[], Key[]; 
{
  DES ( -1 , (unsigned char)'C' , Source , Key , Source );
  return(0);
}

/**********************************************************************/

KCV_hex ( Source )
unsigned char Source[];
{
unsigned char T_Key[16];

  memset ( T_Key , '0' , 16 );
  DES ( 1 , (unsigned char)'H' , Source , T_Key , Source );
  return(0);
}

/**********************************************************************/

KCV ( Source )
unsigned char Source[];
{
unsigned char T_Key[8];

  memset ( T_Key , 0 , 8 );
  DES ( 1 , (unsigned char)'C' , Source , T_Key , Source );
  return(0);
}

/**********************************************************************/

KCV_1 ( Source )
unsigned char *Source;
{
unsigned char T_Key[8];

  memset ( T_Key , 0 , 8 );
  DES ( -1 , (unsigned char)'C' , Source , T_Key , Source );
  return(0);
}

/**********************************************************************/


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

unsigned char Hex_To_Ascii(c)
char  c;
{
      if (c <= 0x09) return (c + 0x30);
      else
      {
         if (c == 0x0A) return ( 0x41);
         else if (c == 0x0B) return ( 0x42);
         else if (c == 0x0C) return ( 0x43);
         else if (c == 0x0D) return ( 0x44);
         else if (c == 0x0E) return ( 0x45);
         else if (c == 0x0F) return ( 0x46);
      }
     return(0x00);
}

void BCDUncompress (unsigned char *sSrc, unsigned char *sDst, int nLng)
{

   int  i = 0;
   int  j = 0;
   int   nExtraZero = 0;
   char sWork [256];

   memset(sWork, '\0', sizeof(sWork));

   for (i=0; i<nLng; i++){
      sWork[j++] = Hex_To_Ascii( (char)((sSrc[i] & 0xf0) >> 4)) ;
      sWork[j++] = Hex_To_Ascii( (char)(sSrc[i] & 0x0f ) ) ;
   }

   memcpy(sDst, sWork, j);

   return ;
}

void BCDCompress ( unsigned char *sSrc, unsigned char *sDst, int nLng)
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

   return ;
}

void fct_Adjust_Parity( unsigned char *pInput, unsigned char *pOutput)
{

   unsigned char vInputCompressed[256];
   unsigned char vOutputCompressed[256];
   int           i, len;

   memset(vInputCompressed, 0, sizeof(vInputCompressed));
   memset(vOutputCompressed, 0, sizeof(vOutputCompressed));

   len = strlen(pInput);

   BCDCompress(pInput, vInputCompressed, len);

   for(i=0; i<len/2; ++i)
      vOutputCompressed[i] = parity_table[vInputCompressed[i]];

   BCDUncompress(vOutputCompressed, pOutput, len/2);


}

void fct_KCV_calc ( unsigned char * pKeyIn,  unsigned char * pKCVout )
{
   unsigned char   key1  [17];
   unsigned char   key2  [17];
   unsigned char   key3  [17];
   unsigned char   output  [17];

   memset(key1, 0, sizeof(key1));
   memset(key2, 0, sizeof(key2));
   memset(key3, 0, sizeof(key3));
   memset(output, 0, sizeof(output));

   memcpy(key1, pKeyIn + 0, 16);
   memcpy(key2, pKeyIn + 0, 16);
   memcpy(key3, pKeyIn + 0, 16);

   if ( strlen(pKeyIn) > 16)
      memcpy(key2, pKeyIn +16, 16);

   if ( strlen(pKeyIn) > 32)
      memcpy(key3, pKeyIn +32, 16);

   DES(1, (unsigned char)'H', "0000000000000000", key1, output);
   DES(-1, (unsigned char)'H', output, key2, output);
   DES(1, (unsigned char)'H', output, key3, pKCVout);

}

void fct_3DES ( int flag, char *key, char *InputData, char *OutputData)
{

char  output1 [ 16 +1 ];
char  output2 [ 16 +1 ];
char  Data [ 16 +1 ];
char  key1 [ 16 +1 ];
char  key2 [ 16 +1 ];
char  key3 [ 16 +1 ];
int   i;

memset(output1, 0, sizeof(output1));
memset(output2, 0, sizeof(output2));
memset(key1, 0, sizeof(key1));
memset(key2, 0, sizeof(key2));
memset(key3, 0, sizeof(key3));

memcpy(key1, key, 16);
memcpy(key2, key, 16);
memcpy(key3, key, 16);

if(strlen(key)>16)
   memcpy(key2, key+16, 16);

if(strlen(key)>32)
   memcpy(key3, key+32, 16);

/** DES on The Input Data **/
        for (i=0; i<strlen(InputData); i+=16)
        {
           memcpy(Data, InputData +i, 16);
           DES(flag, (unsigned char)'H', Data, 1==flag?key1:key3, output1);
           DES(-1*flag, (unsigned char)'H', output1, key2, output2);
           DES(flag, (unsigned char)'H', output2, 1==flag?key3:key1, output1);
           memcpy(OutputData+i, output1, 16);
        }

}

/** Input Data Should be 16 Bytes in Hex Digits **/
void fct_XOR(unsigned char *Input1, unsigned char *Input2, unsigned char *Output)
{
        int i;
        unsigned char HexInput1[8];
        unsigned char HexInput2[8];
        unsigned char HexOutput[8];

        BCDCompress(Input1, HexInput1, 16);
        BCDCompress(Input2, HexInput2, 16);
        for(i=0;i<8;i++) *(HexOutput+i)=*(HexInput1+i)^*(HexInput2+i);
        BCDUncompress(HexOutput, Output, 8);
}
