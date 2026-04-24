#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <malloc.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <queue.h>
#include <ascii.h>
#include <define.h>
#include <command.h>
#include <ndc_protocol.h>
#include <prints.h>
#include <atmopcode.h>
#include <langue.h>
#include <paramdef.h>
#include <atmopcode.h>
#include <actioncode.h>
#include <ndcmsgcst.h>
#include <ndc_param.h>
#include <thr_ndc.h>
#include <iso_hps.h>
#include <tlv_fields.h>

#define UBP_SCR_NO_AMOUNT_ENTRY				"999"
#define UBP_SCR_NO_INQ_N_PAY				"999"
#define UBP_SCR_NO_INQ_ONLY					"999"
/* input types*/
#define UBP_IT_CLEAR_TEXT					'1'
#define UBP_IT_HIDDEN_TEXT					'2'
#define UBP_IT_TERM							'3'


/* action types */
#define UBP_AT_USER_INPUT				'P'		/* the kind of user input and appropriate state no are to be determined*/
#define UBP_AT_INQ_SMMRY_N_PAY_T		'T'
#define UBP_AT_INQ_SMMRY_N_PAY_C		'C'
#define UBP_AT_INQ_SUMMARY				'I'
#define UBP_AT_PAY_ONLY_T				'p'
#define UBP_AT_PAY_ONLY_C				'c'



/*Move to NDC */
#define UBP_SERV_TP_INQUIRY_ONLY	'I'
#define UBP_SERV_TP_PAYMENT_ONLY	'P'
#define UBP_SERV_TP_INQ_N_PAY		'B'
#define UBP_SERV_TP_NONE			'N'



int FormatInputEntryScreen( 
	int 	 nIndexCtx,
char    *pInputScreen,
char     pTransLang,
char     *pScreen,
int     *pScreenLen);

int GetCustomerInput(int nIndiceCtx, char* Terminal_Number, char* sAcquirerBank, char* sScreen, char* sCustomerInput)
{
	char		NDCBuffer		[BUF_LG_MAX];
	char		BBuffer			[    33];
	char		screen			[   512];
	char        Coord_number    [    33];
	char		sLine			[MAX_LINE_TRC];
	int			screen_len;
	int			offset;
	int			retour;
	int			nFdConnect;
	int			NDCBufferLen;
	char		last_input_char;
	int			nLength;

	memset(NDCBuffer		,		0,	sizeof(NDCBuffer      ));
	memset(BBuffer			,		0,	sizeof(BBuffer      ));
	memset(screen			,		0,	sizeof(screen      ));
	memset(Coord_number		,		0,	sizeof(Coord_number      ));


	offset = 0;
	screen_len = 0;

	Prepare_Header_Int_Reply(DATA_COMMAND, Terminal_Number,	&NDCBuffer);				
	offset += 11;

	NDCBuffer[offset] = INTERACTIVE_TRANSACTION_RESPONSE;		
	offset ++;

	/* Display Flag */
	NDCBuffer[offset] ='1';						offset ++;

	/* Active Keys */
	/* Numeric Keys	*/  NDCBuffer[offset + 0] ='1';
	/* FDK A	*/ 		NDCBuffer[offset + 1] ='0';
	/* FDK B	*/ 		NDCBuffer[offset + 2] ='0';
	/* FDK C	*/ 		NDCBuffer[offset + 3] ='1';
	/* FDK D	*/ 		NDCBuffer[offset + 4] ='1';
	/* Cancel Key	*/  NDCBuffer[offset + 5] ='1';
	/* FDK F	*/ 		NDCBuffer[offset + 6] ='1';
	/* FDK G	*/ 		NDCBuffer[offset + 7] ='0';
	/* FDK H	*/ 		NDCBuffer[offset + 8] ='0';
	/* FDK I	*/ 		NDCBuffer[offset + 9] ='0';

	/* activation des FDK suivant le nombre de comptes */

	offset += 10;

	/* Field Separator */
	NDCBuffer[offset] = FS;						offset ++;

	/* Screen Timer Field */

	memcpy(NDCBuffer + offset, "030" , 3);				offset += 3; /* !!! Attention il est en dure */

	/* Field Separator */
	NDCBuffer[offset] = FS;						offset ++;



	retour =FormatInputEntryScreen(nIndiceCtx,	
		sScreen,																				
		tab_NdcCtx[nIndiceCtx-RESERVED_THREAD].TransLanguage, 
		screen,
		&screen_len);

	if (screen_len > 0	)	
	{
		memcpy(NDCBuffer + offset, screen, screen_len);
		offset += screen_len;
	}

	/* envoi de la reponse interactive */
	nFdConnect = GetFdByOraCtx(nIndiceCtx);
	retour = WriteLineMsg( nFdConnect, (char *)&NDCBuffer, offset );

	retour = ReadLineMsg( nFdConnect, NDCBuffer, &NDCBufferLen );
	if ( retour != OK)
	{
		return QUITTER;
	}

	Download_Field(NDCBuffer, 4, Coord_number, NDCBufferLen);
	Download_Field(NDCBuffer, 10, BBuffer, NDCBufferLen);
	memcpy(tab_NdcCtx[nIndiceCtx-RESERVED_THREAD].Coord_number,Coord_number, strlen(Coord_number));

	nLength = strlen(BBuffer);
	last_input_char = BBuffer[nLength-1];
	sprintf(sLine,"INPUT BBuffer = [%s], Last Input %c",BBuffer , last_input_char);
	trace_event(sLine,TRACE);

	switch(last_input_char)
	{
	case 'T': return QUITTER;
	case 'E':
	case 'F': 
		trace_event ( "Transaction Cancelled By User ...", ERROR ); 
		return 	(REQUEST_CANCELED);		                                     
		break;
	case 'C':
		memcpy(sCustomerInput,BBuffer,nLength);
		sCustomerInput[nLength]='\0';
		return OK;
		break;
	case 'D':
		trace_event ( "Re-Enter Data ...", ERROR ); 
		return RETURN;
	}

	trace_event ( "invalid char ...", ERROR ); 
	return QUITTER;
}


