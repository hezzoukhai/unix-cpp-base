#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#include <memory.h>
#include <define.h>
#include <tools.h>
#include <ascii.h>

int      fp;
extern 	int 	errno;
convertAmexFile ( char  *fileDirectoryPath, char *inputFileName , char *outputFileName, int lengthMax )
{

    int  nbr_read;
    char buffer [ 4096];
	char   detail[MAX_LINE_TRC];
	char    inputFile		[   MAX_LINE_TRC] ;
	char    outputFile		[   MAX_LINE_TRC] ;
	
	int      fdIn;
	int      fdOut;
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
	memset (detail, 0, sizeof(detail));
	memset(inputFile,0,sizeof(inputFile));
	memset(outputFile,0,sizeof(outputFile));
	
	sprintf(inputFile,"%s/%s",fileDirectoryPath , inputFileName);
	sprintf(outputFile,"%s/%s",fileDirectoryPath , outputFileName);
	
    if ( ( fdIn = open( inputFile , O_RDONLY ) ) < 0 )
    {
		sprintf(detail, "ERROR OPENING FILE : %s %d", inputFile, errno);
		trace_event(detail, PROCESSING);
        return(NOK);
    }
	
	if ( (fdOut = open(outputFile,O_WRONLY|O_TRUNC|O_CREAT,0777)) < 0 )
	{
		sprintf(detail, "ERROR OPENING FILE : %s %d", outputFile, errno);
		trace_event(detail, PROCESSING);
        return(NOK);
	}
	

   
    while ( 1 )
    {
		nbr_read = read(fdIn, buffer, lengthMax );
		
    	if ( nbr_read <= 0 ) 
        {
			break;
        }
        memcpy(sSrc,buffer,lengthMax);
        EbcdicToAscii(sSrc,sDst,lengthMax);
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
           memcpy(buffer,sDst,lengthMax);
	   
		write(fdOut,buffer,lengthMax);
		write(fdOut,"\n",1);
   }
   
   close(fdOut);
   close(fdOut);
   return(OK);
}
