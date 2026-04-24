#include <stdio.h>
#include <define.h>
#include <security_data.h>
#include <resources.h>
#include <stdio.h>
#include <sys/time.h>
#include <iso_hps.h>
#include <define.h>
#include <list_thr.h>
#include <queue.h>
#include <tlv_private.h>
#include <tlv_privfields.h>
#include <tlv_fields.h>
#include <p7_define.h>
#include <comms_param.h>
#include <ctype.h>

/*
t_secdata secdata[] = {
                            {  0, 2 },
                            {  2, 2 },
                            {  4, 3 },
                            {  7, 3 },
                            { 10, 8 },
                            { 18, 6 },
                            { 24, 8 },
                            { 32, 6 },
                            { 38,15 }, 
                            { 53, 9 }
                         };
*/

#define SECURITY_FORMAT_OFST				0
#define PIN_BLOCK_FORMAT_OFST				SECURITY_FORMAT_OFST	+ SECURITY_FORMAT_LEN
#define PEK_INDEX_OFST						PIN_BLOCK_FORMAT_OFST	+ PIN_BLOCK_FORMAT_LEN
#define MAC_KEY_INDEX_OFST					PEK_INDEX_OFST			+ PEK_INDEX_LEN
#define SOURCE_PID_OFST						MAC_KEY_INDEX_OFST		+ MAC_KEY_INDEX_LEN
#define SOURCE_RESOURCE_OFST				SOURCE_PID_OFST			+ SOURCE_PID_LEN
#define DESTINATION_PID_OFST				SOURCE_RESOURCE_OFST	+ SOURCE_RESOURCE_LEN
#define DESTINATION_RESOURCE_OFST			DESTINATION_PID_OFST	+ DESTINATION_PID_LEN
#define USER_OFST							DESTINATION_RESOURCE_OFST + DESTINATION_RESOURCE_LEN
#define PURGE_TIME_OFST						USER_OFST				+ USER_LEN
/*#define PURGE_TIME_PERIOD_OFST				PURGE_TIME_OFST			+ PURGE_TIME_LEN*/
#define SOURCE_NODE_ID_OFST					PURGE_TIME_OFST			+ PURGE_TIME_LEN
#define DESTINATION_NODE_ID_OFST			SOURCE_NODE_ID_OFST		+ NODE_ID_LEN
/*#define SEC_DATA_LEN						DESTINATION_NODE_ID_OFST + NODE_ID_LEN*/
#define PURGE_TIME_MS_OFST					DESTINATION_NODE_ID_OFST + NODE_ID_LEN
#define SEC_DATA_LEN						PURGE_TIME_MS_OFST + PURGE_TIME_MS_LEN


#define RES_INF_RES_NODE_OFST           0
#define RES_INF_RES_ID_OFST             RES_INF_RES_NODE_OFST + RES_INF_RES_NODE_LEN
#define RES_INF_RES_CD_OFST             RES_INF_RES_ID_OFST +	RES_INF_RES_ID_LEN
#define RES_INF_RES_TP_OFST             RES_INF_RES_CD_OFST +	RES_INF_RES_CD_LEN
#define RES_INF_RES_LIVE_OFST           RES_INF_RES_TP_OFST +	RES_INF_RES_TP_LEN
#define RES_INF_RES_PS_OFST             RES_INF_RES_LIVE_OFST + RES_INF_RES_LIVE_LEN
#define RES_INF_RES_PID_OFST            RES_INF_RES_PS_OFST +	RES_INF_RES_PS_LEN


static t_secdata secdata[] = {
                            {  SECURITY_FORMAT_OFST		,	SECURITY_FORMAT_LEN		},
                            {  PIN_BLOCK_FORMAT_OFST	,	PIN_BLOCK_FORMAT_LEN	},
                            {  PEK_INDEX_OFST			,	PEK_INDEX_LEN			},
                            {  MAC_KEY_INDEX_OFST		,	MAC_KEY_INDEX_LEN		},
                            { SOURCE_PID_OFST			,	SOURCE_PID_LEN			},
                            { SOURCE_RESOURCE_OFST		,	SOURCE_RESOURCE_LEN		},
                            { DESTINATION_PID_OFST		,	DESTINATION_PID_LEN		},
                            { DESTINATION_RESOURCE_OFST	,	DESTINATION_RESOURCE_LEN },
                            { USER_OFST					,	USER_LEN				}, 
                            { PURGE_TIME_OFST			,	PURGE_TIME_LEN			},
							{ SOURCE_NODE_ID_OFST		,	NODE_ID_LEN				},
							{ DESTINATION_NODE_ID_OFST	,	NODE_ID_LEN				},
							{ PURGE_TIME_MS_OFST		,	PURGE_TIME_MS_LEN				},
                         };

static t_secdata res_info_data[] = {
                            {	RES_INF_RES_NODE_OFST		,	RES_INF_RES_NODE_LEN		},
                            {	RES_INF_RES_ID_OFST			,	RES_INF_RES_ID_LEN			},
                            {	RES_INF_RES_CD_OFST			,	RES_INF_RES_CD_LEN			},
                            {	RES_INF_RES_TP_OFST			,	RES_INF_RES_TP_LEN			},
                            {	RES_INF_RES_LIVE_OFST		,	RES_INF_RES_LIVE_LEN		},
                            {	RES_INF_RES_PS_OFST			,	RES_INF_RES_PS_LEN			},
                            {	RES_INF_RES_PID_OFST		,	RES_INF_RES_PID_LEN			},
                           
                         };

#define TRACK1_SEPARATOR        0x5E