int Ubp_Inquiry_Processing	(	int nIndiceCtx,
								char *sMyResource,
								char *sAuthoTable,
								char *sAcquirerBank,
								char *InternationalRoutCheck, 
								char *sServGatewayFlag,
								char *sBillCalculFlag,
								char *sTlvBuffer ,
								TSIsoInfo *sIsoMsg)
{
	
	char			sLine		[MAX_LINE_TRC];
	int     		nLength;
	int 			nRetCode;
	int				nResult;
	int				Utility_input_count;
	int				nThrIndex;
	char			pTransLang      [12];
	char			sFieldData[128];
	char			pInputScreen    [4];
	char			sInputType[12];
	char			sActionType[12];
	char			sCustomerInput[128];
	char			sServiceType[1 + 1];
	char			sUtility_id[30 + 1];
	int				i;
	int				nOffset;
	char			cMaster_flag      [2];
    


	trace_event("Start Ubp_Inquiry_Processing()",PROCESSING);

	memset(pTransLang      ,		0,	sizeof(pTransLang      )); 
	memset(sFieldData      ,		0,	sizeof(sFieldData      ));
	memset(pInputScreen    ,		0,	sizeof(pInputScreen      ));
	memset(sInputType		,		0,	sizeof(sInputType      ));
	memset(sActionType     ,		0,	sizeof(sActionType      ));
	memset(sServiceType     ,		0,	sizeof(sServiceType      ));
    memset(sUtility_id     ,		0,	sizeof(sUtility_id      ));  
    memset(cMaster_flag     ,		    0,	sizeof(cMaster_flag      )); 
    
 
 
	nThrIndex = nIndiceCtx-RESERVED_THREAD;   
 
    sprintf(cMaster_flag,"%d",tab_NdcCtx[nThrIndex].Master_idx);
    
	sprintf(sLine,"OpReq = [%d] cMaster_flag[%s]", tab_NdcCtx[nThrIndex].OpReq ,cMaster_flag);
	trace_event ( sLine, PROCESSING );

	sprintf(tab_NdcCtx[nThrIndex].gService,"%d",tab_NdcCtx[nThrIndex].OpReq);
	 
 
	
    nResult = get_utility_by_oc(	nIndiceCtx, 
									tab_NdcCtx[nThrIndex].sProfileCode,
									tab_NdcCtx[nThrIndex].AcquirerBank, 
									tab_NdcCtx[nThrIndex].OpReq, 
									cMaster_flag, 
									sUtility_id);
	if( nResult != OK )
	{
		trace_event ( "get_utility_by_oc(ERR) ERROR", ERROR );
		return 	(NOK);
	}
    
    memcpy(tab_NdcCtx[nThrIndex].gUtility,sUtility_id,strlen(sUtility_id));	
	tab_NdcCtx[nThrIndex].PrintFlag = NOK;
	
	
	nResult = fetch_input_count(	nIndiceCtx, 
									tab_NdcCtx[nThrIndex].AcquirerBank,
									sUtility_id, 
									sServiceType,
									&Utility_input_count );

	if ( Utility_input_count == 0 )
	{
		trace_event ( "No Input Data", PROCESSING );
		return 	(NOK);
	}

	if ( tab_NdcCtx[nThrIndex].TransLanguage	== ARABIC)
		memcpy(pTransLang,"ARABIC",6); 
	else 		
		memcpy(pTransLang,"ENGLISH",7);


	i =0;
	nOffset = 4;
	memcpy(tab_NdcCtx[nThrIndex].gRecordData,"0000",4);
	
	i++;

	for ( ; i<= Utility_input_count; i++ )
	{

		nResult = fetch_input_screen(	nIndiceCtx,
										tab_NdcCtx[nThrIndex].AcquirerBank,
										sUtility_id,
										i, 
										pTransLang, 
										pInputScreen,
										sInputType);
		if( nResult != OK )
		{
			trace_event ( "fetch_input_screen() ERROR", ERROR );
			return 	(NOK);
		}

		
			
		memset(sCustomerInput     ,		0,	sizeof(sCustomerInput      ));
		nResult = GetCustomerInput(	nIndiceCtx, 
									tab_NdcCtx[nThrIndex].terminal, 
									tab_NdcCtx[nThrIndex].AcquirerBank, 
									pInputScreen, 
									sCustomerInput);
		if( nResult == RETURN )
		{
			i--;
			continue; 
		}
		else if( nResult  != OK )
		{
			trace_event ( "GetCustomerInput() ERROR", ERROR );
			return NOK;
		}

		sprintf(sLine, "Before Input_Data = (%s)(%d)", tab_NdcCtx[nThrIndex].gRecordData,nOffset);
		trace_event(sLine, PROCESSING);

		nLength = strlen(sCustomerInput);
		sprintf(tab_NdcCtx[nThrIndex].gRecordData+nOffset,"%.3d%.3d%.*s",i, nLength-1,nLength-1,sCustomerInput);

		nOffset += 3 + 3 + nLength-1 ;
			
		sprintf(sLine, "Input_Data = (%s)(%d)len(%d)", tab_NdcCtx[nThrIndex].gRecordData+3,nOffset,strlen(tab_NdcCtx[nThrIndex].gRecordData));
		trace_event(sLine, PROCESSING);
	 
 
	}

	if( sServiceType[0] == UBP_SERV_TP_PAYMENT_ONLY )
	{
		/* we will return back to the ATM, set the op code and come back to the server to do the payment*/
		return OK;
	}
	else if( sServiceType[0] == UBP_SERV_TP_INQUIRY_ONLY || sServiceType[0] == UBP_SERV_TP_INQ_N_PAY)
	{     
                 
		sprintf(tab_NdcCtx[nThrIndex].gRecordData,"%.4d%s",strlen(tab_NdcCtx[nThrIndex].gRecordData)-4   ,tab_NdcCtx[nThrIndex].gRecordData+4 );
		  PutTlvBuffer(AUTO_UF_FILE_RECORD,sTlvBuffer,tab_NdcCtx[nThrIndex].gRecordData,strlen(tab_NdcCtx[nThrIndex].gRecordData));
   		  PrintTlvBuffer(sTlvBuffer);
		 
		if ( UbpInquiry_Serv   ( nIndiceCtx, sTlvBuffer ) != OK ) 
		{
			trace_event ( "Error Returned ...", ERROR );  
			return NOK;
		} 
		return OK;
	}
	else
	{
		sprintf(sLine, "Invalid service type %c", sServiceType[0]);
		trace_event(sLine, PROCESSING);
		return NOK;
	}

	trace_event("End   Ubp_Inquiry_Processing(NOK)",PROCESSING);
	return NOK;

}


