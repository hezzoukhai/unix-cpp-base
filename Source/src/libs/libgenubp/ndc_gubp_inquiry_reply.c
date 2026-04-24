/******************************************************************************/
/*   (c) Hightech Payment Systems 1997                                        */
/*       Authorisation Management for Ndc connecxion V 2.00 	              */
/******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <malloc.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <tlv_private.h>

#include <queue.h>
#include <ascii.h>
#include <card.h>
#include <define.h>
#include <command.h>
#include <ndc_protocol.h>
#include <prints.h>
#include <atmopcode.h>
#include <langue.h>
#include <paramdef.h>
#include <atmopcode.h>
#include <ndc_param.h>
#include <thr_ndc_extern.h>
#include <iso_hps.h>
#include <gubp_opcode.h>
#include <tlv_fields.h>

/* =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int   Format_Output_Label(
							int                     nIndexCtx,
							char                    lReconcData[ ],
							int                     y_pos,
							char                    *ubpLabel,
							char                    lang)
{
	char     sLine[MAX_LINE_TRC];
	int       ubpLabelLen;
	char      sOutPut_1       [120];
	char      sOutPut_2       [120];
	char      sOutPut_3       [120];
	char      sOutPut_4       [120];
	char      sOutPut_5       [120];
	char      sActionCode   [120];
	int      lgField;
	int      cpt;
	int      retour;
	int      Iamount;
	int      Ilen = 0;
 
         

	sprintf(sLine, "Start Format_Output_Label(%c)", y_pos);
	trace_event(sLine, PROCESSING);

	/*
	- initialisation des variables ------------------------------------
	*/
	ubpLabelLen = 0;
	memset(sOutPut_1,       0, sizeof(sOutPut_1)      );
	memset(sOutPut_2,       0, sizeof(sOutPut_2)      );
	memset(sOutPut_3,       0, sizeof(sOutPut_3)      );
	memset(sOutPut_4,       0, sizeof(sOutPut_4)      );
	memset(sOutPut_5,       0, sizeof(sOutPut_5)      );
	memset(sActionCode,  	 0, sizeof(sActionCode)      );
 

	/* [MUHAMMAD UMER#100#FLY DUBAI#5/02/2012#CONFIRMED]      */
	/* [MUHAMMAD UMER][#100][02/2012][][]) */
	sprintf(sLine,"ReconcData = (%s))", lReconcData); /* 12345#678#321 */
	trace_event ( sLine, PROCESSING );
    
	 GetTlvBuffer( "001" ,lReconcData,sOutPut_1,&Ilen) ;
     GetTlvBuffer( "002" ,lReconcData,sOutPut_2,&Ilen) ;
	 GetTlvBuffer( "003" ,lReconcData,sOutPut_3,&Ilen) ;
	 GetTlvBuffer( "004" ,lReconcData,sOutPut_4,&Ilen) ;
	 GetTlvBuffer( "005" ,lReconcData,sOutPut_5,&Ilen) ;  
     
 	sprintf(sLine, "sOutPut_1[%s];sOutPut_2[%s];sOutPut_3[%s];sOutPut_4[%s];sOutPut_5[%s]", sOutPut_1,sOutPut_2,sOutPut_3,sOutPut_4,sOutPut_5);
	trace_event(sLine, PROCESSING);
 
 
 
	/* - formatage de ubpLabel --------------------------------------- */
	ubpLabel[ubpLabelLen] = ESC;
	ubpLabelLen+=1;
	memcpy(ubpLabel + ubpLabelLen, "(1", 2);
	ubpLabelLen+=2;

  if(sOutPut_1[0] != '\0')
  {
	ubpLabel[ubpLabelLen] = SI;
	ubpLabelLen+=1;
	ubpLabel[ubpLabelLen] = 'G';
	ubpLabelLen+=1;
	ubpLabel[ubpLabelLen] = 'A';
	ubpLabelLen+=1;
	memcpy(ubpLabel + ubpLabelLen,sOutPut_1, strlen(sOutPut_1));
	ubpLabelLen+=strlen(sOutPut_1);  
  }

  if(sOutPut_2[0] != '\0')
  {
	ubpLabel[ubpLabelLen] = SI;
	ubpLabelLen+=1;
	ubpLabel[ubpLabelLen] = 'H';
	ubpLabelLen+=1;
	ubpLabel[ubpLabelLen] = 'A';
	ubpLabelLen+=1;
	memcpy(ubpLabel + ubpLabelLen,sOutPut_2, strlen(sOutPut_2));
	ubpLabelLen+=strlen(sOutPut_2);
  }
  
  
  if(sOutPut_3[0] != '\0')
  {
	ubpLabel[ubpLabelLen] = SI;
	ubpLabelLen+=1;
	ubpLabel[ubpLabelLen] = 'I';
	ubpLabelLen+=1;
	ubpLabel[ubpLabelLen] = 'A';
	ubpLabelLen+=1;
	memcpy(ubpLabel + ubpLabelLen,sOutPut_3, strlen(sOutPut_3));
	ubpLabelLen+=strlen(sOutPut_3);
  
  }
  
  
  if(sOutPut_4[0] != '\0')
  {
	ubpLabel[ubpLabelLen] = SI;
	ubpLabelLen+=1;
	ubpLabel[ubpLabelLen] = 'J';
	ubpLabelLen+=1;
	ubpLabel[ubpLabelLen] = 'A';
	ubpLabelLen+=1;
	memcpy(ubpLabel + ubpLabelLen,sOutPut_4, strlen(sOutPut_4));
	ubpLabelLen+=strlen(sOutPut_4);
   }

   if(sOutPut_5[0] != '\0')
    {
	ubpLabel[ubpLabelLen] = SI;
	ubpLabelLen+=1;
	ubpLabel[ubpLabelLen] = 'J';
	ubpLabelLen+=1;
	ubpLabel[ubpLabelLen] = 'A';
	ubpLabelLen+=1;
	memcpy(ubpLabel + ubpLabelLen,sOutPut_4, strlen(sOutPut_4));
	ubpLabelLen+=strlen(sOutPut_4);
    }

	sprintf(sLine, "Format_Output_Label(%s)", ubpLabel);
	trace_event(sLine, PROCESSING);

	sprintf(sLine, "End   Format_Output_Label(%d)", ubpLabelLen);
	trace_event(sLine, PROCESSING);
	return (ubpLabelLen);
}