/* Mehdi Elyajizi PCI */
void    comphex(unsigned char    *target,unsigned char    *source)
{
   int    i;
   unsigned char   a;
   unsigned char   b;
   unsigned char   c;

   for(i = 0; i < strlen(source)/2; i++)
   {
      a = *(source + 2 * i);
      b = *(source + 2 * i + 1);
      c = 0x00;

      if (a == 0x41) a = 0x0A;
      else if (a == 0x42) a = 0x0B;
         else if (a == 0x43) a = 0x0C;
            else if (a == 0x44) a = 0x0D;
               else if (a == 0x45) a = 0x0E;
                  else if (a == 0x46) a = 0x0F;
      if (b == 0x41) b = 0x0A;
      else if (b == 0x42) b = 0x0B;
         else if (b == 0x43) b = 0x0C;
            else if (b == 0x44) b = 0x0D;
               else if (b == 0x45) b = 0x0E;
                  else if (b == 0x46) b = 0x0F;
      c = (unsigned char) ((a << 4) & 0xF0) | (b & 0x0F);
      *(target +  i)  = c;
   }
   return;
}
int TrunCard(char *Card, char *Trunc)
{
  int Clenght;
  int i;
  Clenght = strlen(Card);

  if (Clenght <= 10)
  {
   	memcpy(Trunc, Card, Clenght);
   	return (OK);
  }
        memcpy(Trunc, Card, 6);

        for(i=0; i <= Clenght - (6 + 4) - 1; i++ )
                memcpy(Trunc + 6 + i, "*", 1);

        memcpy(Trunc + 6 + ( Clenght - (6 + 4)) , Card + 6 + ( Clenght - (6 + 4)) , Clenght - (6 + ( Clenght - (6 + 4))) );

        memcpy(Trunc + Clenght -1, "*", 1 );

        return 0;
}
int DeCryptCard(char *source, char *dest)
{
	memset(dest, 0, sizeof(dest));
	
	DES(-1, (unsigned char)'H', dest, (unsigned char *)KEY_PASS, dest);	
	comphex(dest, source);
	return 0;
}

/*Begin MKB060309 PCI Norme Integration */
void EncryptBuff(char *sSrc,int nlenSrc,char *sDest)

{
	int i,j,k;

	char sTmpBuf[64];
	char sDestPrint[2000];

	memset(sDest,0,sizeof(sDest));
	memset(sDestPrint,0,sizeof(sDestPrint));
	memset(sTmpBuf,0,sizeof(sTmpBuf));
	trace_event("Before Dump",TRACE); 

	j=nlenSrc;

	sprintf(sDestPrint,"%01d",j%8);

	if (j <=8)
	{
		DES(1, (unsigned char)'C', sSrc, (unsigned char *)KEY_PASS, sDest);
		memcpy(sDestPrint + 1,sDest,8);
		/*dump_buffer_debug(sDestPrint,9 , 'A', 'B', 'I');*/

	}
	else
	{

		k=j%8;
		for (i=0;i<j-k;i++)

		{
			memset(sTmpBuf,0,sizeof(sTmpBuf));
			DES(1, (unsigned char)'C', sSrc + i, (unsigned char *)KEY_PASS, sTmpBuf);
			memcpy(sDest + i,sTmpBuf,8);
			i = i + 7;

		}

		if (k!=0)
		{
			
			memset(sTmpBuf,0,sizeof(sTmpBuf));
			DES(1, (unsigned char)'C', sSrc + i, (unsigned char *)KEY_PASS, sTmpBuf);
			memcpy(sDest + i,sTmpBuf,8);
			memcpy(sDestPrint + 1,sDest,j + 8 - k);
			dump_buffer_debug(sDestPrint,j + 9 - k , 'A', 'B', 'I');

		}
		else
		{
			memcpy(sDestPrint + 1,sDest,j);
			dump_buffer_debug(sDestPrint,j + 1 , 'A', 'B', 'I');

		}


	}


}
void EncryptBuffer(char *sSrc,int nlenSrc,char *sDest, int* nLenDst, char* print_flags)

{
	int i,j,k;

	char sTmpBuf[64];
	char sDestPrint[2000];

	memset(sDest,0,sizeof(sDest));
	memset(sDestPrint,0,sizeof(sDestPrint));
	memset(sTmpBuf,0,sizeof(sTmpBuf));
	trace_event("Before Dump",TRACE); 

	j=nlenSrc;

	sprintf(sDestPrint,"%01d",j%8);

	if (j <=8)
	{
		DES(1, (unsigned char)'C', sSrc, (unsigned char *)KEY_PASS, sDest);
		memcpy(sDestPrint + 1,sDest,8);
		(*nLenDst) = 8;
		if( strlen(print_flags) >= 3)
			dump_buffer_debug(sDestPrint,9 , print_flags[0], print_flags[1], print_flags[2]);

	}
	else
	{

		k=j%8;
		for (i=0;i<j-k;i++)

		{
			memset(sTmpBuf,0,sizeof(sTmpBuf));
			DES(1, (unsigned char)'C', sSrc + i, (unsigned char *)KEY_PASS, sTmpBuf);
			memcpy(sDest + i,sTmpBuf,8);
			i = i + 7;

		}

		if (k!=0)
		{
			
			memset(sTmpBuf,0,sizeof(sTmpBuf));
			DES(1, (unsigned char)'C', sSrc + i, (unsigned char *)KEY_PASS, sTmpBuf);
			memcpy(sDest + i,sTmpBuf,8);
			memcpy(sDestPrint + 1,sDest,j + 8 - k);
			(*nLenDst) = j + 8 - k;
			dump_buffer_debug(sDestPrint,j + 9 - k , print_flags[0], print_flags[1], print_flags[2]);

		}
		else
		{
			memcpy(sDestPrint + 1,sDest,j);
			(*nLenDst) = j;
			dump_buffer_debug(sDestPrint,j + 1 , print_flags[0], print_flags[1], print_flags[2]);
		}
	}
}
/***************************************************/
void DecryptBuff(char *sSrcPrint,int nlenSrc,char *sDest,int nLenToprint)

{
int i,j,k;

char sTmpBuf[64];
char sSrc[2000];
char sRem[1];

memset(sTmpBuf,0,sizeof(sTmpBuf));
memset(sSrc,0,sizeof(sSrc));
memset(sRem,0,sizeof(sRem));
memcpy(sSrc,sSrcPrint + 1,nlenSrc - 1);



j=nlenSrc - 1;

if (j <=8)
        DES(-1, (unsigned char)'C', sSrc, (unsigned char *)KEY_PASS, sDest);
else
   {

    k=j%8;
    for (i=0;i<j-k;i++)

     {
             memset(sTmpBuf,0,sizeof(sTmpBuf));
             DES(-1, (unsigned char)'C', sSrc + i, (unsigned char *)KEY_PASS, sTmpBuf);
             memcpy(sDest + i,sTmpBuf,8);
             i = i + 7;

     }

    if (k!=0)
       {
            
             memset(sTmpBuf,0,sizeof(sTmpBuf));
             DES(-1, (unsigned char)'C', sSrc + i, (unsigned char *)KEY_PASS, sTmpBuf);
             memcpy(sDest + i,sTmpBuf,8);

       }

   }

if( sSrcPrint[0] != '0')
 {
  sRem[0] = sSrcPrint[0];
  j= j- (8 - atoi(sRem));
 }
 dump_buffer_debug(sDest,j, 'A', 'B', 'I');

}

