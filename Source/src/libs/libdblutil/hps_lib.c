/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      hps_lib.c                                                              */
/*                                                                             */
/* Description                                                                 */
/*     Tools Management                                                        */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001MK     		18-02-2002   		Mahjoub KARROUM    		Initiale Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <define.h>
#include <ascii.h>


void    String_To_Octal(String, Length, Octal_String)
unsigned char    *String;
int     Length;
char    *Octal_String;
{
int i;

  for (i=0; i<Length; i++)
  {
        sprintf(Octal_String + (3 * i), "%03d", *(String + i));
  }
  return;
}
/******************************************************************************/
/* Fonction update_cassette_data.                                             */
/* ----------------------------                                               */
/* Description : Fonction qui charge met a jour les cassettes du gab.         */
/* Entree      : Le numero du terminal.                                       */
/*       Les information concernants les cassettes.                   */
/* Sortie      :                                               */
/* Revision    :                                                              */
/******************************************************************************/
int     update_cassette_data(nIndexCtx,term_nbr,pAcqBank/*ICH270715*/, cassettes_data)
int		nIndexCtx;
char    *term_nbr;
char    *pAcqBank;/*ICH270715*/
char    *cassettes_data;
{
   int   retour;
   int   offset;
   int   lg_buff;
   int   nbr_cassette;
   char   lg_rcv [4];

   char            V_cassette_1    [8];
   char            V_status_1      [2];
   char            V_note_type_1   [2];
   long             V_montant_1;

   char            V_cassette_2    [8];
   char            V_status_2      [2];
   char            V_note_type_2   [2];
   long             V_montant_2;

   char            V_cassette_3    [8];
   char            V_status_3      [2];
   char            V_note_type_3   [2];
   long             V_montant_3;

   char            V_cassette_4    [8];
   char            V_status_4      [2];
   char            V_note_type_4   [2];
   long             V_montant_4;

   char            tmp_amount_1    [20];
   char            tmp_amount_2    [20];
   char            tmp_amount_3    [20];
   char            tmp_amount_4    [20];

   trace_event("Start update_cassette_data()",PROCESSING);

   memset(lg_rcv,         0, sizeof(lg_rcv));

   memset(V_note_type_1,  0, sizeof(V_note_type_1));
   memset(V_note_type_2,  0, sizeof(V_note_type_2));
   memset(V_note_type_3,  0, sizeof(V_note_type_3));
   memset(V_note_type_4,  0, sizeof(V_note_type_4));
   memset(V_cassette_4,   0, sizeof(V_cassette_4));
   memset(V_cassette_2,   0, sizeof(V_cassette_2));
   memset(V_cassette_3,   0, sizeof(V_cassette_3));
   memset(V_cassette_4,   0, sizeof(V_cassette_4));
   memset(V_status_1,     0, sizeof(V_status_1));
   memset(V_status_2,     0, sizeof(V_status_2));
   memset(V_status_3,     0, sizeof(V_status_3));
   memset(V_status_4,     0, sizeof(V_status_4));

   memset(tmp_amount_1,   0, sizeof(tmp_amount_1));
   memset(tmp_amount_2,   0, sizeof(tmp_amount_2));
   memset(tmp_amount_3,   0, sizeof(tmp_amount_3));
   memset(tmp_amount_4,   0, sizeof(tmp_amount_4));

   memcpy(lg_rcv, cassettes_data, 3);
   offset = 3 + 3;
   lg_buff      = atoi(lg_rcv);
   nbr_cassette = (lg_buff - 3 ) / 16; 


   if ( nbr_cassette == 1 )
   {
      memcpy(tmp_amount_1, cassettes_data + offset, 9); 
      offset +=9;
      memcpy(tmp_amount_2, "000000000", 9);
      memcpy(tmp_amount_3, "000000000", 9);
      memcpy(tmp_amount_4, "000000000", 9);

      memcpy(V_cassette_1, cassettes_data + offset, 7); 
      offset +=7;
      memcpy(V_cassette_2, "0000000", 7);
      memcpy(V_cassette_3, "0000000", 7);
      memcpy(V_cassette_4, "0000000", 7);

      load_note_type(V_note_type_1, cassettes_data + offset); 
      memcpy(V_note_type_2, "0", 1);
      memcpy(V_note_type_3, "0", 1);
      memcpy(V_note_type_4, "0", 1);

      memcpy(V_status_1, "A", 1);
      memcpy(V_status_2, "N", 1);
      memcpy(V_status_3, "N", 1);
      memcpy(V_status_4, "N", 1);

      V_montant_1 = atoi(tmp_amount_1);
      if (V_montant_1 == 0) memcpy(V_status_1, "E", 1);

      V_montant_2 = atoi(tmp_amount_2);
      V_montant_3 = atoi(tmp_amount_3);
      V_montant_4 = atoi(tmp_amount_4);
   }
   else if ( nbr_cassette == 2 )
   {
      memcpy(tmp_amount_1, cassettes_data + offset, 9); 
      offset +=9;
      memcpy(tmp_amount_2, cassettes_data + offset, 9); 
      offset +=9;
      memcpy(tmp_amount_3, "000000000", 9);
      memcpy(tmp_amount_4, "000000000", 9);

      memcpy(V_cassette_1, cassettes_data + offset, 7); 
      offset +=7;
      memcpy(V_cassette_2, cassettes_data + offset, 7); 
      offset +=7;
      memcpy(V_cassette_3, "0000000", 7);
      memcpy(V_cassette_4, "0000000", 7);

      load_note_type(V_note_type_1, cassettes_data + offset);
      offset +=3;
      load_note_type(V_note_type_2, cassettes_data + offset);
      offset +=3;
      memcpy(V_note_type_3, "0", 1);
      memcpy(V_note_type_4, "0", 1);

      memcpy(V_status_1, "A", 1);
      memcpy(V_status_2, "A", 1);
      memcpy(V_status_3, "N", 1);
      memcpy(V_status_4, "N", 1);

      V_montant_1 = atoi(tmp_amount_1);
      if (V_montant_1 == 0) memcpy(V_status_1, "E", 1);

      V_montant_2 = atoi(tmp_amount_2);
      if (V_montant_2 == 0) memcpy(V_status_2, "E", 1);

      V_montant_3 = atoi(tmp_amount_3);
      V_montant_4 = atoi(tmp_amount_4);
   }
   else if ( nbr_cassette == 3)
   {
      memcpy(tmp_amount_1, cassettes_data + offset, 9); 
      offset +=9;
      memcpy(tmp_amount_2, cassettes_data + offset, 9); 
      offset +=9;
      memcpy(tmp_amount_3, cassettes_data + offset, 9); 
      offset +=9;
      memcpy(tmp_amount_4, "000000000", 9);

      memcpy(V_cassette_1, cassettes_data + offset, 7); 
      offset +=7;
      memcpy(V_cassette_2, cassettes_data + offset, 7); 
      offset +=7;
      memcpy(V_cassette_3, cassettes_data + offset, 7); 
      offset +=7;
      memcpy(V_cassette_4, "0000000", 7);

      load_note_type(V_note_type_1, cassettes_data + offset);
      offset +=3;
      load_note_type(V_note_type_2, cassettes_data + offset);
      offset +=3;
      load_note_type(V_note_type_3, cassettes_data + offset);
      offset +=3;
      memcpy(V_note_type_4, "0", 1);

      memcpy(V_status_1, "A", 1);
      memcpy(V_status_2, "A", 1);
      memcpy(V_status_3, "A", 1);
      memcpy(V_status_4, "N", 1);

      V_montant_1 = atoi(tmp_amount_1);
      if (V_montant_1 == 0) memcpy(V_status_1, "E", 1);

      V_montant_2 = atoi(tmp_amount_2);
      if (V_montant_2 == 0) memcpy(V_status_2, "E", 1);

      V_montant_3 = atoi(tmp_amount_3);
      if (V_montant_3 == 0) memcpy(V_status_3, "E", 1);

      V_montant_4 = atoi(tmp_amount_4);
   }
   else if ( nbr_cassette == 4)
   {
      memcpy(tmp_amount_1, cassettes_data + offset, 9); 
      offset +=9;
      memcpy(tmp_amount_2, cassettes_data + offset, 9); 
      offset +=9;
      memcpy(tmp_amount_3, cassettes_data + offset, 9); 
      offset +=9;
      memcpy(tmp_amount_4, cassettes_data + offset, 9); 
      offset +=9;

      memcpy(V_cassette_1, cassettes_data + offset, 7); 
      offset +=7;
      memcpy(V_cassette_2, cassettes_data + offset, 7); 
      offset +=7;
      memcpy(V_cassette_3, cassettes_data + offset, 7); 
      offset +=7;
      memcpy(V_cassette_4, cassettes_data + offset, 7); 
      offset +=7;

      load_note_type(V_note_type_1, cassettes_data + offset);
      offset +=3;
      load_note_type(V_note_type_2, cassettes_data + offset);
      offset +=3;
      load_note_type(V_note_type_3, cassettes_data + offset);
      offset +=3;
      load_note_type(V_note_type_4, cassettes_data + offset);
      offset +=3;

      memcpy(V_status_1, "A", 1);
      memcpy(V_status_2, "A", 1);
      memcpy(V_status_3, "A", 1);
      memcpy(V_status_4, "A", 1);

      V_montant_1 = atoi(tmp_amount_1);
      if (V_montant_1 == 0) memcpy(V_status_1, "E", 1);

      V_montant_2 = atoi(tmp_amount_2);
      if (V_montant_2 == 0) memcpy(V_status_2, "E", 1);

      V_montant_3 = atoi(tmp_amount_3);
      if (V_montant_3 == 0) memcpy(V_status_3, "E", 1);

      V_montant_4 = atoi(tmp_amount_4);
      if (V_montant_4 == 0) memcpy(V_status_4, "E", 1);
   }

   retour =  maj_cassette(nIndexCtx,
	   term_nbr,
	   pAcqBank/*ICH270715*/,
       V_cassette_1,
       V_status_1,
       V_note_type_1,
       V_montant_1,
       V_cassette_2,
       V_status_2,
       V_note_type_2,
       V_montant_2,
       V_cassette_3,
       V_status_3,
       V_note_type_3,
       V_montant_3,
       V_cassette_4,
       V_status_4,
       V_note_type_4,
       V_montant_4);

   if (retour != OK)
   {
      trace_event("maj_cassette Failed ", ERROR);
   }


   trace_event("End   update_cassette_data()",PROCESSING);
   return(retour);
}

