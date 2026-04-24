#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#include <memory.h>
#include <define.h>
#include <tools.h>
#include <ascii.h>

int      fp;

main ( char argc , char **argv )
{

    int nLengthMax, nbr_read;
    char buffer [ 4096];
    int		nFdOut;
    char sSrc[4096];
    char sDst[4096];
    char sSrc55[4096];
    char sDst55[4096];
   
    memset ( buffer  , 0, sizeof ( buffer ));
    memset ( sSrc    , 0, sizeof ( sSrc   ));
    memset ( sDst    , 0, sizeof ( sDst   ));
    memset ( sSrc55  , 0, sizeof ( sSrc55 ));
    memset ( sDst55  , 0, sizeof ( sDst55 ));

    nLengthMax = atoi ( argv [ 2 ] );
    if ( argc < 3 )
    {
        fprintf ( stdout ,"\n\n Use [%s] [filemae] [length] 1>err 2>output", argv [ 0 ] );
        exit (0);
    }

    if ( ( fp = open( argv [ 1 ] , O_RDONLY ) ) < 0 )
    {
        fprintf ( stdout ,"\n\n Can Not Open The File [%s] \n\n",  argv [ 1 ] );
        exit (0);
    }

    nFdOut = fileno(stdout);
    while ( 1 )
    {
   	nbr_read = read(fp, buffer, nLengthMax );
    	if ( nbr_read <= 0 ) 
        {
       		fprintf ( stderr ,"\n\n End Of File Processing \n\n");
		exit (0);
        }
        memcpy(sSrc,buffer,nLengthMax);
        EbcdicToAscii(sSrc,sDst,nLengthMax);
        if ( memcmp(sDst,"924007",6) == 0)
        {
           memcpy(sSrc55,sSrc+585+4,256);
           memset ( sDst55  , ' ', sizeof ( sDst55 ));
           HexToAscii(sSrc55, sDst55, 256);
           memset ( buffer  , 0, sizeof ( buffer ));
           memcpy(buffer,sDst,589);
           memcpy(buffer+589,sDst55,252);
           memcpy(buffer+841,sDst+841,559);
        }
        else
           memcpy(buffer,sDst,nLengthMax);
	write(nFdOut,buffer,nLengthMax);
	write(nFdOut,"\n",1);
   }
}