/*End MKB060309 PCI Norme Integration */
int CryptCard(char *Card, char *CryptCrd)/*, char    *CryptCrd)*/
{
  int Clenght;
  int i;
  Clenght = strlen(Card);
  char    clearCard[22 + 1];
  char    CryptHex[128];
  char sLine [LG_MAX + 1];

  memset(sLine, 0, sizeof(sLine));
	memset(CryptHex,0,sizeof(CryptHex));
	memset(clearCard,0,strlen(Card));
	

        if ((strlen(Card)  <= 10) || (debug_option != 1))
	   return (OK);

	memcpy(clearCard,Card +6,strlen(Card) - 6 - 4);
	memcpy(clearCard + strlen(Card) - 6 - 4,Card + strlen(Card) -1  ,1);

  for(i=0;i<strlen(clearCard);i++)
          sprintf(CryptHex + (2*i),"%0X",clearCard[i]);     
       
	DES(1, (unsigned char)'H', CryptHex, (unsigned char *)KEY_PASS, CryptCrd);

  /*for(i=0;i<strlen(CryptCrd);i++)
          sprintf(CryptHex + (2*i),"%0X",CryptCrd[i]);*/
                        
        return 0;
}


/* Mehdi Elyajizi PCI */
/****************************************************************************/
/*  TRACK2_DATA                                                             */
/****************************************************************************/
int GetTrack2Data ( char *sTrack2Data ,
                    char *sPan, 
					char *sExpiryDate, 
					char *sServiceCode, 
					char *sPvv, 
					char *sCvv1 )
{


   int i,len;
   len = strlen ( sTrack2Data );
   if( len != 0 )
    {
       for(i=0;(isdigit(sTrack2Data[i]) != 0); i++ );
       memcpy ( sPan           , sTrack2Data,  i );
       memcpy ( sExpiryDate    , sTrack2Data + i + 1,  4 );
       memcpy ( sServiceCode   , sTrack2Data + i + 1 + 4 , 3 );
       memcpy ( sPvv           , sTrack2Data + i + 1 + 7 , 5 );
       memcpy ( sCvv1          , sTrack2Data + i + 1 + 12, 3 );
       if (strlen ( sCvv1          ) == 0)   /* To Combat Fraud */
           memcpy ( sCvv1          , "000" ,3 );
     }
	return(OK);
}

/****************************************************************************/
/*  TRACK2_DATA                                                             */
/****************************************************************************/
int MaskTrack2Data (char *sTrack2Data ,
					char *sMaskFlag)
{

	int i,len;
	len = strlen ( sTrack2Data );
	if( len == 0 )
	{
		return OK;
	}

    for(i=0;(isdigit(sTrack2Data[i]) != 0); i++ );
	if( sMaskFlag[0] != '0' && i<=len)
	{
		memset(sTrack2Data,'X',i);
	}
	if( sMaskFlag[1] != '0' && (i+1+4)<=len)
	{
		memset(sTrack2Data + i + 1,'X',4);
	}
	if( sMaskFlag[2] != '0' && (i+5+3)<=len)
	{
		memset(sTrack2Data + i + 1 + 4,'X',3);
	}
	if( sMaskFlag[3] != '0' && (i+8+1)<=len)
	{
		memset(sTrack2Data + i + 1 + 7,'X',1);
	}
	if( sMaskFlag[4] != '0' && (i+9+4)<=len)
	{
		memset(sTrack2Data + i + 1 + 8,'X',4);
	}
	if( sMaskFlag[5] != '0' && (i+13+3)<=len )
	{
		memset(sTrack2Data + i + 1 + 12,'X',3);
	}

	return(OK);
}
/* ----------------------------------------------------------------------------------*/ 
/* 																					 */ 
/* ----------------------------------------------------------------------------------*/ 
int GetTrack3Data(char *sTrack3Data,char *sOffset)
{
int i,len;
   len = strlen ( sTrack3Data );
   if( len != 0 )
    {
       for(i=0;(isdigit(sTrack3Data[i]) != 0); i++ );
       memcpy ( sOffset         , sTrack3Data + OFFS_OFFSET , 4 );
     }
  return(OK);
}
/****************************************************************************/
/*  TRACK1_DATA                                                             */
/****************************************************************************/
int GetTrack1Data ( char *sTrack1Data ,
                    char *sPan, 
					char *sExpiryDate, 
					char *sServiceCode, 
					char *sPvv, 
					char *sCvv1 )
{

   int i,j,len;
   len = strlen ( sTrack1Data );

   if( len != 0 )
   {
       for(i=0; sTrack1Data[i] != TRACK1_SEPARATOR ; i++ );
       j = i+1;

       memcpy ( sPan           , sTrack1Data + 1  ,  i  - 1 );
       for(i=j; sTrack1Data[i] != TRACK1_SEPARATOR ; i++ ); 
     
       memcpy ( sExpiryDate    , sTrack1Data + i + 1,  4 );
       memcpy ( sServiceCode   , sTrack1Data + i + 1 + 4 , 3 );
       memcpy ( sPvv           , sTrack1Data + i + 1 + 7 , 5 );
      /*START NAB20052019 Certif SG :le cvv1 n'est pas bien positionee en track1 voir doc:Payment Technology Standards Manual: Track 1 Data*/
      /* j = i+23; *//**OBE200612**/
       /*for ( i=j ; isdigit ( sTrack1Data [ i ] ) == 0 ; i++ );*/
	  if (len >=11 )
	  {
		  j=len - 11 + 2 ;
		  memcpy ( sCvv1          , sTrack1Data + /*i*/j , 3 );
	  }
	  /*END NAB20052019*/
	  /*Start IBO20221228 PLUTONL-5606*/
	  else
	  {
		  memcpy ( sCvv1          , "000" , 3 );
	  }
	  if (strlen(sCvv1) == 0)
		  memcpy ( sCvv1          , "000" , 3 );
	  /*End IBO20221228 PLUTONL-5606*/
   }
  return(OK);
}

