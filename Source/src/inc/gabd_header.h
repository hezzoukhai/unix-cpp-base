#ifndef RM_HEADERD_H
#define RM_HEADERD_H

#define TR_NAME_LEN 12

typedef struct s_headerD 
{
   /*char tr_name[TR_NAME_LEN];*/
   char lmac             [4];
   char no_gab           [6];
   char no_msg           [4];
   char date_gab         [6];
   char heure_gab        [4];
   char no_enrg          [6];
   char code_op          [2];
}headerD_t;

typedef struct s_evenemnt  /* Pour enregistrer le evenement */
{
   char terminal         [7];
   char code_op          [2];
   char result_op        [3];
   char evenement_1      [5];
   char evenement_2      [5];
   char evenement_3      [5];
   char evenement_4      [5];
   char date_gab         [7];
   char heure_gab        [5];
}evenement_t;

#endif
