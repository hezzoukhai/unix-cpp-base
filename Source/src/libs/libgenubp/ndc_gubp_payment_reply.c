
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
#include  <tlv_autho.h>
#include  <tlv_private.h>

/* =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int   Format_Payment_Output_Label(
int                     nIndexCtx,
char      lReconcData[  1024],
int      y_pos,
char      *ubpLabel,
char      lang)
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
	int      i =0;
	int      j =0;
	int      count =0;


	sprintf(sLine, "Start Format_Payment_Output_Label(%c)", y_pos);
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


	/*[MUHAMMAD UMER#100#FLY DUBAI#5/02/2012#CONFIRMED]      */
	/*[MUHAMMAD UMER][#100][02/2012][][]) */
	sprintf(sLine,"lReconcData = [%s])", lReconcData); /* 12345#678#321 */
	trace_event ( sLine, PROCESSING );


	for (i =1; i<= 5; i++ )
	{	
		for (j = count+1; j< strlen(lReconcData); j++ )
		{	        	
			if ( memcmp(lReconcData + j, "#", 1) == 0 ) 
			break;
		}	      

		if (i == 1)  
		memcpy(sOutPut_1,lReconcData,j);
		if (i == 2)  
		memcpy(sOutPut_2,lReconcData+ count,j-count);
		if (i == 3)  
		memcpy(sOutPut_3,lReconcData+ count,j-count);
		if (i == 4)  
		memcpy(sOutPut_4,lReconcData+ count,j-count);
		if (i == 5)  
		memcpy(sOutPut_5,lReconcData+ count,j-count);

		j+=1;
		count=j;

		sprintf(sLine,"values are= [%d][%d])", count,j); 
		trace_event ( sLine, PROCESSING );
	}

	sprintf(sLine,"Outputs are = [%s][%s][%s][%s][%s])", sOutPut_1,sOutPut_2,sOutPut_3,sOutPut_4,sOutPut_5); 
	trace_event ( sLine, PROCESSING );

	/*
	if ((int)strlen(tab_NdcGUbp[nIndexCtx].gName) > 16)
		Ilen = 16;
	else
		Ilen = (int)strlen(tab_NdcGUbp[nIndexCtx].gName); 	   

	*/

	/* - formatage de ubpLabel --------------------------------------- */
	ubpLabel[ubpLabelLen] = ESC;
	ubpLabelLen+=1;
	memcpy(ubpLabel + ubpLabelLen, "(1", 2);
	ubpLabelLen+=2;


	ubpLabel[ubpLabelLen] = SI;
	ubpLabelLen+=1;
	ubpLabel[ubpLabelLen] = 'G';
	ubpLabelLen+=1;
	ubpLabel[ubpLabelLen] = 'F';
	ubpLabelLen+=1;
	memcpy(ubpLabel + ubpLabelLen,sOutPut_1, strlen(sOutPut_1));
	ubpLabelLen+=strlen(sOutPut_1);

	ubpLabel[ubpLabelLen] = SI;
	ubpLabelLen+=1;
	ubpLabel[ubpLabelLen] = 'H';
	ubpLabelLen+=1;
	ubpLabel[ubpLabelLen] = 'F';
	ubpLabelLen+=1;
	memcpy(ubpLabel + ubpLabelLen,sOutPut_2, strlen(sOutPut_2));
	ubpLabelLen+=strlen(sOutPut_2);

	ubpLabel[ubpLabelLen] = SI;
	ubpLabelLen+=1;
	ubpLabel[ubpLabelLen] = 'I';
	ubpLabelLen+=1;
	ubpLabel[ubpLabelLen] = 'F';
	ubpLabelLen+=1;
	memcpy(ubpLabel + ubpLabelLen,sOutPut_3, strlen(sOutPut_3));
	ubpLabelLen+=strlen(sOutPut_3);

	ubpLabel[ubpLabelLen] = SI;
	ubpLabelLen+=1;
	ubpLabel[ubpLabelLen] = 'J';
	ubpLabelLen+=1;
	ubpLabel[ubpLabelLen] = 'F';
	ubpLabelLen+=1;
	memcpy(ubpLabel + ubpLabelLen,sOutPut_4, strlen(sOutPut_4));
	ubpLabelLen+=strlen(sOutPut_4);

	ubpLabel[ubpLabelLen] = SI;
	ubpLabelLen+=1;
	ubpLabel[ubpLabelLen] = 'K';
	ubpLabelLen+=1;
	ubpLabel[ubpLabelLen] = 'F';
	ubpLabelLen+=1;
	memcpy(ubpLabel + ubpLabelLen,sOutPut_5, strlen(sOutPut_5));
	ubpLabelLen+=strlen(sOutPut_5);


	sprintf(sLine, "Format_Payment_Output_Label(%s)", ubpLabel);
	trace_event(sLine, PROCESSING);

	sprintf(sLine, "End   Format_Payment_Output_Label(%d)", ubpLabelLen);
	trace_event(sLine, PROCESSING);
	return (ubpLabelLen);
}

