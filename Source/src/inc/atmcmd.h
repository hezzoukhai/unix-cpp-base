  /***********************************************************************
  **  nom	: atmcmd.h						**
  **  desc	: gestion des commandes operateur pour un GAB		**
  ** auteur	: Younes OUADI (02 Avr 1999)				**
  **			(c) 1999 Hightech Payment Systems		**
  ***********************************************************************/

#ifndef HPS_ATMCMD_H
#define HPS_ATMCMD_H

typedef struct atm_command_t
{
    char	fTerminalNumber	[16];
    char    fAcqBank [6+1];/*ICH270715*/
    int		fCmdSeq;
    char	fCommand	[31];
    char	fStatus		[ 2];
    char	fRequestDate	[13];
    char	fProcessingDate	[13];
    char	fPurgeDate	[13];
} atm_command;

/* - definition des constatntes valeurs de fStatus -------------------------- */
#define	AC_NOT_PROCESSED		"0"
#define	AC_IN_PROGRESS			"1"
#define	AC_SUCCES_PROCESS		"2"
#define	AC_UNSUCCES_PROCESS		"3"
#define	AC_CANCELED			    "4"

#endif /*** HPS_ATMCMD_H ***/
