  /***********************************************************************
  **  nom	: billing.h						**
  **  desc	: fichier include pour la gestion des comptes		**
  **  auteur	: Younes OUADI (24 Oct 1998)				**
  **			(C) 1998 Hightech Payment Systems		**
  ***********************************************************************/

#ifndef HPS_BILLING_H
#define HPS_BILLING_H
/* definition de la structur billing_s */
typedef struct billing_t{
    char		billingNumber	[ 25];
    char		billingWording	[ 31];
    char		billingDate		[  11];
    char		billingCurrency	[  4];
	double		billingAmount;
    int			billingAvailable;
} billing_s;

#define	MAX_BILLING			6
#define	NO					0
#define	YES					1

#define OC_BILLING_1			'I'
#define OC_BILLING_2			'H'
#define OC_BILLING_3			'G'
#define OC_BILLING_4			'F'
#define OC_BILLING_5			'A'
#define OC_BILLING_6			'B'
#define OC_BILLING_7			'C'
#endif /*** HPS_BILLING_H ***/
