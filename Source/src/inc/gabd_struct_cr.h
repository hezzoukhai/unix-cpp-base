#ifndef RM_STRUCT_CR_H
#define RM_STRUCT_CR_H
typedef struct crfixe_s
{
   char  num_cr           [ 6];
   char  cd_file          [ 1];
} crfixe_t;

typedef struct crhead_s
{
   char  num_cr           [ 6];
   char  cd_file          [ 1];
   char  cd_service       [ 3];
   char  num_carte        [21];
   char  result           [ 1];
   char  event1           [ 4];
   char  event2           [ 4];
   char  event3           [ 4];
   char  event4           [ 4];
} crhead_t;

typedef struct crhisto_s
{
   char  categ_compte     [ 1];
   char  num_compte       [11];
} crhisto_t;

typedef struct crcheckd_s
{
   char  categ_compte     [ 1];
   char  num_compte       [11];
   char  type_checque     [ 1];
   char  nb_checque       [ 2];
   char  type_remise      [ 1];
}crcheckd_t;

typedef struct crdepot_s
{
   char  categ_compte     [ 1];
   char  num_compte       [11];
   char  montant          [11];
}crdepot_t;

typedef struct crrib_s
{
   char  categ_compte     [ 1];
   char  num_compte       [11];
}crrib_t;

typedef struct crretrait2_s
{
   char  mt_retrait       [ 7];
   char  mt_demande       [ 7];
   char  mt_total         [ 7];
   char  categ_compte     [ 1];
   char  num_auto         [ 6];
   char  num_compte       [11];
   char  num_retrait      [ 4];
   char  cd_reponse       [ 2];
   char  piste_iso2       [37];
}crretrait2_t;

typedef struct crretrait23_s
{
   char  mt_retrait       [  7];
   char  mt_demande       [  7];
   char  mt_total         [  7];
   char  categ_compte     [  1];
   char  num_auto         [  6];
   char  num_compte       [ 11];
   char  num_retrait      [  4];
   char  cd_reponse       [  2];
   char  piste_iso2       [ 37];
   char  piste_iso3       [104];
   char  ecr_carte        [  1];
}crretrait23_t;
typedef struct depcompt_s
{
   char  Ncompt_cr         [11];
   char  Ncompt_dep        [11];
   char  montant_v         [11];
   char  codedev            [3];
}depcompt_t;
#endif /* RM_STRUCT_CR_H */