/*                   DISPLAY ERROR                                         */
/* =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int   Format_Error_Label(
int                     nIndexCtx,
char      lReconcData[  1024],
char      *ubpLabel,
char      lang)
{
	char     sLine[MAX_LINE_TRC];
	int       ubpLabelLen;   
	int      lgField;
	int      cpt;
	int      retour;
	int      Iamount;
	int      Ilen = 0;
	int      i =0;
	int      j =0;
	int      count =0;

	memset(sLine,  	 0, sizeof(sLine)      );

	sprintf(sLine, "Start Format_Error_Label()" );
	trace_event(sLine, PROCESSING);

	/*
	- initialisation des variables ------------------------------------
	*/
	ubpLabelLen = 0; 




	sprintf(sLine,"lReconcData = [%s]", lReconcData); 
	trace_event ( sLine, PROCESSING );


	/* if ((int)strlen(lReconcData) > 16)
	Ilen = 16;
	else
	Ilen = (int)strlen(tab_NdcGUbp[nIndexCtx].gName); 	   */



	/* - formatage de ubpLabel --------------------------------------- */
	ubpLabel[ubpLabelLen] = ESC;
	ubpLabelLen+=1;
	memcpy(ubpLabel + ubpLabelLen, "(1", 2);
	ubpLabelLen+=2;

	ubpLabel[ubpLabelLen] = SI;
	ubpLabelLen+=1;
	ubpLabel[ubpLabelLen] = 'J';
	ubpLabelLen+=1;
	ubpLabel[ubpLabelLen] = 'A';
	ubpLabelLen+=1;
	memcpy(ubpLabel + ubpLabelLen,lReconcData, strlen(lReconcData));
	ubpLabelLen+=strlen(lReconcData);


	sprintf(sLine, "Format_Error_Label(%s)", ubpLabel);
	trace_event(sLine, PROCESSING);

	sprintf(sLine, "End   Format_Error_Label(%d)", ubpLabelLen);
	trace_event(sLine, PROCESSING);
	return (ubpLabelLen);
}
/*****************END Dispaly Error***********************************/



