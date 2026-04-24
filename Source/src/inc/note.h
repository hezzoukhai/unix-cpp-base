  /***********************************************************************
  **  nom	: note.h						**
  **  desc	: fichier header du module de distribution		**
  **  auteur	: Younes OUADI (19 Mars 1999)				**
  **			(C) 1999 : Hightech Payment Systems		**
  ***********************************************************************/
#ifndef HPS_NOTE_H
#define HPS_NOTE_H
typedef struct note_t
{
        int     fNumber;        /* note number                  */
        int     fValue;         /* note value                   */
        char    fDenomination;  /* note denomination            */
        int     fType;          /* note type                    */
        double  fPercent;       /* note number percent          */
        int     fDistribute;    /* distributed notes number     */
        char    fNoteNbr[3];    /* distributed notes number for */
                                /* special use in Reply         */
} note_s;
/* la nouvelle structure integre le fonctinalités DBL
typedef struct note_t
{
	int	fNumber;	
	int	fValue;	
	int	fType;
	double	fPercent;	
	int	fDistribute;
} note_s;
*/
#define	CASSETTES_NBR		4

#define MAX_NOTES               20


#define MIN_VAL_AMOUNT          5000    /* Ajout par Y.K le 31-01-00 : montant Min distribue par le GAB DBL en CFA */
#define MAX_VAL_AMOUNT          200000  /* Ajout par Y.K le 25-03-00 : montant Max distribue par le GAB DBL en CFA */


#endif /*** HPS_NOTE_H ***/