/******************************************************************************/
/* Fonction update_compteur_cassettes                                        */
/* -------------------                           */
/* Description : Fonction qui permet la mise a jour des cassettes d'un        */
/*       terminal donnes suite a un retrait                          */
/*                                       */
/* Entree      : Le nummero du terminal                        */
/*              les donnees de compteurs des cassettes                       */
/* Sortie      :                                                              */
/* Revision    :                                                              */
/******************************************************************************/
int update_compteur_cassettes (nIndexCtx,V_terminal_number, V_cassettes_data)
int		nIndexCtx;
char   *V_terminal_number;
char   *V_cassettes_data;
{
   int   retour;
   int   nb_cassettes;
   char   ch_nb_cassettes[4];

   char   ch_nb1 [3];
   char   ch_nb2 [3];
   char   ch_nb3 [3];
   char   ch_nb4 [3];

   int   nb1 =0;
   int   nb2 =0;
   int   nb3 =0;
   int   nb4 =0;

   trace_event("Start update_compteur_cassettes",PROCESSING);

   memset(ch_nb1,          0, sizeof(ch_nb1));
   memset(ch_nb2,          0, sizeof(ch_nb2));
   memset(ch_nb3,          0, sizeof(ch_nb3));
   memset(ch_nb4,          0, sizeof(ch_nb4));
   memset(ch_nb_cassettes, 0, sizeof(ch_nb_cassettes));

   memcpy(ch_nb_cassettes, V_cassettes_data, 3);
   nb_cassettes    = (atoi(ch_nb_cassettes))/2;

   switch (nb_cassettes)
   {
   case   1 :   
      memcpy(ch_nb1, V_cassettes_data + 3, 2);
      nb1 = atoi(ch_nb1);
      break;
   case   2 :   
      memcpy(ch_nb1, V_cassettes_data + 3, 2);
      nb1 = atoi(ch_nb1);
      memcpy(ch_nb2, V_cassettes_data + 5, 2);
      nb2 = atoi(ch_nb2);
      break;
   case   3 :   
      memcpy(ch_nb1, V_cassettes_data + 3, 2);
      nb1 = atoi(ch_nb1);
      memcpy(ch_nb2, V_cassettes_data + 5, 2);
      nb2 = atoi(ch_nb2);
      memcpy(ch_nb3, V_cassettes_data + 7, 2);
      nb3 = atoi(ch_nb3);
      break;
   case   4 :   
      memcpy(ch_nb1, V_cassettes_data + 3, 2);
      nb1 = atoi(ch_nb1);
      memcpy(ch_nb2, V_cassettes_data + 5, 2);
      nb2 = atoi(ch_nb2);
      memcpy(ch_nb3, V_cassettes_data + 7, 2);
      nb3 = atoi(ch_nb3);
      memcpy(ch_nb4, V_cassettes_data + 9, 2);
      nb4 = atoi(ch_nb4);
      break;
   default   :   
      trace_event("ERREUR NOMBRE CASSETTES", ERROR);
      break;
   }

   retour = update_cassette(nIndexCtx,V_terminal_number, nb1, nb2, nb3, nb4);
   if (retour != OK)
   {
      trace_event("update_cassette Failed ", ERROR);
   }

   trace_event("End   update_compteur_cassettes",PROCESSING);
   return (retour);
}

