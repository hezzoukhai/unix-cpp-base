/************************************************************************************/
/*  (c) Hightech Payment Systems 2006                                               */
/*  NAME                                                                       	    */
/*      iccbuild_buf_header.c                                                       */
/*                                                                             	    */
/* FUNCTION                                                                         */
/*         Management Buffer header qui peut etre Specifique (file card production) */
/* NOTES                                                                            */
/*                                                                                  */
/* INITIAL VERSION                                                                  */
/*        Y.LAMRANI   17/07/2006 -         V 2.2.0                                  */
/************************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include <iso_iccprod.h>
#include <tools.h>
#include <define.h>
/*#include <emv_check_fields.h>**Linux**/
#include <emv_header_len.h>

/*
nLenH1    5  Longeur en decimal du seperateur entre Mag data et Chip Data
nLenH2	  7  c'est la longeur en decimal de la longeur du reste(en general c'est fixe)
nLenH3    4  Longeur de l'AID (en general c'est fixe)
nLenH4    2  Longeur en Hex de la longeur du reste (en general c'est fixe)
nLenH5   12  Longeur du (Bin+Padding+KeK Version)
nLenH6    2  Longeur en Hex de la longeur du reste (en general c'est fixe)
*/

int BuildIccBufHead (char  *sIccBuffer, int nLen, char *BufHeader, char *BufTrailer , int BufHeaderHexLen, char *szBufferOut, int *nLengthOut)
{
   char  	sLine[MAX_LINE_TRC];
   char    	szBufferOut1[LG_MAX];
   char         szBufferOuttemp[8 + 1];
   int          LenTemp;

       

    memcpy(szBufferOut1, sIccBuffer, nLen);
    memcpy(szBufferOut , BufHeader , BufHeaderHexLen + nLenH1+nLenH2);
    memcpy(szBufferOut+ BufHeaderHexLen+ nLenH1+nLenH2 ,szBufferOut1 , nLen );
    

    memcpy(szBufferOut+ BufHeaderHexLen+ nLenH1+nLenH2+ nLen, BufTrailer, strlen(BufTrailer));
    *nLengthOut = nLen + BufHeaderHexLen+ nLenH1+nLenH2+ strlen(BufTrailer);
  
    /*Longeur du 1er reste decimal */
    memset(szBufferOuttemp, 0,sizeof(szBufferOuttemp));
    sprintf(szBufferOuttemp, "%.7d",nLen+BufHeaderHexLen);
    memcpy(szBufferOut+nLenH1,szBufferOuttemp ,nLenH2);
   
    /*Recuperation du 2eme rest Hex */
    memset(szBufferOuttemp , 0      , sizeof(szBufferOuttemp));
    sprintf(szBufferOuttemp, "%.4X" , nLen+BufHeaderHexLen -nLenH3-nLenH4 ); 

    LenTemp = AsciiToHex(szBufferOuttemp , szBufferOut+ nLenH1+nLenH2+nLenH3, 4);

    /*Recuperation du 3eme reste Hex */
    memset(szBufferOuttemp, 0,sizeof(szBufferOuttemp)); 
    sprintf(szBufferOuttemp, "%.4X",nLen+BufHeaderHexLen -nLenH3-nLenH4-nLenH5-nLenH6);
    LenTemp = AsciiToHex(szBufferOuttemp , szBufferOut+ nLenH1+nLenH2+nLenH3+nLenH4+nLenH5 , 4);
	
	/*SKO290509*/ return(0);
}

int BuildIccBufHeadCi (char  *sIccBuffer, int nLen, char *BufHeader, char *BufTrailer , int BufHeaderHexLen, char *szBufferOut, int *nLengthOut)
{
   char  	sLine[MAX_LINE_TRC];
   char    	szBufferOut1[LG_MAX];
   char         szBufferOuttemp[8 + 1];
   int          LenTemp;

       

    memcpy(szBufferOut1, sIccBuffer, nLen);
    memcpy(szBufferOut , BufHeader , nLenH1+nLenH2);
    memcpy(szBufferOut+ nLenH1+nLenH2 ,szBufferOut1 , nLen );
    

    memcpy(szBufferOut+ nLenH1+nLenH2+ nLen, BufTrailer, strlen(BufTrailer));
    *nLengthOut = nLen + nLenH1+nLenH2+ strlen(BufTrailer);
  
    /*Longeur du 1er reste decimal */
    memset(szBufferOuttemp, 0,sizeof(szBufferOuttemp));
    sprintf(szBufferOuttemp, "%.7d",nLen);
    memcpy(szBufferOut+nLenH1,szBufferOuttemp ,nLenH2);
   
    /*Recuperation du 2eme rest Hex */
 /*   memset(szBufferOuttemp , 0      , sizeof(szBufferOuttemp));
    sprintf(szBufferOuttemp, "%.4X" , nLen+BufHeaderHexLen -nLenH3-nLenH4 );  */

    /*LenTemp = AsciiToHex(szBufferOuttemp , szBufferOut+ nLenH1+nLenH2+nLenH3, 4);  */

    /*Recuperation du 3eme reste Hex */
   /* memset(szBufferOuttemp, 0,sizeof(szBufferOuttemp)); 
    sprintf(szBufferOuttemp, "%.4X",nLen+BufHeaderHexLen -nLenH3-nLenH4-nLenH5-nLenH6);  
    LenTemp = AsciiToHex(szBufferOuttemp , szBufferOut+ nLenH1+nLenH2+nLenH3+nLenH4+nLenH5 , 4);*/
	
	/*SKO290509*/ return(0);
}


