#ifndef RM_STRUCT_LOC_H
#define RM_STRUCT_LOC_H
typedef struct evt10_s
{
   char    heure     [4];
   char    cd_retour [1];
}evt10_t;

typedef struct evt11_s
{
   char    date      [6];
   char    cd_retour [1];
}evt11_t;

typedef struct evt20_s
{
   char    num_carte [ 21];
   char    cd_retour [  1];
}evt20_t;

typedef struct evt49_s
{
   char    cd_retour [  1];
}evt49_t;

typedef struct evt50_s
{
   char    total_cof       [  7];
   char    nb_bill         [  4];
   char    cd_bill         [  1];
   char    num_cass        [  1];
   char    cd_retour       [  1];
}evt50_t;

typedef struct evt51_s
{
   char    total_cof       [  7];
   char    nb_bill         [  4];
   char    reserved        [  4];
   char    cd_bill         [  1];
   char    nb_bill_dist    [  4];
   char    num_cass        [  1];
   char    cd_retour       [  1];
}evt51_t;

typedef struct evt52_s
{
   char    reserved        [  3];
   char    nb_dcheque      [  3];
   char    mt_cheque       [ 11];
   char    nb_despece      [  3];
   char    mt_espece       [ 11];
   char    cd_retour       [  1];
}evt52_t;

typedef struct evt54_s
{
   char    nb_retrt        [  4];
   char    mt_retrt        [ 11];
   char    nb_virmt        [  4];
   char    mt_virmt        [ 11];
   char    nb_desp         [  4];
   char    mt_desp         [ 11];
   char    nb_dcheque      [  4];
   char    mt_dcheque      [ 11];
   char    nb_hist         [  4];
   char    nb_imprib       [  4];
   char    nb_solde        [  4];
   char    nb_demchq       [  4];
   char    reserved        [ 16];
   char    cd_retour       [  1];
}evt54_t;

typedef struct evt5C_s
{
   char    cd_service      [  3];
   char    num_id          [ 21];
   char    result          [  1];
   char    categ_compte    [  1];
   char    num_compte      [ 11];
   char    mt_depot        [ 11];
   char    date_depot      [  6];
   char    heure_depot     [  4];
   char    num_enrg        [  6];
}evt5C_t;

typedef struct evt71_s
{
   char    cd_retour       [  1];
}evt71_t;

typedef struct evt79_s
{
   char    type_cle        [  1];
   char    indice          [  1];
   char    bank_id         [  6];
   char    indice_cle      [  4];
   char    cd_retour       [  1];
}evt79_t;

typedef struct evt85_s
{
   char    sec_param       [  4];
   char    cd_retour       [  1];
}evt85_t;

#endif /* RM_STRUCT_LOC_H */