/***********************************************************************
**  nom   : FormatInputEntryScreen()            **
**  desc   : formatage de la liste des comptes d'un porteur   **
**  entree   : pAccountList   -> liste des comptes         **
**        pAccountListSize-> taille de la liste des comptes   **
**  sortie   : screen    -> ecran liste des comptes      **
**        screenLen    -> langueur            **
**  auteur   : Younes OUADI (25 Oct 1998)            **
***********************************************************************/
int FormatInputEntryScreen( 
	int 	 nIndexCtx,
char    *pInputScreen,
char     pTransLang,
char     *pScreen,
int     *pScreenLen)
{
	ndc_param   		scr;
	char            	sLine		   	[MAX_LINE_TRC];
	char      		lScrLang    	[ 4];
	char      		lStateNbr    	[ 4];
	int 				nIndex = nIndexCtx - RESERVED_THREAD;
	int      		cpt;
	int      		lRet;

	trace_event("Start FormatInputEntryScreen()", PROCESSING);

	/* - initialisation des variables --------------------------------------- */
	memset(lStateNbr,      0, sizeof(lStateNbr));
	memset(lScrLang,      0, sizeof(lScrLang));
	memset(pScreen,         0, sizeof(pScreen)    );
	memset(sLine,         0, sizeof(sLine)    );
	*pScreenLen = 0;


	switch(pTransLang)
	{
	case ARABIC         : 
		memcpy(lScrLang,SCR_ARABIC,   3); 
		break;
	case ENGLISH        : 
		memcpy(lScrLang,SCR_ENGLISH,  3); 
		break;
	case FRENSH         : 
		memcpy(lScrLang,SCR_FRENSH,   3); 
		break;
	default             : 
		memcpy(lScrLang,SCR_ANY_LANG, 3);
	}


	/* - initialisation de SCREEN ------------------------------------------- */

	lRet = GetScreen(nIndexCtx,tab_NdcCtx[nIndex].terminal,tab_NdcCtx[nIndex].AcquirerBank, pInputScreen, lScrLang, &scr);

	if ( lRet != OK )
	{
		trace_event("End   FormatInputEntryScreen GetScreen(NOK)", PROCESSING);
		return (NOK);
	}

	memcpy(pScreen + *pScreenLen,   scr.data, scr.len);   
	*pScreenLen+=scr.len;


	pScreen[*pScreenLen] = SI;               
	*pScreenLen+=1;      
	memcpy(pScreen + *pScreenLen, "JJ", 2);         
	*pScreenLen+=2;



	trace_event("End   FormatInputEntryScreen(OK)", PROCESSING);
	return (OK);
}



