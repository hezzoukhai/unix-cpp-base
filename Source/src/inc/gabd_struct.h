#ifndef RM_STRUCTD_H
#define RM_STRUCTD_H

#define	STATUS_ERR 	3

typedef struct s_riguich_head
{
   char num_carte      [21];
   char code_action    [ 1];
   char code_affiche   [ 1];
   char solde_jour     [ 4];
   char reserved       [ 5];
   char nb_compte      [ 1];
}riguich_head_t;

typedef struct s_rmess_trail
{
   char message_clt1   [40];
   char message_clt2   [40];
}rmess_trail_t;

typedef struct s_riguich_body
{
   char categ_compte   [ 1];
   char num_compte     [11];
   char solde          [12];
   char plancher       [ 7];
   char plafond        [ 7];
   char op_autorise    [ 5];
}riguich_body_t;

typedef struct s_igiso23
{
   char piste_iso2     [ 37];
   char piste_iso3     [104];
   char lg_pin         [  1];
   char pin_block      [ 16];
   char num_cle        [  1];
   char indice_cle     [  4];
} igiso23_t;

typedef struct s_mesiso2
{
   char piste_iso2     [ 37];
   char lg_pin         [  1];
   char pin_block      [ 16];
   char mt_demande     [ 12]; /* initialise a 0 dans le cas interrogation */
   char trans_id       [  6]; /* initialise a 0 dans le cas interrogation */
   char num_cle        [  1];
   char indice_cle     [  4];
/* - depand de version GABD --
   char num_retrait    [  4];
   char num_seq        [  2];
   char code_lang      [  1];
--------------------------- */
} mesiso2_t;


typedef struct s_mesvir
{
   char piste_iso2     [ 37];
   char lg_pin         [  1];
   char pin_block      [ 16];
   char trans_id       [  6]; /* initialise a 0 dans le cas interrogation */
   char num_cle        [  1];
   char indice_cle     [  4];
   char compt1         [ 11];
   char compt2	       [ 6];
   char mt_demande     [ 12];
} mesvir_t;

typedef struct s_retrep
{
   char num_auto       [  6];
   char code_decision  [  2];
   char mt_max         [ 12];
   char nb_alea        [  6]; /* initialise a 0 dans le cas format 0 */
   char code_affiche   [  1];
} retrep_t;

typedef struct s_retiso23
{
   char piste_iso2     [ 37];
   char piste_iso3     [104];
   char lg_pin         [  1];
   char pin_block      [ 16];
   char mt_demande     [ 12]; 
   char trans_id       [  6];
   char num_cle        [  1];
   char indice_cle     [  4];
} retiso23_t;

typedef struct s_anuliso2
{
   char piste_iso2       [ 37];
   char mt_demande       [ 12];
   char num_auto         [  6];
   char cd_rep_gab       [  2];
   char zone_chiffre     [ 16];
   char trans_id         [  6];
   char trans_date_heure [ 10];
   char num_cle          [  1];
   char indice_cle       [  4];
}anuliso2_t;


typedef struct s_mini_releve
{
   char carte_id       [ 21];
   char comte_id       [ 11];
}mini_releve_t;


typedef struct s_anuliso23
{
   char piste_iso2       [ 37];
   char piste_iso3       [104];
   char mt_demande       [ 12];
   char num_auto         [  6];
   char cd_rep_gab       [  2];
   char trans_id         [  6];
   char trans_date_heure [ 10];
}anuliso23_t;

typedef struct s_reponse
{
   char cd_reponse       [  2];
}reponse_t;

typedef struct s_fintr
{
   char num_carte        [ 21];
}fintr_t;

typedef struct s_rfintr
{
   char CHSP             [  4];
   char donne            [329];
}repfintr_t;

typedef struct gabd_param_t
  { int 	len;
    char    data[LG_MAX];
  } gabd_param;

typedef struct SEventGabdT{
    char	*Value;
    char	*EventId;
    char	*fLabel;
    char	*aLabel;
} SEventGabd;

#endif