int GetAmxTrack2Data (		char *	sTrack2Data ,
							int		nFmt,
							char *	sPan, 
							char *	sExpiryDate, 
							char *	sServiceCode, 
							char *	sEffDate, 
							char *	sCSC5 )
{


   int i,len;
   len = strlen ( sTrack2Data );
   int nOffset =0;
   if( len != 0 )
    {
       for(i=0;(isdigit(sTrack2Data[i]) != 0); i++ );

       memcpy ( sPan           , sTrack2Data,  i ); nOffset +=i;
	   nOffset++;
       memcpy ( sExpiryDate    , sTrack2Data + nOffset,  4 ); nOffset+=4;
	   if( nFmt == ISO_7813_T2_FMT )
	   {
		memcpy ( sServiceCode   , sTrack2Data + nOffset , 3 ); nOffset+=3;
	   }
	   else
	   {
		   sServiceCode[0]='\0';
	   }
       memcpy ( sEffDate       , sTrack2Data + nOffset , 4 );nOffset+=4;

       memcpy ( sCSC5          , sTrack2Data + nOffset, 5 );
       if (strlen ( sCSC5          ) == 0)   /* To Combat Fraud */
           memcpy ( sCSC5          , "00000" ,5 );
     }
	return(OK);
}

/*Start AMER20210909 PLUTONL-3523*/
int GetAmxTrack1Data (		char *	sTrack1Data ,
							char *	sPan, 
							char *	sExpiryDate, 
							char *	sServiceCode, 
							char *	sCSC5 )
{


	int i,len;
	len = strlen ( sTrack1Data );
	int nOffset =0;
	
	if( len != 0 )
	{ 
		for(i=1; sTrack1Data[i] != '^'; i++ );

		memcpy ( sPan           , sTrack1Data + 1,  i-1 );

		for(nOffset=i+1; sTrack1Data[nOffset] != '^'; nOffset++ );
		nOffset+=1;

		memcpy ( sExpiryDate    , sTrack1Data + nOffset, 4 ); 
		nOffset+=4;

		memcpy ( sServiceCode   , sTrack1Data + nOffset, 3 ); 
		nOffset+=3;
		
		nOffset+=4;

		memcpy ( sCSC5          , sTrack1Data + nOffset, 5 );
		
		if (strlen ( sCSC5          ) == 0)   /* To Combat Fraud */
		   memcpy ( sCSC5          , "00000" ,5 );
    }
	
	return(OK);
}
/*End AMER20210909 PLUTONL-3523*/

/****************************************************************************/
/*  GET SECURITY_DATA                                                       */
/****************************************************************************/

int GetSecurityData (int subfield,char *security_data, char *value ,int *length)
{
	int		nLength;
    /* some interfaces still don't fill the the new fields nodes id so we work around that*/
	nLength = strlen(security_data);
	if( nLength < SEC_DATA_LEN )
	{
		memset(security_data + nLength, '0', SEC_DATA_LEN - nLength);
		security_data[SEC_DATA_LEN]='\0';
	}

	memcpy(value, security_data + secdata[subfield].offset, secdata[subfield].length);
    value [secdata[subfield].length] = 0;
	*length = secdata[subfield].length;
    return( OK );
}

/****************************************************************************/
/*  PUT SECURITY_DATA                                                       */
/****************************************************************************/

int PutSecurityData (int subfield,char *security_data, char *value ,int length)
{
	int		nLength;

    if ( length != secdata[subfield].length) 
	{
		char sLine[MAX_LINE_TRC];
		sprintf(sLine,"PutSecurityData Error(%d): LEN FLD ERROR %d,%d ",subfield,secdata[subfield].length,length);
        trace_event ( sLine, ERROR );
        return ( NOK );
    }

	
    /* some interfaces still don't fill the the new fields nodes id so we work around that*/
	nLength = strlen(security_data);
	if( nLength < SEC_DATA_LEN )
	{
		memset(security_data + nLength, '0', SEC_DATA_LEN - nLength);
		security_data[SEC_DATA_LEN]='\0';
	}

    memcpy(security_data + secdata[subfield].offset, value , length );


    return( OK );
}
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

int RessourceUp( TSResources *sResource )
{
  pid_t pid_interface;


  if ( ( memcmp ( sResource->resource_live , "Y" , 1 ) == 0 ) &&
       ( memcmp ( sResource->pris_processing_step , "ONL" , 3 ) == 0 ) )
       {
		pid_interface = sResource->pris_pid;
		if ( kill( pid_interface, 0) == 0)
        	{
            		trace_event("End   RessourceUp (NOK)", PROCESSING);
            		return(NOK);
        	}
    		return ( OK );
       }
   return ( NOK );
}

     
/****************************************************************************/

/****************************************************************************/

