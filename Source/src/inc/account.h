  /***********************************************************************
  **  nom	: account.h						**
  **  desc	: fichier include pour la gestion des comptes		**
  **  auteur	: Younes OUADI (24 Oct 1998)				**
  **			(C) 1998 Hightech Payment Systems		**
  ***********************************************************************/

#ifndef HPS_ACCOUNT_H
#define HPS_ACCOUNT_H
/*** definition de la structur account_s */
typedef struct account_t{
    char		accountNumber	[ 25];
    char		accountWording	[ 31];
    char		accountType	[  3];
    char		accountCurrency	[  4];
    int			accountAvailable;
} account_s;

#define	MAX_ACCOUNT			6
#define	NO					0
#define	YES					1

#define SOURCE_ACCOUNT			1
#define TARGET_ACCOUNT			2

#endif /*** HPS_ACCOUNT_H ***/
