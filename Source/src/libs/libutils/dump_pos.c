/***************************************************************************/
/*            UTILS_ISO.H  : UTILITAIRE POUR COPIER ET AFFICHER            */
/*                           MESSAGE  ISO 8583  POS    (*F.M*)            */
/***************************************************************************/
#include <stdio.h>
#include <define.h>
#include <iso_pos.h>

#define SUCCES  0
#define ECHEC  -1


void affiche_message_iso_POS( char *data )
{
   char STRING_FIELD[512];
   char sub_data[512];
   char TI[2];
   int nLength;
   int i,j,k;
   TSPosInfo msgInfo;


	InitPosInfo(&msgInfo);

  AnalysePos  ( data , &msgInfo);
 fprintf(stderr,"\n-----------------------------------");
 fprintf(stderr,"\n- M.T.I      : [%04d]" , msgInfo.nMsgType );
 fprintf(stderr,"\n-FLD (FIELD): CONTENT             \n");
 fprintf(stderr, "-----------------------------------");

 for( i = 1 ; i < 127 ; i++)
 {
    switch (i)
    {
    default :
        if ( GetPosField(i, &msgInfo, data, &nLength ) == SUCCES )
          fprintf(stderr,"\n- FLD (%03d) : [%s]" , i+1 , data );
       break;
    }
 }
 fprintf(stderr, "\n-----------------------------------\n");
}