void SwapDestAndSource( char * sSecurityData )
{
	char				sSourceResource[RESOURCE_ID_LEN + 1];
	char				sDestResource[RESOURCE_ID_LEN + 1];
	char				sSourceId[PROC_PID_LEN + 1];
	char				sDestinationId[PROC_PID_LEN + 1];
	char				sSourceNodeId[NODE_ID_LEN + 1];
	char				sDestNodeId[NODE_ID_LEN + 1];
	char				tmp_buffer[7];
	int					nLength;
	char				sLine[MAX_LINE_TRC];

	memset (   sSourceResource , 0, sizeof(sSourceResource) );
	memset (     sDestResource , 0, sizeof(sDestResource) );
	memset (         sSourceId , 0, sizeof(sSourceId) );
	memset (    sDestinationId , 0, sizeof(sDestinationId) );
	memset (    sSourceNodeId , 0, sizeof(sSourceNodeId) );
	memset (    sDestNodeId , 0, sizeof(sDestNodeId) );
	memset (        tmp_buffer , 0, sizeof(tmp_buffer) );

	trace_event ( "SwapDestAndSource ( Start )", PROCESSING );

	GetSecurityData(SOURCE_RESOURCE,sSecurityData,sSourceResource,&nLength);
	GetSecurityData(SOURCE_NODE_ID,sSecurityData,sSourceNodeId,&nLength);
	GetSecurityData(SOURCE_PID     ,sSecurityData,sSourceId,&nLength);

	GetSecurityData(DESTINATION_RESOURCE ,sSecurityData,sDestResource,&nLength);
	GetSecurityData(DESTINATION_NODE_ID ,sSecurityData,sDestNodeId,&nLength);
	GetSecurityData(DESTINATION_PID      ,sSecurityData,sDestinationId,&nLength);

	PutSecurityData(SOURCE_RESOURCE	,sSecurityData,sDestResource , RESOURCE_ID_LEN );
	PutSecurityData(SOURCE_NODE_ID	,sSecurityData,sDestNodeId , NODE_ID_LEN );
	PutSecurityData(SOURCE_PID		,sSecurityData,sDestinationId , PROC_PID_LEN );

	PutSecurityData(DESTINATION_RESOURCE,sSecurityData,sSourceResource , RESOURCE_ID_LEN );
	PutSecurityData(DESTINATION_NODE_ID,sSecurityData,sSourceNodeId , NODE_ID_LEN );
	PutSecurityData(DESTINATION_PID	,sSecurityData,sSourceId , PROC_PID_LEN );


	trace_event ( "SwapDestAndSource ( End )", PROCESSING );
}


/*****************************************************************************/

/*****************************************************************************/
int ReplyIsoMsg    ( 	TSIsoInfo * IsoInfo , 
	char      * Field38 , 
	char      * Field39 
	)
{

	char       sSecurityData[128 + 1];
	int                   nLength;  

	GetIsoField( ISO_SECURITY_DATA , IsoInfo , sSecurityData , &nLength ); 
	SwapDestAndSource( sSecurityData );
	PutIsoField( ISO_SECURITY_DATA , IsoInfo , sSecurityData , nLength ); 


	if( Field38 != NULL && strlen ( Field38 ) != 0 )
		PutIsoField( ISO_AUTHOR_ID 	, IsoInfo , Field38 , strlen(Field38) );


	if( Field39 != NULL && strlen ( Field39 ) != 0 )
		PutIsoField( ISO_RESPONSE_CODE , IsoInfo , Field39 , strlen(Field39) );

	return( OK );
}
/******************************************************************************/

/******************************************************************************/

int ReplyIsoMsgToBal( 	int nBal , TSIsoInfo * IsoInfo , 
	char      * Field38 , 
	char      * Field39 
	)
{
	TSBalExchange			sBalMsg;
	TSBalHeader				sHeader;
	char					sSecurityData[100];
	int						nLength;  
	int						nMsgType;
	char					sLine[MAX_LINE_TRC];
	char					SourceNodeId[NODE_ID_LEN + 1];
	char					DestNodeId[NODE_ID_LEN + 1];
	int						nBalType;
	char					sDestinationPid[PROC_PID_LEN + 1];
	int						nDestinationPid;
	int						nCheckStatus =0;

	trace_event ("Start ReplyIsoMsgToBal()", PROCESSING );

	memset( sSecurityData,0,sizeof(sSecurityData));
	memset( SourceNodeId,0,sizeof(SourceNodeId));
	memset( DestNodeId,0,sizeof(DestNodeId));
	memset( sDestinationPid,0,sizeof(sDestinationPid));

	ReplyIsoMsg ( IsoInfo , Field38 , Field39 );


	GetIsoField( ISO_SECURITY_DATA , IsoInfo , sSecurityData , &nLength ); 
	GetSecurityData( SOURCE_NODE_ID , sSecurityData ,SourceNodeId, &nLength);
	GetSecurityData( DESTINATION_NODE_ID , sSecurityData ,DestNodeId, &nLength);

	GetSecurityData( DESTINATION_PID , sSecurityData ,sDestinationPid, &nLength);/*IBO20230831 PLUTONL-6866*/
	nDestinationPid = atoi( sDestinationPid );/*IBO20230831 PLUTONL-6866*/

	nMsgType = IsoInfo->nMsgType;

	if (nBal == nBalMsg)
	{
		if (memcmp(DestNodeId, SourceNodeId, NODE_ID_LEN) != 0)
		{
			sBalMsg.sHeader.nPartnerId = getuid();
			nBalType = nBalBrr;
		} 
		else 
		{
			sBalMsg.sHeader.nPartnerId = nDestinationPid;/*IBO20230831 PLUTONL-6866*/
			nBalType = nBalMsg;
			nCheckStatus = 1;	/*IBO20230831 PLUTONL-6866*/
		}
	} 
	else 
	{
		sBalMsg.sHeader.nPartnerId = nDestinationPid; /*IBO20230831 PLUTONL-6866*/
		nBalType = nBal;
		nCheckStatus = 1;	/*IBO20230831 PLUTONL-6866*/
	} 
	
	/*PROD00055494(4): make sure the interface is still UP*/
	if( nCheckStatus ==1 && (nDestinationPid == 0 || kill ( nDestinationPid , 0 ) != 0 ))
	{
		sprintf(sLine,">>> Interface is down[%d], ignoring message",nDestinationPid);
		trace_event (sLine, ERROR );
		return NOK;
	}

	sBalMsg.sHeader.nSourceId   = getpid();
	sBalMsg.sHeader.nCommand    = nMsgType;
	sBalMsg.sHeader.msgId		= IsoInfo->msgId;

	nLength = IsoBuildMsg( sBalMsg.sBody , IsoInfo );
	sBalMsg.sHeader.nLen = nLength;


	if ( WriteBalMsg( nBalType , &sBalMsg ) != OK )
	{
		trace_event ("Failed to send reply request", PROCESSING );
		trace_event ("End ReplyIsoMsgToBal(NOK)", PROCESSING );
		return ( NOK );
	}

	trace_event ("End ReplyIsoMsgToBal(OK)", PROCESSING );
	return( OK );
}

