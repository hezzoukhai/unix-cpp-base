#ifndef RM_STRUCT_ARRET_H
#define RM_STRUCT_ARRET_H
typedef struct arret_s
{
   char nb_retrait         [ 4];
   char mt_retrait         [11];
   char nb_virment         [ 4];
   char mt_virment         [11];
   char nb_espece          [ 4];
   char mt_espece          [11];
   char nb_dcheque         [ 4];
   char mt_dcheque         [11];
   char nb_hist            [ 4];
   char nb_rib             [ 4];
   char nb_solde           [ 4];
   char nb_cheque          [ 4];
   char reserved           [16];
}arret_t;
#endif /* RM_STRUCT_ARRET_H */
