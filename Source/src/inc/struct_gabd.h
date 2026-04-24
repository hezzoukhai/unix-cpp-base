#ifndef RM_STRUCT_GABD_H
#define RM_STRUCT_GABD_H
struct gabdem_s
{
   char           type           [  4+1];
   char           num_msg        [  4+1];
   char           num_gab        [  6+1];
   char           num_carte      [ 21+1];
   char           cd_trans       [  2+1];
   char           piste_2        [ 37+1];
   char           piste_3        [104+1];
   char           pin_block      [ 16+1];
   char           num_cle        [  1+1];
   char           indice_cle     [  4+1];
   char           mt_demande     [ 12+1];
   char           num_retrait    [  6+1];
   char           date_demande   [  4+1];
   char           heure_demande  [  6+1];
   char           num_auto       [  6+1];
   char           cd_reponse     [  2+1]; /* Annulation   : code reponse GAB
                                           Confirmation : code reponse recu */
   char           flag_dist      [  1+1]; /* champ pour compte rendu */
   char           mt_retrait     [  7+1]; /* champ pour compte rendu */
   char           ecr_carte      [  1+1]; /* champ pour compte rendu */
};

struct gabrep_s
{
   char           type           [  4];
   char           num_carte      [ 21];
   char           cd_reponse     [  2];
   char           pin_block      [ 16]; /* Champ d'authentif En cas de retrait*/
   char           num_auto       [  6];
   char           solde_jour     [ 12]; /* Montant Max en cas cd_rep = 61*/
   char           nb_compte      [  1];
   char           inf_compte     [304];
};

typedef struct infcmpt_s
{
   char           categ_compte   [  1]; /* Message d'interrogation       */
   char           num_compte     [ 11]; /* Message d'interrogation       */
   char           solde_compte   [ 12]; /* Message d'interrogation       */
   char           plafond        [  7]; /* Message d'interrogation       */
   char           plancher       [  7]; /* Message d'interrogation       */
}infcmpt_t;

typedef struct logmes_s
{
   int    length;
   char   code_op[2];
   char   buf_data[512];
} logmes_t;
#endif /* RM_STRUCT_GABD_H */
