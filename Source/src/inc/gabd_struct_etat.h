#ifndef RM_STRUCT_ETAT_H
#define RM_STRUCT_ETAT_H

typedef struct reqetat_s
{
   char   num_LN         [ 4];
   char   date_LN        [ 6];
   char   champ_MV       [ 1];
   char   champ_LS       [ 1];
   char   champ_EH       [ 1];
   char   etat_gab       [ 1];
   char   etat_cassette  [ 2];
   char   etat_imprim    [ 2];
   char   compte_rendu   [ 1];
   char   reserved_1     [ 6]; /* Initialise a zero */
   char   ver_logiciel   [ 6]; 
   char   reserved_2     [ 3]; /* Initialise a '001'*/
   char   reserved_3     [ 9]; /* Initialise a zero */
   char   date_gcarte    [ 6];
   char   ver_gcarte     [ 4];
   char   DATXF          [ 6]; /* Initialise a zero */
   char   NUTXF          [ 4]; /* Initialise a zero */
   char   DATXB          [ 6]; /* Initialise a zero */
   char   NUTXB          [ 4]; /* Initialise a zero */
   char   date_fecran    [ 6]; 
   char   ver_fecran     [ 4]; 
   char   date_param_apli[ 6]; 
   char   ver_param_apli [ 4]; 
   char   cle_master     [ 1]; 
   char   num_cle_master [ 1]; 
   char   cle_Kt1        [ 1]; 
   char   ind_cle_Kt1    [ 4]; 
   char   cle_Kt2        [ 1]; 
   char   ind_cle_Kt2    [ 4]; 
   char   cle_Ks3        [ 1]; 
   char   ind_cle_Ks3    [ 4]; 
   char   cle_Ks4        [ 1]; 
   char   ind_cle_Ks4    [ 4]; 
   char   cle_Ks5        [ 1]; 
   char   ind_cle_Ks5    [ 4]; 
   char   cle_Ks6        [ 1]; 
   char   ind_cle_Ks6    [ 4]; 
}reqetat_t;

typedef struct repetat_s
{
   char   ch_datheur     [ 1];
   char   etat_gab       [ 1];
   char   reserved_1     [ 1]; /* Initialise a I */
   char   arrete         [ 1];
   char   cd_reponse     [ 2];
   char   date           [ 6];
   char   heure          [ 4];
   char   reserved_2     [ 6]; /* Initialise a 0 */
   char   reserved_3     [ 3]; /* Initialise a I */
   char   load_log       [ 1]; 
   char   cmd_load       [ 1]; 
   char   nb_file2load   [ 2]; 
}repetat_t;

typedef struct etatfile_s
{
   char   nom_fich       [ 2];
}etatfile_t;

typedef struct etatrail_s
{
   char   cle_load       [ 1];
}etatrail_t;

#endif /* RM_STRUCT_ETAT_H */
