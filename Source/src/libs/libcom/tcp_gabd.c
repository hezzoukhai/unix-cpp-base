#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socketvar.h>

#include <define.h>
#include <line_com.h> 
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
int ReadLineMsg(	int  nHandle,
					char *sBuf,
					int  *nLen
					)
{
char *sPtr;
int  nRead,nRetCode ;
int  nLeft ;
char sWork[LG_MAX];
char sLine[MAX_LINE_TRC];
char  sDest[ LG_MAX ];/*MKB060309 PCI Norme Integration */

	trace_event("Start ReadLineMsg()", PROCESSING);
  
	sPtr  = sBuf;
	nLeft = LG_VTCP_HDR;		/* evaluate 1st part of the message */

	while (nLeft){
		 if ( (nRead = recv (nHandle, sPtr, nLeft, MSG_PEEK)) <= 0) 
			break;		

		sPtr  += nRead;
		nLeft -= nRead;
	}

	if (nRead <= 0){ 
			sprintf(sLine, "Communication failure errno = %d", errno);
			trace_event(sLine, PROCESSING);
			trace_event("End ReadLineMsg (DISCONECT)", PROCESSING);
			return(OR_DISCONECT);
	} 

    sprintf(sLine,  "nLeft = %d, nRead = %d", nLeft, nRead);
    trace_event(sLine, TRACE);

    /*Begin MKB060309 PCI Norme Integration */
    if (debug_option == 1)
    	dump_buffer(sBuf, nRead, 'A','L','I');
   else
        EncryptBuff(sBuf,nRead,sDest);
   /*End MKB060309 PCI Norme Integration */


	szcpy(sWork, sBuf , LG_VTCP_HDR );
	nLeft  = atoi(sWork);

	*nLen = nLeft;				/* Assume we will read the entire message */ 
	sprintf(sLine,"Longueur lu est %d",*nLen);
	trace_event(sLine,TRACE);

	sPtr   = sBuf;
	nLeft += LG_VTCP_HDR;

 	while (nLeft  ){
		if ( (nRead = recv (nHandle, sPtr, nLeft, 0)) <= 0) 
			break;		

		sPtr  += nRead;
		nLeft -= nRead;
	}

	if (nRead <= 0){ 
			sprintf(sLine, "Communication failure errno = %d", errno);
			trace_event(sLine, PROCESSING);
			trace_event("End ReadLineMsg (DISCONECT)", PROCESSING);
			return(OR_DISCONECT);
	} 

    	/*Begin MKB060309 PCI Norme Integration */
    	if (debug_option == 1)
		dump_buffer(sBuf, *nLen + LG_VTCP_HDR , 'A','L','I'); /*EBE140506: Adding 'A' */
   	else
        	EncryptBuff(sBuf,*nLen + LG_VTCP_HDR,sDest);
   	/*End MKB060309 PCI Norme Integration */

	trace_event("End ReadLineMsg (OK)", PROCESSING);
	return (OK);
}

int ReadLineMsgold(int nFd, char *sBuffer, int *nLength)
{
   int   nLenMsg, nRead;
   int   i;
   char sLine[MAX_LINE_TRC];
   char  sBuffRcv[ LG_MAX ];

   /*** Lecture de La Longueuer du message ****/
   trace_event("Start ReadLineMsg()",TRACE);
   do
   {
       nRead = recv(nFd, sBuffRcv, LG_VTCP_HDR , MSG_PEEK);
   } while ( (nRead !=LG_VTCP_HDR) && (nRead > 0)); 
   if (nRead < 0)
   {
   	  sprintf(sLine,"End   ReadLineMsg(ERROR MSG PEAK): nRead : %d,errno : %d",nRead,errno);
	  trace_event(sLine,TRACE);
      return (NOK);
   }
   if (nRead == 0)
   {
   	  trace_event("End   ReadLineMsg(DISCONECT MSG PEAK)",TRACE);
      return (OR_DISCONECT);
   }

   nLenMsg = ((unsigned char)sBuffRcv[0] * 256) + (unsigned char)sBuffRcv[1] + LG_VTCP_HDR ;
   fprintf(stderr,"NLENMSG = %d BUFF = %02X %02X \n", nLenMsg, (unsigned char)sBuffRcv[0], (unsigned char)sBuffRcv[1]);
   do
   {
   		nRead = recv(nFd, sBuffRcv, nLenMsg, 0);
   }while ( (nRead !=LG_VTCP_HDR) && (nRead > 0));

   if (nRead < 0)
   {
   	  sprintf(sLine,"End   ReadLineMsg(ERROR): nRead : %d,errno : %d",nRead,errno);
	  trace_event(sLine,TRACE);
      return (NOK);
   }
   if (nRead == 0)
   {
   	  trace_event("End   ReadLineMsg(DISCONECT)",TRACE);
      return (OR_DISCONECT);
   }
   memcpy( sBuffer, sBuffRcv +  LG_VTCP_HDR , nLenMsg - LG_VTCP_HDR);
   *nLength = nLenMsg - LG_VTCP_HDR;

   trace_event("End   ReadLineMsg(OK)",TRACE);
   return (OK);
}

int WriteLineMsg(int nFd, char *sBuffer, int nLength)
{
   char  sBuffSnd[ LG_MAX ];
   int retour;
   int i;

   memset (sBuffSnd, 0, LG_MAX);

   sBuffSnd [ 0 ] = nLength / 256;
   sBuffSnd [ 1 ] = nLength % 256;
   memcpy (sBuffSnd + LG_VTCP_HDR , sBuffer, nLength);
   fprintf(stderr,"\nDEBUT SEND ");
   for (i = 0; i< nLength; i++)
   {
	  if ( !(i%24))
		 fprintf(stderr,"\n");
	  fprintf(stderr,"%02X ", sBuffSnd[i]);
   }
   fprintf(stderr,"\nFIN SEND");
   fprintf ( stderr,"\n av send Socket = %d longueur = %d \n", nFd , nLength );
   retour = send ( nFd, sBuffSnd, nLength + LG_VTCP_HDR , 0);
   if (retour <= 0)
   {
      fprintf(stderr,"Error Sending Msg : %d \n", errno);
      return(NOK);
   }
   return(OK);
}

int  CloseCldCom()
{
return(OK);
}