/******************************************************************************/
/* Fonction load_trans_record.                                                */
/* ---------------------------                           */
/* Description : Fonction qui formate l'enregistrement des dix dernieres      */
/*       transactions pour un releve.                                 */
/* Entree      : Le buffer du message a envoyer au gab.                    */
/*              L'enregistrement des donnees recu de la base.                */
/* Sortie      : la longueur du buffer ecrit dans le message d'envoi.         */
/* Date/Auteur : 23/01/97 Y.KAOUTAR                                           */
/* Revision    :                                                              */
/******************************************************************************/
int   load_trans_record(target, source)
char    *target;
char    *source;
{
   char    tmp_record [LG_MAX];
   int     offset;
   int     i, j;
   int     nbr_enrg;
   char   tmp_lg [4];

   trace_event("Start load_trans_record()", PROCESSING);

   memset(tmp_lg    , 0, sizeof(tmp_lg));
   memset(tmp_record, 0, sizeof(tmp_record));
   offset = 0;
   j      = 0;

   memcpy(tmp_lg, source, 2);
   nbr_enrg = atoi(tmp_lg);

   if ( nbr_enrg > 0 )
   {
      if ( nbr_enrg > 8 )
      {
         j = j + ((nbr_enrg - 8) * 39);
         nbr_enrg = 8;
      }
      for (i = 0; i < nbr_enrg; i++)
      {
         memcpy(tmp_record + offset, source + 2 + j, 38); 
         offset += 38;
         if ( *(source + 2 + j + 38) == '+' )
         {
            memcpy(tmp_record + offset, "C", 1);
            offset ++;
         }
         else
         {
            memcpy(tmp_record + offset, "D", 1);
            offset ++;
         }


         j += 39;
      }

      sprintf(target,        "%03d", offset + 2);
      sprintf(target + 3,    "%02d", nbr_enrg);
      memcpy(target + 2 + 3, tmp_record, offset);
   }
   else
   {
      sprintf(target,        "%03d", offset + 2);
      sprintf(target + 3,    "%02d", nbr_enrg);
   }
   trace_event("End   load_trans_record()", PROCESSING);
   return(offset + 5);
}

