#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

int      fp;

main ( char argc , char **argv )
{

    int nLengthMax, nbr_read;
    char buffer [ 4096];
/*EBE151009: handle binary data*/
	int		nFdOut;
   
    memset ( buffer  , 0 , sizeof ( buffer ));
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
        /*fprintf ( stdout ,"%s\n", buffer );*/
		write(nFdOut,buffer,nLengthMax);
		write(nFdOut,"\n",1);
   }
}