int SendStopIsoMessage( char *Resource , pid_t nPid )
{
   char             data[256];
   char             date_gmt[11 + 1];
   int              data_Lgth;
   TSTlvPrivate  TlvInfo;
   char          tmpField[256];
   char          sPid[8+1];
   int           tmpFieldLen;
   time_t          nPurgeTime;
   char            sPurgeTime[ 9 + 1];
   int                   nLength;  
   TSBalExchange         sBalMsg;
   TSIsoInfo             IsoInfo;
   char               sLine[MAX_LINE_TRC];

   InitIsoInfo( &IsoInfo );
   IsoInfo.nMsgType = 1606;
   memset ( sPurgeTime , 0 ,  9 + 1 );

   /* CHAMP 7  : XMIT DATE AND TIME */
       memset ( date_gmt , 0 , sizeof ( date_gmt ));
       current_gmt_date(date_gmt);
       AddIsoField( ISO_XMIT_TIME , &IsoInfo , date_gmt + 2  , 10 );

   /* CHAMP 24 : FUNCTION CODE */
       AddIsoField( ISO_FUNCTION_CODE , &IsoInfo , "600"  , 3 );

   /* CHAMP 25 : REASON CODE  */
      AddIsoField( ISO_MSG_REASON , &IsoInfo , "6600"  , 4 );

/*************************************/
/* Field 44: Additional Response Data */
/* BAL_TYPE */
/* Field 44: Additional Response Data */
   /* 48 in TLV format. */

   InitTlvPrivate (&TlvInfo);
   AddTlvPrivate (BAL_TYPE_TAG , &TlvInfo, "ADM" , 3 );
   tmpFieldLen = PrivateBuildTlv (tmpField, &TlvInfo);
   AddIsoField (ISO_ADTNL_RESP_DATA , &IsoInfo, tmpField, tmpFieldLen);


   /* CHAMP 53 : SECURITY DATA  */
   memset(tmpField,0,sizeof(tmpField));
   PutSecurityData(SECURITY_FORMAT  , tmpField , "00" , 2);
   PutSecurityData(PIN_BLOCK_FORMAT , tmpField , "00" , 2);
   PutSecurityData(PIN_ENCRYPTION_KEY_INDEX , tmpField , "000" , 3);
   PutSecurityData(MAC_KEY_INDEX , tmpField , "000" , 3);

   memset( sPid , 0, sizeof ( sPid ));
   sprintf (sPid , "%08d", getpid());
   PutSecurityData(SOURCE_PID , tmpField , sPid  , 8);
   PutSecurityData(SOURCE_RESOURCE , tmpField , "000000"  , 6);

   memset( sPid , 0, sizeof ( sPid ));
   sprintf (sPid , "%08d", nPid );
   PutSecurityData(DESTINATION_PID , tmpField ,  sPid  , 8);
   PutSecurityData(DESTINATION_RESOURCE , tmpField , Resource , 6);
   PutSecurityData(USER , tmpField , "000000000000000"  , 15);



   memset ( sPurgeTime , '0',  9 );
   PutSecurityData( PURGE_TIME, tmpField , sPurgeTime ,9);
   AddIsoField (ISO_SECURITY_DATA, &IsoInfo, tmpField, strlen(tmpField));

   memset ( (char *)&sBalMsg , 0, sizeof ( TSBalExchange ));

   sprintf(sLine, "\nsBalMsg.sHeader.nPartnerId => %d.\nsBalMsg.sHeader.nSourceId=> %d.\nsBalMsg.sHeader.nCommand=> %d\n" , sBalMsg.sHeader.nPartnerId, sBalMsg.sHeader.nSourceId, sBalMsg.sHeader.nCommand);
   
   sBalMsg.sHeader.nPartnerId  = nPid;
   sBalMsg.sHeader.nSourceId   = getpid();
   sBalMsg.sHeader.nCommand    = 1606;
   nLength = IsoBuildMsg( sBalMsg.sBody , &IsoInfo );
   sBalMsg.sHeader.nLen = nLength;

   if ( WriteBalMsg( nBalAdm , &sBalMsg ) != OK )
   {
      trace_event ("Send Stop Message Failed", PROCESSING );
      return ( NOK );
   }
   return ( OK );

}

int SendSatusLineInfo( char *Resource , pid_t nPid , int nStatus)
{
   char             data[256];
   char             date_gmt[10 + 1];
   int              data_Lgth;
   TSTlvPrivate  TlvInfo;
   char          tmpField[256];
   char          sPid[8+1];
   int           tmpFieldLen;
   time_t          nPurgeTime;
   char            sPurgeTime[ 9 + 1];
   int                   nLength;  
   TSBalExchange         sBalMsg;
   TSIsoInfo             IsoInfo;

   InitIsoInfo( &IsoInfo );
   IsoInfo.nMsgType = 1606;
   memset ( sPurgeTime , 0 ,  9 + 1 );

   /* CHAMP 7  : XMIT DATE AND TIME */
       memset ( date_gmt , 0 , sizeof ( date_gmt ));
       current_gmt_date(date_gmt);
       AddIsoField( ISO_XMIT_TIME , &IsoInfo , date_gmt + 2  , 10 );

   /* CHAMP 24 : FUNCTION CODE */
   if ( nStatus == COM_UP)
       AddIsoField( ISO_FUNCTION_CODE , &IsoInfo , "611"  , 3 );
   else
       AddIsoField( ISO_FUNCTION_CODE , &IsoInfo , "610"  , 3 );

   /* CHAMP 25 : REASON CODE  */
      AddIsoField( ISO_MSG_REASON , &IsoInfo , "6600"  , 4 );

/* Field 48: Additional Private Data, it contains VISA fields 44 & */
   /* 48 in TLV format. */
   InitTlvPrivate (&TlvInfo);
   AddTlvPrivate (BAL_TYPE_TAG , &TlvInfo, "ADM" , 3 );
   tmpFieldLen = PrivateBuildTlv (tmpField, &TlvInfo);
   AddIsoField (ISO_ADTNL_DATA_PRIV, &IsoInfo, tmpField, tmpFieldLen);


   /* CHAMP 53 : SECURITY DATA  */
   PutSecurityData(SECURITY_FORMAT  , tmpField , "00" , 2);
   PutSecurityData(PIN_BLOCK_FORMAT , tmpField , "00" , 2);
   PutSecurityData(PIN_ENCRYPTION_KEY_INDEX , tmpField , "000" , 3);
   PutSecurityData(MAC_KEY_INDEX , tmpField , "000" , 3);

   memset( sPid , 0, sizeof ( sPid ));
   sprintf (sPid , "%08d", getpid());
   PutSecurityData(SOURCE_PID , tmpField , sPid  , 8);
   PutSecurityData(SOURCE_RESOURCE , tmpField , "000000"  , 6);

   memset( sPid , 0, sizeof ( sPid ));
   sprintf (sPid , "%08d", nPid );
   PutSecurityData(DESTINATION_PID , tmpField ,  sPid  , 8);
   PutSecurityData(DESTINATION_RESOURCE , tmpField , Resource , 6);
   PutSecurityData(USER , tmpField , "000000000000000"  , 15);



   memset ( sPurgeTime , '0',  9 );
   PutSecurityData( PURGE_TIME, tmpField , sPurgeTime ,9);
   AddIsoField (ISO_SECURITY_DATA, &IsoInfo, tmpField, strlen(tmpField));

   sBalMsg.sHeader.nPartnerId  = nPid;
   sBalMsg.sHeader.nSourceId   = getpid();
   sBalMsg.sHeader.nCommand    = 1606;
   nLength = IsoBuildMsg( sBalMsg.sBody , &IsoInfo );
   sBalMsg.sHeader.nLen = nLength;

   if ( WriteBalMsg( nBalAdm , &sBalMsg ) != OK )
   {
      trace_event ("Send Status Line Info Message Failed", PROCESSING );
      return ( NOK );
   }
   return ( OK );

}