/******************************************************************************/
/* Fonction UbpInquiryReply                                           */
/* ---------------------------------                                          */
/* Description : Fonction qui traite la reponse a envoyer au GAB NCR.         */
/* Entree      : Le buffer des donnes recues du NCR.                          */
/* Sortie      : retourne OK (0) si le traitement est bien effectue,          */
/*               et NOK (-1) dans le cas contraire.                           */
/* Date/Auteur : 10/03/96 Y. KAOUTAR   	                                      */
/* Revision    : 02/12/98 Y. OUADI                                            */
/******************************************************************************/
int	UbpInquiryReply  (
int			nIndexCtx,
char		*Coordination_nbr,
TSIsoInfo   *pP7Buff ,
char		pTransLang,
int			pOpReq,
char 		*sTlvBuffer)
{

	char		sLine			[MAX_LINE_TRC];
	char		tmpField		[LG_MAX];
	char		balanceLabel	[BUF_LG_MAX];
	char        NDCBufferOut	[LG_MAX];
	char		Terminal_Number [    18];
	char		response_label  [    26];
	char		trace_audit_nbr [     7];
	char		screen_nbr		[     4];
	char		up_screen_nbr	[     4];
	char		lScrLang		[     4];
	char		lReceiptCode	[     3 + 1];
	char		label			[    64];
	char		lTransName		[    64];
	char        szActionFlag    [     3];
	char        szActionlabel   [    20];
	char		 mac_data[9];
	int			nIndex = nIndexCtx - RESERVED_THREAD;
	int			lLen;
	int			balanceLabelLen;
	int			labelLen;
	int			lRespCode;
	int			offset;
	int			pickup_order;
	int     	retour;
	int			print_flag;
	int			pCategory;
	int 		lgField;
	int			on_us_flag = OK;
	int 		nFdConnect;
	char        clgField[3];
	char        Card_type_flag[2];
	char        sCard_type[2+1];
	char        lRecordData        [  1024];
	
	char        ResponseCode[3 + 1];  
	char        sMsgId[23 + 1];       
	TSTlvPrivate  TlvInfo;



	sprintf(sLine, "Start UbpInquiryReply(%c)",pTransLang);
	trace_event(sLine,PROCESSING);


	pickup_order 	  = 0;

	memset(NDCBufferOut,	0, sizeof(NDCBufferOut)		);
	memset(response_label,	0, sizeof(response_label)	);
	memset(Terminal_Number,	0, sizeof(Terminal_Number)	);
	memset(screen_nbr,		0, sizeof(screen_nbr)		);
	memset(lScrLang,		0, sizeof(lScrLang)		);
	memset(lReceiptCode,	0, sizeof(lReceiptCode)		);
	memset(tmpField,		0, sizeof(tmpField)		);
	memset(szActionFlag,    0, sizeof(szActionFlag)     );
	memset(szActionlabel,   0, sizeof(szActionlabel)        );
	memset(lRecordData,          0, sizeof(lRecordData)      );
	memset(Card_type_flag,          0, sizeof(Card_type_flag)      );
	memset(ResponseCode,          0, sizeof(ResponseCode)      );
	memset(sMsgId,          0, sizeof(sMsgId)      );
	memset(mac_data,          0, sizeof(mac_data)      );
	memset(lTransName,          0, sizeof(lTransName)      );
	memset(up_screen_nbr,          0, sizeof(up_screen_nbr)      );
	memset(sCard_type,          0, sizeof(sCard_type)      );

	tab_NdcCtx[nIndex].pkPrn->len = 0;
	tab_NdcCtx[nIndex].pkScr->len = 0;

	offset = 0;
	pCategory = pOpReq;

	switch(pTransLang)
	{
	case ARABIC         : memcpy(lScrLang,SCR_ARABIC,   3); break;
	case ENGLISH        : memcpy(lScrLang,SCR_ENGLISH,  3); break;
	case FRENSH         : memcpy(lScrLang,SCR_FRENSH,   3); break;
	default             : memcpy(lScrLang,SCR_ANY_LANG, 3);
	}

	dump_p7_struct(pP7Buff);

	GetIsoField(ISO_AUDIT_NBR, pP7Buff, trace_audit_nbr, &lgField);
	/*GetIsoField(ISO_RECORD_DATA, pP7Buff, lRecordData, &lgField); */
	 GetTlvBuffer(AUTO_UF_FILE_RECORD ,sTlvBuffer,lRecordData,&lLen); 
	/*memcpy(lRecordData,tab_NdcCtx[nIndex].gRecordData,strlen(tab_NdcCtx[nIndex].gRecordData));*/


/*Start ICH20170126 New TSN Mng*/
	if ( GetIsoField(ISO_ADTNL_DATA_PRIV, pP7Buff, tmpField, &lgField)== OK )
	{
		InitTlvPrivate(&TlvInfo);
		AnalyseTlvPrivate(tmpField, lgField, &TlvInfo);
		GetTlvPrivate(EXTERNAL_STAN_TAG, &TlvInfo, trace_audit_nbr, &lgField);
	}
/*End ICH20170126 New TSN Mng*/


	if ( GetIsoField(ISO_RESPONSE_CODE, pP7Buff, ResponseCode, &lgField) == SUCCES)
		lRespCode = atoi(ResponseCode );
	else
		lRespCode = SYSTEM_MALFUNCTION;

	if ( GetActionInfo( nIndexCtx,ResponseCode,szActionFlag,szActionlabel) != OK )
		trace_event("GetActionFlag (NOK) ",PROCESSING);

	/*
	- formatege du NDCBufferOut ----------------------------------------
	*/

#ifdef NDC_MAC				
	Prepare_Header_Reply_MAC(TRANSACTION_REPLY, tab_NdcCtx[nIndex].Luno,tab_NdcCtx[nIndex].Tvn, NDCBufferOut + offset);
	offset += 22;		
#else
	Prepare_Header_Reply(TRANSACTION_REPLY, Terminal_Number, NDCBufferOut + offset);
	offset += 8;
#endif
	

	 
	/* - Next id data ----------------------------------------------------- */
	if (lRespCode == 0)
		Get_Reply_Data(	nIndexCtx,
						lRespCode, 
						pOpReq, 
						pOpReq,
						tab_NdcCtx[nIndex].terminal,
						tab_NdcCtx[nIndex].AcquirerBank,						
						sCard_type,/*CARD TYPE*/
						tab_NdcCtx[nIndex].Master_idx,
						NDCBufferOut + offset, 
						screen_nbr, 
						up_screen_nbr);
	else 
		Get_Reply_Data(	nIndexCtx,
						lRespCode, 
						pOpReq, 
						-1,
						tab_NdcCtx[nIndex].terminal,
						tab_NdcCtx[nIndex].AcquirerBank,  						
						sCard_type,/*CARD TYPE*/
						tab_NdcCtx[nIndex].Master_idx,					
						NDCBufferOut + offset, 
						screen_nbr, 
						up_screen_nbr);
	offset+=3;

 
	/* - Field Separator -------------------------------------------------- */
	NDCBufferOut[offset] = FS;	 				offset+=1;


	/* - Field Separator -------------------------------------------------- */
	NDCBufferOut[offset] = FS;	 				offset+=1;


	/* - Transaction Serial Number ---------------------------------------- */
	sprintf(sLine,"Transaction Serial Number             : |%s|", trace_audit_nbr);
	trace_event(sLine,TRACE);

	memcpy(NDCBufferOut + offset, trace_audit_nbr + 2, 4);		offset+=4;


	/* - Function identifier --------------------------------------------- */
	NDCBufferOut[offset++] = NDC_FN_ID_SET_NEXT_STATE_PRINT;

	/* - Screen Field ---------------------------------------------------- */
	if(lRespCode == 0  ) 
	{
		/* - Screen number ---------------------------------------------------*/
		memcpy(NDCBufferOut + offset,	screen_nbr, 3);		offset+=3;

		/* - Screen Data -----------------------------------------------------*/
		retour = GetScreen(	nIndexCtx,
							tab_NdcCtx[nIndex].terminal,  
			tab_NdcCtx[nIndex].AcquirerBank ,
							screen_nbr, 
							lScrLang, 
							tab_NdcCtx[nIndex].pkScr);
		if(retour == OK)
		{
			memcpy(NDCBufferOut + offset -3, tab_NdcCtx[nIndex].pkScr->data, 3);  
			memcpy(NDCBufferOut + offset, tab_NdcCtx[nIndex].pkScr->data, tab_NdcCtx[nIndex].pkScr->len); offset+=tab_NdcCtx[nIndex].pkScr->len;

			/* - Balance Label -----------------------------------------------*/ 
			memset(balanceLabel, 0, sizeof(balanceLabel));
			balanceLabelLen = Format_Output_Label (nIndex,
				lRecordData,
				'I', 
				balanceLabel,
				pTransLang);
			if (balanceLabelLen > 0)
			{
				memcpy(NDCBufferOut + offset,balanceLabel, balanceLabelLen);	
				offset+=balanceLabelLen; 
			}			
		}
	}
	else
	{
		/* - Screen number ---------------------------------------------------*/
		memcpy(NDCBufferOut + offset,	screen_nbr, 3);		offset+=3;

		/* - Screen Data -----------------------------------------------------*/
		retour = GetScreen(	nIndexCtx,
							tab_NdcCtx[nIndex].terminal, 
							tab_NdcCtx[nIndex].AcquirerBank, 
							screen_nbr, 
							lScrLang, 
							tab_NdcCtx[nIndex].pkScr);
		if(retour == OK)
		{
			memcpy(NDCBufferOut + offset -3, tab_NdcCtx[nIndex].pkScr->data, 3);  
			memcpy(NDCBufferOut + offset, tab_NdcCtx[nIndex].pkScr->data, tab_NdcCtx[nIndex].pkScr->len); offset+=tab_NdcCtx[nIndex].pkScr->len;

			/* - Response Label ----------------------------------------------*/
			memset(balanceLabel, 0, sizeof(balanceLabel));
			memset(lRecordData, 0, sizeof(lRecordData));
			if ( GetIsoField( ISO_KEY_MNG_DATA , pP7Buff, lRecordData, &lgField) == SUCCES )
			{			       
				balanceLabelLen = Format_Error_Label (nIndex,
					lRecordData,
					'I', 
					balanceLabel,
					pTransLang);
				if (balanceLabelLen > 0)
				{
					memcpy(NDCBufferOut + offset,balanceLabel, balanceLabelLen);	
					offset+=balanceLabelLen; 
				}

			}
			memset(label, 0, sizeof(label));


		}
	}


	/*
	- Field Separator --------------------------------------------------
	*/
	NDCBufferOut[offset] = FS;	 				offset+=1;

	/*
	- Message Co-ordination Number -------------------------------------
	*/
	memcpy(NDCBufferOut + offset, Coordination_nbr + 1, 1);		offset+=1;

	/*
	- Card Return/Retain Flag ------------------------------------------
	*/
	NDCBufferOut[offset++] = NDC_CRRF_CARD_RETURN;
	
	memcpy(NDCBufferOut + offset, "0", 1);		offset += 1;

	/*
	- envoi du buffer NDCBufferOut -----------------------------
	*/
#ifdef NDC_MAC
	Generate_The_Mac(nIndexCtx , tab_NdcCtx[nIndex].terminal , NDCBufferOut , offset,mac_data);
	NDCBufferOut[offset] = FS;
	offset += 1;
	memcpy(NDCBufferOut + offset, mac_data, 8);
	offset += 8;
#endif


	
	nFdConnect = GetFdByOraCtx(nIndexCtx);
	retour = WriteLineMsg(nFdConnect,NDCBufferOut, offset); 

	/* - sauvegarde du context --------------------------------------------*/
	InitIsoInfo(&tab_NdcCtx[nIndex].gContext.fP7Buff);
	memset(tab_NdcCtx[nIndex].gContext.fCoordNbr, 0, sizeof(tab_NdcCtx[nIndex].gContext.fCoordNbr));

	tab_NdcCtx[nIndex].gContext.fRespCode	= lRespCode;
	tab_NdcCtx[nIndex].gContext.fTransCode	= pOpReq;
	memcpy(tab_NdcCtx[nIndex].gContext.fCoordNbr, Coordination_nbr, strlen(Coordination_nbr));
	memcpy(tab_NdcCtx[nIndex].gTlvBuffer, sTlvBuffer, strlen(sTlvBuffer));
	CopyP7Buffer(pP7Buff, &tab_NdcCtx[nIndex].gContext.fP7Buff);


	trace_event("End   UbpInquiryReply(OK)",PROCESSING);
	return(retour);
}

