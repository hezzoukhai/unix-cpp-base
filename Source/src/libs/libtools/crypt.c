#include <define.h>

/* EBE140523: Removed all crypto functions. Linking with libtools instead to avoid duplicate code*/
main(int argc, char **argv)
{

char sSource[17];
char sKey[17];
int  i;

    memset(sSource, 0, sizeof(sSource));
    memset(sKey, 0, sizeof(sKey));
    memcpy(sSource, argv[2], strlen(argv[2]));
    memcpy(sKey, argv[3], strlen(argv[3]));
    if (argv[1][0] == 'E')
    Encrypte_hex ( sSource , sKey );
    else
    Decrypt_hex ( sSource , sKey );

    printf(" Result is : ");

    for (i = 0; i<strlen(sSource); i++)
    printf("%02X", sSource[i]);

    printf("\n");

}