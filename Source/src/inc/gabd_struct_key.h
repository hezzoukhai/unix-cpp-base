#ifndef RM_STRUCT_KEY_H
#define RM_STRUCT_KEY_H
typedef struct dgabkey_s
{
   char  reserved     [6]; /* Initialise a 0 */
   char  num_seq      [1];
   char  code_trait   [2];
} dgabkey_t;

typedef struct repkeyhead_s
{
   char  reserved     [6];
   char  num_seq      [1];
   char  format_cle   [1];
}repkeyhead_t;

typedef struct keymaster_s
{
   char num_cle       [ 1];
   char cle_AB        [ 1];
   char cle           [22];
}keymaster_t;

typedef struct keytransport_s
{
   char type_cle      [ 1];
   char num_cle       [ 1];
   char indice_cle    [ 4];
   char cle           [16];
   char niveau_cle    [ 1];
   char reserved      [ 1];
}keytransp_t;

typedef struct keycontrol_s
{
   char type_cle      [ 1];
   char num_cle       [ 1];
   char indice_cle    [ 4];
   char bank_id       [ 6];
   char cle           [16];
}keycontrol_t;

#endif /* RM_STRUCT_KEY_H */
