#ifndef PWC__GLOBAL_F__H
#define PWC__GLOBAL_F__H


/*  Mehdi Elyajizi routines encodage/decodage base64 */
#include <malloc.h>

typedef struct b64_chunk
{
/*
** longeur de la chaine, comme on n'aura besoin d'aller de 0 a 3, on utilise un
** unsigned char plutot qu'un unsigned int pour gagner 3 octets sur un une
** plateforme 32 bits.
*/
    int len;
/*
** Buffers d'encodage et de décodage.
*/
    char enc[4];
    char dec[3];
}B64_CHUNK;

/*
** Remise à zero de toutes les valeurs du B64_CHUNK
*/
void zero_memory(B64_CHUNK *);

/*
** Procédures d'encodage et de décodage
*/
void base64_encoding_process(B64_CHUNK *);
void base64_decoding_process(B64_CHUNK *);

/*
** Assignation d'un caractere valide apres encodage
*/
char base64_chr_table(char);

/*
** Fonctions permettant la vérification des bons caracteres
*/
void base64_chr_real(char *);


#endif