void tlvcpy ( char *destination, char *source, int len )
{
    memcpy ( destination  , source , len  );
    destination[ len ] = 0;
}
/*OBE08022007*/
int InsertErrorMsg(TSIsoInfo * IsoInfo ,char *sErrMsg, int nLen)
{
	TSTlvPrivate     TlvInfo;
	char                     szWork[LG_MAX];
	int                              nLenWork;

	InitTlvPrivate (&TlvInfo);
	memset(szWork, 0, sizeof(szWork));
		
	if (GetIsoField (ISO_ADTNL_DATA_PRIV , IsoInfo, szWork, &nLenWork) == SUCCESS)
	{
		AnalyseTlvPrivate (szWork, nLenWork, &TlvInfo);
		PutTlvPrivate (MESSAGE_TEXT_TAG , &TlvInfo, sErrMsg , nLen);
		memset(szWork, 0, sizeof(szWork));
		nLenWork = PrivateBuildTlv (szWork, &TlvInfo);
		if ( nLenWork > 0 )
			PutIsoField (ISO_ADTNL_DATA_PRIV , IsoInfo, szWork , nLenWork);
	}
	else
	{
		AddTlvPrivate (MESSAGE_TEXT_TAG , &TlvInfo, sErrMsg , nLen);
		memset(szWork, 0, sizeof(szWork));
		nLenWork = PrivateBuildTlv (szWork, &TlvInfo);
		if ( nLenWork > 0 )
			PutIsoField (ISO_ADTNL_DATA_PRIV , IsoInfo, szWork , nLenWork);
	}
return ( OK ); 
}
/*OBE08022007*/


int ResourceSsl ( char *sCommsId)
{
	if ( memcmp (sCommsId , P7_COMMS_TCP_IP_SSL, RES_COMMS_ID_LEN) == 0)
		return (1);
	return (NOK);
}

int GetResInfoData (int subfield,char *sResourceInfoData, char *value ,int *length)
{
    
	memcpy(value, sResourceInfoData + res_info_data[subfield].offset, res_info_data[subfield].length);
    value [res_info_data[subfield].length] = '\0';
	*length = res_info_data[subfield].length;
    return( OK );
}


int CheckResourceLive(char* sResourceInfo)
{

	char		sLine[MAX_LINE_TRC];
	char		sPid[PROC_PID_LEN + 1];
	char		sResLive[4];
	char		sResProcStep[8];
	int			nLength;
	pid_t		nPid;

	memset(sPid,'\0',sizeof(sPid));
	memset(sResLive,'\0',sizeof(sResLive));
	memset(sResProcStep,'\0',sizeof(sResProcStep));

	
	GetResInfoData(RES_INF_RES_LIVE,sResourceInfo,sResLive,&nLength);
	GetResInfoData(RES_INF_RES_PS,sResourceInfo,sResProcStep,&nLength);
	GetResInfoData(RES_INF_RES_PID,sResourceInfo,sPid,&nLength);

	if( sResLive[0] != 'Y' || memcmp(sResProcStep,SIGNON,3) != 0 )
	{
		sprintf(sLine,">> Resource down [%s,%d]",sResourceInfo,nPid);
		trace_event(sLine,ERROR);
		return NOK;
	}

	nPid = atoi(sPid);
	if( kill ( nPid , 0 ) != 0 )
	{
		sprintf(sLine,">> Resource down [%s,%d]",sResourceInfo,nPid);
		trace_event(sLine,ERROR);
		return NOK;
	}

	return OK;
}