/**********************************************/
/***********************************************************************
**  nom   : Format_Payment_EntryScreen()            **
**  desc   : formatage de la liste des comptes d'un porteur   **
**  entree   : pAccountList   -> liste des comptes         **
**        pAccountListSize-> taille de la liste des comptes   **
**  sortie   : screen    -> ecran liste des comptes      **
**        screenLen    -> langueur            **
**  auteur   : Younes OUADI (25 Oct 1998)            **
***********************************************************************/
int Format_Payment_EntryScreen(
	int 		nIndexCtx,
char      *pInputScreen,
char      pTransLang,
char      pScreen[201],
int      *pScreenLen)
{
	ndc_param   		scr;
	char            	sLine		   	[MAX_LINE_TRC];
	char      		lScrLang    	[ 4];
	char      		lStateNbr    	[ 4];
	int 				nIndex = nIndexCtx - RESERVED_THREAD;
	int      		cpt;
	int      		lRet;

	trace_event("Start Format_Payment_EntryScreen()", PROCESSING);

	/* - initialisation des variables --------------------------------------- */
	memset(lStateNbr,      0, sizeof(lStateNbr));
	memset(lScrLang,      0, sizeof(lScrLang));
	memset(pScreen,         0, sizeof(pScreen)    );
	*pScreenLen = 0;


	switch(pTransLang)
	{
	case ARABIC         : 
		memcpy(lScrLang,SCR_ARABIC,   3); 
		break;
	case ENGLISH        : 
		memcpy(lScrLang,SCR_ENGLISH,  3); 
		break;
	case FRENSH         : 
		memcpy(lScrLang,SCR_FRENSH,   3); 
		break;
	default             : 
		memcpy(lScrLang,SCR_ANY_LANG, 3);
	}


	/* - selection de l'intitule du compte ---------------------------------- */

	/* - initialisation de SCREEN ------------------------------------------- */


	sprintf(sLine,"pInputScreen = [%s]", pInputScreen );
	trace_event ( sLine, PROCESSING );


	lRet = GetScreen(nIndexCtx,tab_NdcCtx[nIndex].terminal,tab_NdcCtx[nIndex].AcquirerBank, pInputScreen, lScrLang, &scr);

	if ( lRet != OK )
	{
		trace_event("End   Format_Payment_EntryScreen GetScreen(NOK)", PROCESSING);
		return (NOK);
	}

	memcpy(pScreen + *pScreenLen,   scr.data, scr.len);   
	*pScreenLen+=scr.len;


	pScreen[*pScreenLen] = SI;               
	*pScreenLen+=1;      
	memcpy(pScreen + *pScreenLen, "JJ", 2);         
	*pScreenLen+=2;




	trace_event("End   Format_Payment_EntryScreen(OK)", PROCESSING);
	return (OK);
}
