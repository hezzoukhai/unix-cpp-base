  /***********************************************************************
  **  nom	: utility.h						**
  **  desc	: fichier include pour la gestion des utility		**
  **			(C) 1998 Hightech Payment Systems		**
  ***********************************************************************/

#ifndef HPS_UTILITY_H
#define HPS_UTILITY_H
/* definition de la structur utility_s */
typedef struct utility_t{
    char		utilityNumber	[ 25];
    char		utilityWording	[ 31];
    int			utilityAvailable;
} utility_s;

#define	MAX_UTILITY			6

#define OC_UTILITY_1			'I'
#define OC_UTILITY_2			'H'
#define OC_UTILITY_3			'G'
#define OC_UTILITY_4			'F'
#define OC_UTILITY_5			'A'
#define OC_UTILITY_6			'B'
#define OC_UTILITY_7			'C'

#endif /*** HPS_UTILITY_H ***/