/******************************************************************************/
/* Fonction load_account.                                                     */
/* ---------------------------                                                */
/* Description : Fonction qui charge le numero du compte dans le bufferr      */
/*               de la reponse a demande de releve.                           */
/* Entree      : Le buffer du message a envoyer au gab.                       */
/*               L'enregistrement des donnees recu de la base.                */
/* Sortie      : la longueur du buffer ecrit dans le message d'envoi.         */
/* Revision    :                                                              */
/******************************************************************************/
load_account(target, source)
char    *target;
char    *source;
{
   int     lg;
   char    tmp_lg [4];

   trace_event("Start load_account()", PROCESSING);

   memset(tmp_lg,      0, sizeof(tmp_lg));
   memcpy(tmp_lg, source, 3);

   lg = atoi(tmp_lg);

   memcpy(target, source, lg + 3);

   trace_event("End   load_account()", PROCESSING);
   return(lg + 3);

}


/******************************************************************************/
/* Fonction load_activate_key.                                                 */
/* ---------------------------                                                */
/* Description : Fonction qui charge le champ Active_Keys pour l'envoyer      */
/*               au GAB NCR                                                   */
/* Entree      : La position actuelle du pointeur sur le buffer d'envoi       */
/* Sortie      : la longueur de ce champ                                      */
/* Revision    :                                                              */
/******************************************************************************/
int load_activate_key(target)
char    *target;
{
   int lg=0;


   return(lg);
}


/******************************************************************************/
/******************************************************************************/
int       Load_soft_record_data(target, version, data_rec)
char    *target;
char    *version;
char    *data_rec;
{
   int             lg_block;
   int             rec_lg;
   char            tmp_lg [4];

   trace_event("Start Load_soft_record_data()",PROCESSING);

   memset(tmp_lg, 0, sizeof(tmp_lg));

   memcpy(tmp_lg, data_rec, 3);
   rec_lg = atoi(tmp_lg);

   lg_block     = rec_lg + 4 ;

   sprintf(target,    "%03d", lg_block);

   memcpy(target + 3,  version, 4);
   memcpy(target + 7,  data_rec + 3, rec_lg);

   trace_event("End   Load_soft_record_data()",PROCESSING);
   return(lg_block + 3);
}

/**********************************************************************/
int     load_note_type( target, note_data)
char    *target;
char    *note_data;
{
   char    buffer[5];

   memset(buffer, 0, sizeof(buffer));

   memcpy(buffer, note_data, 3);

   if( atoi(buffer) == 50)
      memcpy(target, "03", 2);
   else
      if( atoi(buffer) == 200)
         memcpy(target, "02", 2);
      else
         if( atoi(buffer) == 100)
            memcpy(target, "01", 2);

   return(OK);
}


