  /***********************************************************************
  **  nom	: donation.h						**
  **  desc	: fichier include pour la gestion des donation		**
  **			(C) 1998 Hightech Payment Systems		**
  ***********************************************************************/

#ifndef HPS_DONATION_H
#define HPS_DONATION_H
/* definition de la structur donation_s */
char sDisplayStart  [ 1 + 1 ]; /* L-Left, R-Right */
char sDisplayOnATM  [ 1 + 1 ]; /* Y- Yes, N-No*/
typedef struct donation_t{
	char		donationNumber		[ 25];
	char		donationAraWording	[ 31];
	char		donationEngWording	[ 31];
	int         donationAvailable;
} donation_s;

typedef struct sub_donation_t{
	char		donationNumber			[ 25];
	char		subDonationNumber		[ 25];	
	char		subDonationAraWording	[ 31];
	char		subDonationEngWording	[ 31];
	char		subDonationAccount		[ 25];
	int         subDonationAvailable;
} sub_donation_s;

#define	MAX_DONATION		7
#define	NO					0
#define	YES					1

#define OC_DONATION_1			'I'
#define OC_DONATION_2			'H'
#define OC_DONATION_3			'G'
#define OC_DONATION_4			'F'
#define OC_DONATION_5			'A'
#define OC_DONATION_6			'B'
#define OC_DONATION_7			'C'
#define OC_DONATION_8			'D'

#endif /*** HPS_donation_H ***/