/******************************************************************************/
/* Fonction Ubp_Payment_Reply                                           */
/* ---------------------------------                                          */
/* Description : Fonction qui traite la reponse a envoyer au GAB NCR.         */
/* Entree      : Le buffer des donnes recues du NCR.                          */
/* Sortie      : retourne OK (0) si le traitement est bien effectue,          */
/*               et NOK (-1) dans le cas contraire.                           */
/* Date/Auteur : 10/03/96 Y. KAOUTAR   	                                      */
/* Revision    : 02/12/98 Y. OUADI                                            */
/******************************************************************************/
int	Ubp_Payment_Reply  (
int			nIndexCtx,
char		*Coordination_nbr,
TSIsoInfo   *pP7Buff ,
char		pTransLang,
int			pOpReq,
char 		*sTlvBuffer)
{

	char		sLine			[MAX_LINE_TRC];
	char		tmpField		[LG_MAX];
	char		Iso_AcqBank	 		[6+1   ];
	char		PaymentOutLabel	[LG_MAX];
	char        NDCBufferOut	[LG_MAX];
	char		Terminal_Number [    18];
	char		response_label  [    26];
	char		trace_audit_nbr [     7];
	char        reconc_id       [    11];
	char        reconc_date     [    20];
	char		screen_nbr		[     4];
	char		up_screen_nbr	[     4];
	char		lScrLang		[     4];
	char		lReceiptCode	[     4];
	char		label			[    64];
	char		lTransName		[    64];
	char        szActionFlag    [     4];
	char        szActionlabel   [    20];
	ndc_param	scr;
	ndc_param	prn;
	int			nIndex = nIndexCtx - RESERVED_THREAD;
	int			lLen;
	int			PaymentOutLabelLen;
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
	char         clgField[3];
	char      Card_type_flag[2];
	char      lReconcData        [  1024];
	char      OpCodeData        [  5];
	char        ResponseCode[3 + 1]; 
	char        sMsgId     [23 + 1];      
	char		mac_data   [     9];
	char        sCard_type[2+ 1]; 
	 
	TSTlvPrivate  TlvInfo;

	sprintf(sLine, "Start Ubp_Payment_Reply(%c)",pTransLang);
	trace_event(sLine,PROCESSING);



	pickup_order 	  = 0;

	memset(NDCBufferOut,	0, sizeof(NDCBufferOut)		);
	memset(response_label,	0, sizeof(response_label)	);
	memset(Terminal_Number,	0, sizeof(Terminal_Number)	);
	memset(reconc_id,		0, sizeof(reconc_id)		);
	memset(reconc_date,		0, sizeof(reconc_date)		);
	memset(screen_nbr,		0, sizeof(screen_nbr)		);
	memset(lScrLang,		0, sizeof(lScrLang)		);
	memset(lReceiptCode,	0, sizeof(lReceiptCode)		);
	memset(scr.data,		0, sizeof(scr.data)		);
	memset(tmpField,		0, sizeof(tmpField)		);
	memset(szActionFlag,    0, sizeof(szActionFlag)     );
	memset(szActionlabel,   0, sizeof(szActionlabel)        );
	memset(lReconcData,          0, sizeof(lReconcData)      );
	memset(OpCodeData,          0, sizeof(OpCodeData)      );
	memset(Card_type_flag,          0, sizeof(Card_type_flag)      );
	memset(ResponseCode,    0, sizeof(ResponseCode)      );
	memset(sMsgId,          0, sizeof(sMsgId)      );
	memset(mac_data,        0, sizeof(mac_data)      );
	memset(Iso_AcqBank,		0, sizeof(Iso_AcqBank));
    memset(sCard_type,          0, sizeof(sCard_type)      );

	offset = 0;
	pCategory = pOpReq;


	switch(pTransLang)
	{
	case ARABIC         : memcpy(lScrLang,SCR_ARABIC,   3); break;
	case ENGLISH        : memcpy(lScrLang,SCR_ENGLISH,  3); break;
	case FRENSH         : memcpy(lScrLang,SCR_FRENSH,   3); break;
		default             : memcpy(lScrLang,SCR_ANY_LANG, 3);
	}


	GetIsoField(ISO_AUDIT_NBR, pP7Buff, trace_audit_nbr, &lgField);
	/* GetIsoField(ISO_ADTNL_DATA_PRIV, pP7Buff, tmpField, &lgField);
	InitTlvPrivate(&TlvInfo);
	AnalyseTlvPrivate(tmpField, lgField, &TlvInfo);			
	GetTlvPrivate(ATM_DEPOSIT_OPCODE, &TlvInfo, OpCodeData, &lgField);			
*/ 

	
	memcpy( OpCodeData,tab_NdcCtx[nIndex].gService,4);



	sprintf(sLine, " OpCodeData cntx =[%s]" ,OpCodeData);
	trace_event(sLine, TRACE);


	if ( GetIsoField(ISO_RESPONSE_CODE, pP7Buff, tmpField, &lgField) == SUCCES)
	lRespCode = atoi(tmpField );
	else
	lRespCode = SYSTEM_MALFUNCTION;

	sprintf(sLine, "lRespCode=%d", lRespCode);
	trace_event(sLine, TRACE);

	if ( GetActionInfo( nIndexCtx,tmpField,szActionFlag,szActionlabel) != OK )
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
	/*if(lRespCode == 0  || lRespCode == 160 || lRespCode == 170|| lRespCode == 110|| lRespCode == 121|| lRespCode == 123 )*/
	if(lRespCode == 0 ||  lRespCode == PROCESSING_ERROR )
	{
		Get_Reply_Data(nIndexCtx,lRespCode, atoi(OpCodeData), -1,
		tab_NdcCtx[nIndex].terminal, 
		tab_NdcCtx[nIndex].AcquirerBank,		
		sCard_type,/*CARD TYPE*/
		tab_NdcCtx[nIndex].Master_idx,
		NDCBufferOut + offset, screen_nbr, up_screen_nbr);
		offset+=3;
	}
	else 
	{
		Get_Reply_Data(nIndexCtx,100, atoi(OpCodeData), -1,
		tab_NdcCtx[nIndex].terminal,  
		tab_NdcCtx[nIndex].AcquirerBank,		
		sCard_type,/*CARD TYPE*/
		tab_NdcCtx[nIndex].Master_idx,		
		NDCBufferOut + offset, screen_nbr, up_screen_nbr);
		offset+=3;
	}

	/* - Field Separator -------------------------------------------------- */
	NDCBufferOut[offset] = FS;	 				offset+=1;


	/* - Field Separator -------------------------------------------------- */
	NDCBufferOut[offset] = FS;	 				offset+=1;


	/* - Transaction Serial Number ---------------------------------------- */
	memcpy(NDCBufferOut + offset, trace_audit_nbr + 2, 4);		offset+=4;


	/* - Function identifier --------------------------------------------- */

	NDCBufferOut[offset++] = NDC_FN_ID_SET_NEXT_STATE_PRINT;
 

	/* - Screen Field ---------------------------------------------------- */
	if(lRespCode == 0) 
	{
		/* - Screen number ---------------------------------------------------*/
		memcpy(NDCBufferOut + offset,	screen_nbr, 3);		offset+=3;

		/* - Screen Data -----------------------------------------------------*/
		retour = GetScreen(nIndexCtx,tab_NdcCtx[nIndex].terminal,tab_NdcCtx[nIndex].AcquirerBank, screen_nbr, lScrLang, &scr);
		if(retour == OK)
		{
			memcpy(NDCBufferOut + offset -3, scr.data, 3); 
			memcpy(NDCBufferOut + offset, scr.data, scr.len); offset+=scr.len;

			/* - Balance Label -----------------------------------------------*/
			memset(PaymentOutLabel, 0, sizeof(PaymentOutLabel));
			PaymentOutLabelLen = Format_Payment_Output_Label ( nIndex,
			lReconcData,
			'I', 
			PaymentOutLabel,
			pTransLang);
			if (PaymentOutLabelLen)
			{
				memcpy(NDCBufferOut + offset,PaymentOutLabel, PaymentOutLabelLen);	
				offset+=PaymentOutLabelLen;    
			}   
		}

	}
	else
	{ 

		/* - Screen number ---------------------------------------------------*/
		memcpy(NDCBufferOut + offset,	screen_nbr, 3);		offset+=3;

		/* - Screen Data -----------------------------------------------------*/
		retour = GetScreen(nIndexCtx,tab_NdcCtx[nIndex].terminal,tab_NdcCtx[nIndex].AcquirerBank , screen_nbr, lScrLang, &scr);
		if(retour == OK)
		{
			memcpy(NDCBufferOut + offset -3, scr.data, 3);
			memcpy(NDCBufferOut + offset, scr.data, scr.len); offset+=scr.len;

			/* - Error Response Label Dispaly from Biller ----------------------------------------------*/
			memset(lReconcData, 0, sizeof(lReconcData));

			/*START RJE24032015*/
			memset(label, 0, sizeof(label));
			labelLen = GetResponseLabel(nIndex,
			lRespCode,
			pOpReq, -1,tab_NdcCtx[nIndexCtx].AcquirerBank ,
			pTransLang,
			pOpReq,
			label);			  


			if (labelLen > 0)
			{		       

				sprintf(tmpField, "%s", label);
				PutIsoField(ISO_RESERVED_ISO_2 , pP7Buff, tmpField, labelLen);
				FormatLabel('I', "00", pTransLang, label, &labelLen);
				memcpy(NDCBufferOut + offset,label, labelLen);		
				offset+=labelLen;						

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
	if ( szActionFlag[0] == 'P' )
	{
		memcpy(NDCBufferOut + offset, "1", 1);			offset+=1;
		pickup_order = 1;
	}
	else 
	{
		memcpy(NDCBufferOut + offset,"0", 1);			offset+=1;
	}
 
	/* ISO_TERMINAL_NBR ---- */ 
	if ( GetIsoField(ISO_TERMINAL_NBR , pP7Buff, tmpField, &lgField) == SUCCES
			&& GetIsoField(ISO_ACQR_ID ,pP7Buff, Iso_AcqBank, &lLen)== SUCCES  ) {
		/* trace_audit_nbr + 2 is TSN */
		SetTermTransSeq(nIndexCtx,tmpField, Iso_AcqBank,trace_audit_nbr + 2);
	}


	/*
	- Donnees TICKET -------------------------------------------
	*/
	print_flag = NOK; 


/*NAK TODO*/
	if(lRespCode == 110 )
	memcpy(lReceiptCode, NP_UBP_TIMED_OUT_ENG, 3);				

	else 
	{
		if ( lRespCode == 0 )
		{
			if(pTransLang ==  ARABIC)
			memcpy(lReceiptCode, NP_UBP_TICKET_AR, 3); 			
			else
			memcpy(lReceiptCode, NP_UBP_TICKET_ENG, 3);
		}
		else 
		{
			if(pTransLang ==  ARABIC)
			memcpy(lReceiptCode, NP_UBP_DEC_TICKET_AR, 3);
			else
			memcpy(lReceiptCode, NP_UBP_DEC_TICKET_ENG, 3);
		}
	}
/*NAK TODO*/


	memset(prn.data,            0, sizeof(prn.data));
	retour = NOK;

	retour = GetPrint(nIndexCtx,tab_NdcCtx[nIndex].terminal, tab_NdcCtx[nIndex].AcquirerBank,lReceiptCode,
	pTransLang, pP7Buff, &prn);
	if(retour == OK)
	{
		/*
		- Printer Flag Field --------------------------------
		*/
		memcpy(NDCBufferOut + offset, "2", 1);
		offset += 1;
		memcpy(NDCBufferOut + offset, prn.data, prn.len);
		offset += prn.len;

		/*
		- GROUP SEPARATOR -----------------------------------
		*/
		NDCBufferOut[offset] = GS;
		offset += 1;

		print_flag = OK;
	}
	else
	{
		sprintf(sLine, "- Err-In GetPrint(%.3s) -----------------", lReceiptCode);
		trace_event(sLine, ERROR);
	}

	/*
	- Donnees JOURNAL -------------------------------------------
	*/
	memset(prn.data,                0, sizeof(prn.data));
	retour = NOK;
	
	/*NAK TODO*/
	 retour = GetPrint(nIndexCtx,tab_NdcCtx[nIndex].terminal, tab_NdcCtx[nIndex].AcquirerBank, NP_UBP_JOURNAL_ENG, ENGLISH, pP7Buff, &prn);

	if(retour == OK)
	{
		/*
		- Printer Flag Field --------------------------------
		*/
		memcpy(NDCBufferOut + offset, "1", 1);
		offset += 1;
		memcpy(NDCBufferOut + offset, prn.data, prn.len);
		offset += prn.len;

		print_flag = OK;

	}
	else
	{
		sprintf(sLine, "- Err-In GetPrint(%.3s) -----------------",NP_UBP_JOURNAL_ENG); 
		trace_event(sLine, ERROR);
	}
	 

	if(print_flag == NOK)
	{
		memcpy(NDCBufferOut + offset, "0", 1);              offset += 1;
	}

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


	trace_event("End   Ubp_Payment_Reply()",PROCESSING);
	return(retour);
}

