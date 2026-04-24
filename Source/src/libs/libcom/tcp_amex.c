#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socketvar.h>

#include <define.h>
#include <tcp_var.h>
#include <atmopcode.h>

void  GetFdConnect(char *sFdStr)
{
	sprintf(sFdStr,"%.6d", nFdConnect);
}

void  SetFdConnect(char *sFdStr)
{
	nFdConnect = atoi(sFdStr);
}

int ReadLineMsg(char *sBuffer, int *nLength)
{
	char  sBuffRcv[ LG_MAX ];
	int   nLenMsg = 0, nRead=0;
	char  nLen[5];
	char   sLine[MAX_LINE_TRC];
	int   i;
	char  sDest[ LG_MAX ];

	sprintf ( sLine,"Start ReadLinedMsg(%d)", nFdConnect );
	trace_event(sLine,PROCESSING);

	nRead = read(nFdConnect, sBuffRcv, 1024);
	if (nRead < 0)
	{

		sprintf ( sLine,"Error Reading Msg : %d", errno );
		trace_event(sLine,ERROR);
		if ( errno == EINTR)
		{
			return (NOK);
		}
		else return(OR_DISCONECT);
	}
	if (nRead == 0)
	{
		sprintf ( sLine,"Disconnect : %d", errno );
		trace_event(sLine,ERROR);
		return (OR_DISCONECT);
	}
	
	*nLength -= 2;		/*AMER20180306 PROD00054067: Exclude the MLI length*/
	*nLength = ((unsigned char)sBuffRcv[0] * 256) + (unsigned char)sBuffRcv[1] ;
	
	memcpy( sBuffer,  sBuffRcv+2 , *nLength );

	if (debug_option == 1)
		dump_buffer ( sBuffRcv, nRead , 'A', 'L', 'I' );
	else
		EncryptBuff(sBuffRcv,nLenMsg,sDest);


	sprintf(sLine, "End   ReadLineMsg(%d)", nRead);
	trace_event(sLine, PROCESSING);

	return (OK);
}

int WriteLineMsg(char *sBuffer, int nLength)
{
	int retour;
	int i;
	char  sBuffSnd[ LG_MAX ];
	char  sLine   [ LG_MAX ]; 
	char  sDest[ LG_MAX ];
        int fd;
    	sprintf(sLine, "Start WriteLineMsg snd(%d, %d)", nFdConnect, nLength);
	trace_event(sLine, PROCESSING);

	memset (sBuffSnd, 0, sizeof(sBuffSnd));
	
	nLength += 2;	/*AMER20180306 PROD00054067: Include the length of the MLI*/

	sBuffSnd [ 0 ] = nLength / 256;
	sBuffSnd [ 1 ] = nLength % 256;

        memcpy (sBuffSnd+2 , sBuffer, nLength);


	if (debug_option == 1)
		dump_buffer ( sBuffSnd, nLength , 'A', 'L', 'O' );
	else
		EncryptBuff(sBuffSnd,nLength,sDest);
      fd=nFdConnect;

	/*retour = send ( fd ,sBuffSnd , nLength + 2 , 0);*/
	retour = send ( fd ,sBuffSnd , nLength, 0);		/*AMER20180306 PROD00054067: 2 should be removed because it is already added above*/
	if (retour <= 0)
	{
		sprintf(sLine, "Error Sending Msg : %d", errno);
		trace_event(sLine, ERROR);
		return(NOK);
	}

	sprintf ( sLine,"End   WriteLineMsg(%d)", nFdConnect );
	trace_event(sLine,PROCESSING);

	return(OK);
}

int  CloseCldCom()
{
	return(OK);
}