/****************************************************************************/
/*  TRACK2_DATA                                                             */
/****************************************************************************/
int GetDinersTrack2Data ( char *sTrack2Data ,
							char *sPan, 
							char *sExpiryDate, 
							char *sServiceCode,
							char *sEffectiveDate, 							
							char *sPvv, 
							char *sCvv1,
							char *sTrack2DataC )
{

	char tmpField [255];
	char tmpField1 [255];
	int i,len, j,k,offset;
	len = strlen ( sTrack2Data );
	memset(tmpField,      0, sizeof(tmpField));
	memset(tmpField1,     0, sizeof(tmpField1));
	memset(sTrack2DataC, 0, sizeof(sTrack2DataC));
	trace_event ("GetDinersTrack2Data ..." , PROCESSING);
	j = 0;
	k = 0;
	offset = 0;
   if( len != 0 )
    {
		if(memcmp (sTrack2Data, ";", 1)== 0)
			k = 1;
			
		memcpy (tmpField1, sTrack2Data + k, strlen (sTrack2Data) -k ) ;
			
		for(i=0;(isdigit(tmpField1[i]) != 0); i++ );

		if (memcmp (tmpField1 , "0", 1)== 0)
			{
				memcpy ( sPan           , tmpField1   +1 ,  i -1);
				j = 1;
			}	
		else
				memcpy ( sPan           , tmpField1  ,  i );

			
		memcpy ( sExpiryDate    , tmpField1 + i + 1,  4 );             offset +=4;  /* Expiry date        */
		memcpy ( sServiceCode   , tmpField1 + i + 1 + offset , 3 );    offset +=3;  /* Service Code       */
		memcpy ( sEffectiveDate   , tmpField1 + i + 1 + offset , 4 );  offset +=4; /* effective date     */
		memcpy (sPvv, "1", 1);
		memcpy ( sPvv  + 1      , tmpField1 + i + 1 + offset ,  3 );     offset +=3;  /* Discretionary Data */
		memcpy ( sCvv1          , tmpField1 + i + 1 + offset,   3 );     offset +=6;  /* CVV                */
		memcpy ( sPvv  + 4      , tmpField1 + i + 1 + offset ,    1 );   offset +=2;  /* Discretionary Data */
		
		
		
		
       if (strlen ( sCvv1          ) == 0)   /* To Combat Fraud */
           memcpy ( sCvv1          , "000" ,3 );

		offset = strlen(sPan);
		memcpy (tmpField , sPan, offset);
		memcpy (tmpField + offset, "=", 1);          offset +=1;
		memcpy (tmpField + offset, sExpiryDate,  4); offset +=4;
		memcpy (tmpField + offset, sServiceCode, 3); offset +=3;
		memcpy (tmpField + offset, sPvv,         5); offset +=5;
		/*memcpy (tmpField + offset, "0",          1); offset +=1;*/ /*AMER20170202*/
		memcpy (tmpField + offset, sCvv1,        3); offset +=3;
		
		
		
		memcpy (sTrack2DataC, tmpField , strlen(tmpField));
		

	}

	return(OK);
}
/****************************************************************************/
/*  TRACK1_DATA                                                             */
/****************************************************************************/
int GetDinersTrack1Data ( char *sTrack1Data ,
                    char *sPan, 
					char *sExpiryDate, 
					char *sServiceCode, 
					char *sPvv, 
					char *sCvv1)
{
   char tmpField [255];
   char tmpField1 [255];
   int i,j,len,k,ofsset;
   len = strlen ( sTrack1Data );
   k=0;
   
   if( len != 0 )
   {
   
		if(memcmp (sTrack1Data, "%", 1)== 0)
		{	k = 1; trace_event("k ====== 1" , PROCESSING);}
			
		memcpy (tmpField1, sTrack1Data + k, strlen (sTrack1Data) -k ) ;
			
			
       for(i=0; tmpField1[i] != TRACK1_SEPARATOR ; i++ );
       j = i+1;

       memcpy ( sPan           , tmpField1 + 1  ,  i  - 1 );  /* Card Number */
	   ofsset =0;
	   
       for(i=j; tmpField1[i] != TRACK1_SEPARATOR ; i++ ); 
     
       memcpy ( sExpiryDate    , tmpField1 + i + 1,  4 ); 	        ofsset += 4;   /* sExpiryDate */
       memcpy ( sServiceCode   , tmpField1 + i + 1 + ofsset , 3 );  ofsset += 3;   /* sServiceCode */
	   ofsset += 4;                                                                /* effective date */
	   memcpy ( sPvv       , tmpField1 + i + 1 + ofsset , 5 );      ofsset += 13;  /* Discretionary data */
	   memcpy ( sCvv1  , tmpField1 + i + 1 + ofsset  , 3 );         ofsset += 6;   /* CVV */
       
	   if (memcmp (sCvv1, "   " , 3) == 0)
			memcpy (sCvv1 , "000" , 3 );
		
     
       
   }
  return(OK);
}


int get_pin_info_data ( char* sData, int* nPvki, char* sPvv, char* szOffset, int* tPinLength)
{

	char	sFieldData[256];

	memcpy(szOffset, sData + CSID_IBM_OFST_OFST, CSID_IBM_OFST_LEN);
	szOffset[CSID_IBM_OFST_LEN]='\0';

	memcpy(sPvv, sData + CSID_VISA_PVV_OFST, CSID_VISA_PVV_LEN);
	sPvv[CSID_VISA_PVV_LEN]='\0';
			
	memcpy(sFieldData, sData + CSID_PIN_LEN_OFST, CSID_PIN_LEN_LEN);
	sFieldData[CSID_PIN_LEN_LEN]='\0';
	(*tPinLength) = atoi(sFieldData);

	memcpy(sFieldData, sData + CSID_PVKI_OFST, CSID_PVKI_LEN);
	sFieldData[CSID_PVKI_LEN]='\0';
	(*nPvki) = atoi(sFieldData);

	if( debug_option == 1 )
	{
		sprintf(sFieldData,"pin info(%s,%d,%d,%s,%s)",sData,(*nPvki),(*tPinLength),sPvv,szOffset);
		trace_event(sFieldData, TRACE);
	}
	return OK;
}


int get_cvki ( char* sData, int* nCvki)
{
	char	sCvki[CSID_CVKI_LEN +1];

	memset(sCvki, 0, sizeof(sCvki));

	memcpy(sCvki, sData + CSID_CVKI_OFST, CSID_CVKI_LEN);
	sCvki[CSID_CVKI_LEN]='\0';
	(*nCvki) = atoi(sCvki);

	return OK;
}


void strToUpper(char* string)
{
    int i = 0;
    for (i = 0; string[i] != '\0' ; i++)
    {
        string[i] = toupper(string[i]);
    }
}

/*ICH20191110*/
int ctrlAllowedChar(char* sData)
{
	char 	sLine[MAX_LINE_TRC];
    int 	i = 0;

    for (i = 0; i<strlen(sData) ; i++)
    {
		if (  (sData[i] < 'a' || sData[i] > 'z')
			&& (sData[i] < 'A' || sData[i] > 'Z')
			&& (sData[i] < '0' || sData[i] > '9')
			&& (sData[i] != '.')
			&& (sData[i] != '_')
			&& (sData[i] != '?')
			&& (sData[i] != '/')
			&& (sData[i] != '-')
			&& (sData[i] != ' ')
			 )
		{
		 sprintf(sLine,"The charcter (%c) is not permitted",sData[i]);
		 trace_event ( sLine, ERROR );
		 return ( NOK );
		}  

    }

	 return (OK);
}
