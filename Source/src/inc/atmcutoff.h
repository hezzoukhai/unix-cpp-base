  /***********************************************************************
  **  nom	: atmcutoff.h						**
  **  desc	: gestion de l'arrete GAB				**
  **  auteur	: Younes UADI (10 Sep 1999)				**
  **		(C) 1997-2001 : Hightech Payment Systems		**
  ***********************************************************************/
#ifndef HPS_ATMCUTOFF_H
#define HPS_ATMCUTOFF_H

typedef struct SCurrDataT{
    char	fCurrCode	[ 3];
    char	fBCOAppTrnNbr	[ 6];
    char	fBCOAppTrnAmt	[15];
    char	fBCORevTrnNbr	[ 6];
    char	fBCORevTrnAmt	[15];
    char	fSAFAppTrnNbr	[ 6];
    char	fSAFAppTrnAmt	[15];
    char	fSAFRevTrnNbr	[ 6];
    char	fSAFRevTrnAmt	[15];
    char	fPRGAppTrnNbr	[ 6];
    char	fPRGAppTrnAmt	[15];
    char	fPRGRevTrnNbr	[ 6];
    char	fPRGRevTrnAmt	[15];
    char	fACOAppTrnNbr	[ 6];
    char	fACOAppTrnAmt	[15];
    char	fACORevTrnNbr	[ 6];
    char	fACORevTrnAmt	[15];
    char	fACOSuspTrnNbr	[ 6];
    char	fACOSuspTrnAmt	[15];
} SCurrData;

typedef struct SCutOffData{
    char	fCutOffId		[10];
    char	fCutOffDate		[14];
	char	fTermCurrDate	[14];
    char	fPickUpNbr		[ 6];
    char	fLowTrnNbr		[ 8];
    char	fHighTrnNbr		[ 8];
    SCurrData	fCurrData;
} SCutOffData;

#endif /*** HPS_ATMCUTOFF_H ***/
