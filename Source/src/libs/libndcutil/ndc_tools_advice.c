/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      ndc_tools_advice.c                                                     */
/*                                                                             */
/* Description                                                                 */
/*      ATM Advice Tools Management                                            */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
0001MK     		18-02-2002   		Mahjoub KARROUM    		Initiale Version	
LHI050906      05-09-2006      Loubna Hidraoui 			Function Load_Reason_Code ne retourne aucun Reason Code lorsqu’il s’agit d’une transaction EMV. 
Ce qui génère une erreur lors du message reversal
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>

#include <define.h>
#include <ascii.h>
#include <iso_hps.h>
#include <paramdef.h>
#include <langue.h>
#include <ndc_param.h>
#include <thr_ndc.h> 

#include <iso_hps.h>
#include <ndcstatus.h>
#include <string.h>
#include <command.h>

/* =-= -=- =-= -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- */
/* =-= -=- =-= -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- */
int	CardNotTaken(
					int				nIndexCtx,
					char            Coord_Nbr    [ ],
					TSIsoInfo       *P7Buffer    ,
					char			pTransLang)
{
	char      		sLine[MAX_LINE_TRC];
	ndc_param       prn;
	char            NDCBufferOut            [LG_MAX];
	char            lReceiptCode            [     4];
	char            sAuditNbr[20 +1];
	char            mac_data[9];
	int             lgField;
	int				nFdConnect;
	int				lPrintFlag;
	int             offset;
	int             retour;

	trace_event("Start CardNotTaken()", PROCESSING);

	memset(NDCBufferOut,    0, sizeof(NDCBufferOut) );
	memset(lReceiptCode,    0, sizeof(lReceiptCode) );
	memset(mac_data,    0, sizeof(mac_data) );
	memset(sLine,    0, sizeof(sLine) );
	memset(sAuditNbr,    0, sizeof(sAuditNbr) );

	offset = 0;


#ifdef NDC_MAC				
	Prepare_Header_Reply_MAC(TRANSACTION_REPLY, tab_NdcCtx[nIndexCtx].Luno,tab_NdcCtx[nIndexCtx].Tvn, NDCBufferOut + offset);
	offset += 22;		
#else
	Prepare_Header_Reply(TRANSACTION_REPLY, tab_NdcCtx[nIndexCtx].terminal, NDCBufferOut + offset);
	offset += 8;
#endif


	memcpy(NDCBufferOut + offset, "422", 3);                        
	offset+=3;
	NDCBufferOut[offset] = FS;                                      
	offset+=1;
	NDCBufferOut[offset] = FS;                                      
	offset+=1;
	memcpy(NDCBufferOut + offset, "12344", 5);                      
	offset+=5;
	NDCBufferOut[offset] = FS;                                      
	offset+=1;
	NDCBufferOut[offset] = Coord_Nbr[1];                            
	offset+=1;
	memcpy(NDCBufferOut + offset, "1", 1);                          
	offset+=1;



	/* - Donnees Tickets --------------------------------------------------- */
	lPrintFlag = NOK;

	/* - Receipt Language & cancelation reason ----------------------------- */
	switch (pTransLang)
	{
	case ARABIC	:
		memcpy(lReceiptCode,	NP_TRANS_CANCELED_AR, 3);
		PutIsoField(ISO_RESERVED_ISO_2 , P7Buffer,"013\\O#G&U\" Y#_CZ", 16);
		break;
	case FRENSH	:
		memcpy(lReceiptCode,	NP_TRANS_CANCELED_FR, 3);
		PutIsoField(ISO_RESERVED_ISO_2 , P7Buffer,"CARTE OUBLIEE", 13);
		break;
	default		:
		memcpy(lReceiptCode,	NP_TRANS_CANCELED_ENG, 3);
		PutIsoField(ISO_RESERVED_ISO_2 , P7Buffer,"CARD NOT TAKEN", 14);
	}

	memset(prn.data, 0, sizeof(prn.data));
	prn.len = 0;
	retour = GetPrint(	nIndexCtx+RESERVED_THREAD,
						tab_NdcCtx[nIndexCtx].terminal, 
						tab_NdcCtx[nIndexCtx].AcquirerBank, 
						lReceiptCode, 
						pTransLang, 
						P7Buffer, 
						&prn);
	if(retour == OK)
	{
		/* - Printer Flag Field -------------------------------- */
		memcpy(NDCBufferOut + offset, "2", 1);                  
		offset += 1;
		memcpy(NDCBufferOut + offset, prn.data, prn.len);       
		offset+=prn.len;

		/* - GROUP SEPARATOR ----------------------------------- */
		NDCBufferOut[offset] = GS;                              
		offset += 1;

		lPrintFlag = OK;
	}
	else
	{
		sprintf(sLine, "- Err-In GetPrint(%.3s) -----------------", lReceiptCode);
		trace_event(sLine, ERROR);
	}

	/* - Donnees JOURNAL ------------------------------------------- */
	memset(prn.data,                0, sizeof(prn.data));
	retour = GetPrint(	nIndexCtx+RESERVED_THREAD,
		tab_NdcCtx[nIndexCtx].terminal,
		tab_NdcCtx[nIndexCtx].AcquirerBank,
		NP_PICKUP_TIME_OUT_JOURNAL_ENG,
		ENGLISH,
		P7Buffer,
		&prn);
	if(retour == OK)
	{
		/* - Printer Flag Field -------------------------------- */
		memcpy(NDCBufferOut + offset, "1", 1);                      
		offset+=1;
		memcpy(NDCBufferOut + offset, prn.data, prn.len);           
		offset+=prn.len;

		lPrintFlag = OK;
	}
	else
	{
		sprintf(sLine, "- Err-In GetPrint(%.3s) -----------------", NP_PICKUP_TIME_OUT_JOURNAL_ENG);
		trace_event(sLine, ERROR);
	}

	if ( lPrintFlag == NOK )
	{
		memcpy(NDCBufferOut + offset, "0", 1);                      
		offset+=1;
	}

#ifdef NDC_MAC
	Generate_The_Mac(nIndexCtx+RESERVED_THREAD, tab_NdcCtx[nIndexCtx].terminal , NDCBufferOut , offset ,mac_data);
	NDCBufferOut[offset] = FS;
	offset+= 1;
	memcpy(NDCBufferOut + offset, mac_data, 8);
	offset += 8;
#endif

	
	nFdConnect = GetFdByOraCtx(nIndexCtx+RESERVED_THREAD);
	
	
	/* - envoi du message au GAB -------------------------------------------- */
	retour = WriteLineMsg( nFdConnect,NDCBufferOut, offset );

	memset(NDCBufferOut,    0, sizeof(NDCBufferOut) );
	trace_event("End   CardNotTaken()", PROCESSING);
	return (OK);
}




/* =-= -=- =-= -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- */
/* =-= -=- =-= -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- */
int CashNotTaken(
					int				nIndexCtx,
					char            Coord_Nbr       [   4])
{
	char      		sLine[MAX_LINE_TRC];
	ndc_param       prn;
	char            NDCBufferOut            [LG_MAX];
	TSIsoInfo       P7Buffer                ;
	char            mac_data[9];
	int				nFdConnect;
	int             offset;
	int             retour;

	trace_event("Start CashNotTaken()", PROCESSING);

	memset(NDCBufferOut,    0, sizeof(NDCBufferOut) );
	memset(mac_data,     0, sizeof(mac_data)  );

	offset = 0;

#ifdef NDC_MAC				
	Prepare_Header_Reply_MAC(TRANSACTION_REPLY, tab_NdcCtx[nIndexCtx].Luno,tab_NdcCtx[nIndexCtx].Tvn, NDCBufferOut + offset);
	offset += 22;		
#else
	Prepare_Header_Reply(TRANSACTION_REPLY, tab_NdcCtx[nIndexCtx].terminal, NDCBufferOut + offset);
	offset += 8;
#endif



	memcpy(NDCBufferOut + offset, "411", 3);                        
	offset+=3;
	NDCBufferOut[offset] = FS;                                      
	offset+=1;
	NDCBufferOut[offset] = FS;                                      
	offset+=1;
	memcpy(NDCBufferOut + offset, "12345", 5);                      
	offset+=5;
	NDCBufferOut[offset] = FS;                                      
	offset+=1;
	NDCBufferOut[offset] = Coord_Nbr[1];                            
	offset+=1;
	memcpy(NDCBufferOut + offset, "0", 1);                          
	offset+=1;

	/* - Donnees JOURNAL ------------------------------------------- */
	memset(prn.data,                0, sizeof(prn.data));
	

	retour = GetPrint ( nIndexCtx+RESERVED_THREAD,
		tab_NdcCtx[nIndexCtx].terminal,
		tab_NdcCtx[nIndexCtx].AcquirerBank,
		NP_CASH_FORGET_JOURNAL_ENG,
		ENGLISH,
		&P7Buffer,
		&prn);

	if(retour == OK)
	{
		/* - Printer Flag Field -------------------------------- */
		memcpy(NDCBufferOut + offset, "1", 1);                      
		offset+=1;
		memcpy(NDCBufferOut + offset, prn.data, prn.len);           
		offset+=prn.len;

	}
	else
	{
		memcpy(NDCBufferOut + offset, "0", 1);                      
		offset+=1;
	}

	/* - envoi du message au GAB -------------------------------------------- */

#ifdef NDC_MAC
	Generate_The_Mac(nIndexCtx +RESERVED_THREAD, tab_NdcCtx[nIndexCtx].terminal , NDCBufferOut , offset ,mac_data);
	NDCBufferOut[offset] = FS;
	offset+= 1;
	memcpy(NDCBufferOut + offset, mac_data, 8);
	offset += 8;
#endif

	
	nFdConnect = GetFdByOraCtx(nIndexCtx+RESERVED_THREAD);
	retour = WriteLineMsg( nFdConnect,NDCBufferOut, offset );

	memset(NDCBufferOut,    0, sizeof(NDCBufferOut) );
	trace_event("End   CashNotTaken()", PROCESSING);
	return (OK);
}

/* =-= -=- =-= -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- */
/* =-= -=- =-= -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- */
int EjectCardOrder(
					int			nIndexCtx,
					char        Coord_Nbr       [ ],
					TSIsoInfo  	*P7Buffer,
					char		pTransLang)
{
	char			sLine[MAX_LINE_TRC];
	ndc_param		prn;
	char            NDCBufferOut            [LG_MAX];
	char            lReceiptCode            [     4];
	char            mac_data[9];
	int				nFdConnect;
	int             offset;
	int             retour;
	int				lPrintFlag;


	trace_event("Start EjectCardOrder()", PROCESSING);

	memset(NDCBufferOut,    0, sizeof(NDCBufferOut) );
	memset(lReceiptCode,    0, sizeof(lReceiptCode) );
	memset(mac_data,    0, sizeof(mac_data) );
	offset = 0;



#ifdef NDC_MAC				
	Prepare_Header_Reply_MAC(TRANSACTION_REPLY, tab_NdcCtx[nIndexCtx].Luno,tab_NdcCtx[nIndexCtx].Tvn, NDCBufferOut + offset);
	offset += 22;		
#else
	Prepare_Header_Reply(TRANSACTION_REPLY, tab_NdcCtx[nIndexCtx].terminal, NDCBufferOut + offset);
	offset += 8;
#endif


	memcpy(NDCBufferOut + offset, "422", 3);                        
	offset+=3;
	NDCBufferOut[offset] = FS;                                      
	offset+=1;
	NDCBufferOut[offset] = FS;                                      
	offset+=1;
	memcpy(NDCBufferOut + offset, "12345", 5);                      
	offset+=5;
	NDCBufferOut[offset] = FS;                                      
	offset+=1;
	NDCBufferOut[offset] = Coord_Nbr[1];                            
	offset+=1;
	memcpy(NDCBufferOut + offset, "00", 2);                         
	offset+=2;
	trace_event("Construction de Message ", PROCESSING);

	/* - Donnees Tickets --------------------------------------------------- */
	lPrintFlag = NOK;

	/* - Receipt Language & cancelation reason ----------------------------- */

	switch (pTransLang)
	{
	case ARABIC     :
		/*TODO: la correction apporte par MCK devrait etre applicable pour ARABIC aussi...*/
		memcpy(lReceiptCode,        NP_TRANS_CANCELED_AR, 3);
		PutIsoField(ISO_RESERVED_ISO_2 , P7Buffer,"013A#],U\" ^M TU=", 17);
		break;
	case FRENSH     :
		memcpy(lReceiptCode,        NP_TRANS_CANCELED_FR, 3);
		/*PutIsoField(ISO_RESERVED_ISO_2 , P7Buffer,"019ERREUR DISTRIBUTEUR", 22);MCK27022014*/
		PutIsoField(ISO_RESERVED_ISO_2 , P7Buffer,"ERREUR DISTRIBUTEUR", 19);
		break;
	default         :
		memcpy(lReceiptCode,        NP_TRANS_CANCELED_ENG, 3);
		/*PutIsoField(ISO_RESERVED_ISO_2 , P7Buffer, "020CASH DISPENSER ERROR", 23);MCK27022014*/
		PutIsoField(ISO_RESERVED_ISO_2 , P7Buffer, "CASH DISPENSER ERROR", 20);
		break;
	}


	memset(prn.data, 0, sizeof(prn.data));

	retour = GetPrint(	nIndexCtx+RESERVED_THREAD,
						tab_NdcCtx[nIndexCtx].terminal, 
						tab_NdcCtx[nIndexCtx].AcquirerBank,
						lReceiptCode, 
						pTransLang, 
						P7Buffer, 
						&prn);

	if(retour == OK)
	{
		/* - Printer Flag Field -------------------------------- */
		NDCBufferOut[offset - 1] = '2';
		memcpy(NDCBufferOut + offset, prn.data, prn.len);       
		offset+=prn.len;

		/* - GROUP SEPARATOR ----------------------------------- */
		NDCBufferOut[offset] = GS;                              
		offset += 1;

		lPrintFlag = OK;
	}
	else
	{
		sprintf(sLine, "- Err-In GetPrint(%.3s) -----------------", lReceiptCode);
		trace_event(sLine, ERROR);
	}

	/* - Donnees JOURNAL ------------------------------------------- */
	memset(prn.data,                0, sizeof(prn.data));
	retour = GetPrint(	nIndexCtx+RESERVED_THREAD,
						tab_NdcCtx[nIndexCtx].terminal,
						tab_NdcCtx[nIndexCtx].AcquirerBank,
						NP_PICKUP_TIME_OUT_JOURNAL_ENG,
						ENGLISH,
						P7Buffer,
						&prn);

	if(retour == OK)
	{
		/* - Printer Flag Field -------------------------------- */
		memcpy(NDCBufferOut + offset, "1", 1);                      
		offset+=1;
		memcpy(NDCBufferOut + offset, prn.data, prn.len);           
		offset+=prn.len;

		lPrintFlag = OK;
	}

	if ( lPrintFlag == NOK )
	{
		memcpy(NDCBufferOut + offset, "0", 1);                      
		offset+=1;
	}


	/* - envoi du message au GAB -------------------------------------------- */
#ifdef NDC_MAC
	Generate_The_Mac(nIndexCtx +RESERVED_THREAD, tab_NdcCtx[nIndexCtx].terminal , NDCBufferOut , offset ,mac_data);
	NDCBufferOut[offset] = FS;
	offset+= 1;
	memcpy(NDCBufferOut + offset, mac_data, 8);
	offset += 8;
#endif
	

	nFdConnect = GetFdByOraCtx(nIndexCtx+RESERVED_THREAD);

	retour = WriteLineMsg( nFdConnect,NDCBufferOut, offset );

	memset(NDCBufferOut,    0, sizeof(NDCBufferOut) );
	trace_event("End   EjectCardOrder(OK)", PROCESSING);
	return (OK);
}



/* =-= -=- =-= -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- */
/*Not used
int TicketNotAvailable(
						int				nIndexCtx,
						TSIsoInfo    	*P7Buffer,
						char        	Coord_Nbr       [ ])
{

	ndc_param       	prn;

	char            	NDCBufferOut            [LG_MAX];
	char      			sLine[MAX_LINE_TRC];
	char           		mac_data[9];
	int					nFdConnect;
	int             	offset;
	int             	retour;

	trace_event("Start TicketNotAvailable()", PROCESSING);

	memset(NDCBufferOut,    0, sizeof(NDCBufferOut) );
	memset(mac_data,     0, sizeof(mac_data)  );	
	memset(sLine,           0, sizeof(sLine)  );	
	offset = 0;

#ifdef NDC_MAC				
	Prepare_Header_Reply_MAC(TRANSACTION_REPLY, tab_NdcCtx[nIndexCtx].Luno,tab_NdcCtx[nIndexCtx].Tvn, NDCBufferOut + offset);
	offset += 22;		
#else
	Prepare_Header_Reply(TRANSACTION_REPLY, tab_NdcCtx[nIndexCtx].terminal, NDCBufferOut + offset);
	offset += 8;
#endif


	memcpy(NDCBufferOut + offset, "035", 3); / * --- il faut mettre un etat qui contient un message ---* /
	offset+=3;
	NDCBufferOut[offset] = FS;                                      
	offset+=1;
	NDCBufferOut[offset] = FS;                                      
	offset+=1;
	memcpy(NDCBufferOut + offset, "12345", 5);                      
	offset+=5;
	NDCBufferOut[offset] = FS;                                      
	offset+=1;
	NDCBufferOut[offset] = Coord_Nbr[1];                            
	offset+=1;
	memcpy(NDCBufferOut + offset, "0", 1);                          
	offset+=1;

	
	memset(prn.data,                0, sizeof(prn.data));

	retour = GetPrint(nIndexCtx+RESERVED_THREAD,tab_NdcCtx[nIndexCtx].terminal,    
		tab_NdcCtx[nIndexCtx].AcquirerBank,
		NP_PICKUP_TIME_OUT_JOURNAL_ENG,
		ENGLISH,P7Buffer,&prn);
	if(retour == OK)
	{
		
		memcpy(NDCBufferOut + offset, "1", 1);                      
		offset+=1;
		memcpy(NDCBufferOut + offset, prn.data, prn.len);           
		offset+=prn.len;

	}
	else
	{
		memcpy(NDCBufferOut + offset, "0", 1);                      
		offset+=1;
	}


	
#ifdef NDC_MAC
	Generate_The_Mac(nIndexCtx +RESERVED_THREAD, tab_NdcCtx[nIndexCtx].terminal , NDCBufferOut , offset ,mac_data);
	NDCBufferOut[offset] = FS;
	offset+= 1;
	memcpy(NDCBufferOut + offset, mac_data, 8);
	offset += 8;
#endif

	
	nFdConnect = GetFdByOraCtx(nIndexCtx+RESERVED_THREAD);
	retour = WriteLineMsg( nFdConnect,NDCBufferOut, offset );

	memset(NDCBufferOut,    0, sizeof(NDCBufferOut) );
	trace_event("End   TicketNotAvailable()", PROCESSING);
	return (OK);
}
*/
/* =-= -=- =-= -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- */
/*-Start LHI050906 **/
int Load_Reason_Code ( 
	SStatusInfo   *status,
	int response_code,
	char *reason_code)
{

	char      sLine[MAX_LINE_TRC];

	sprintf(sLine, "Start Load_Reason_Code(%.1s,%.5s,%d)", status->fStatusDesc, status->fStatusInfo, response_code);
	trace_event(sLine,PROCESSING);


	if( status->fStatusDesc[0] == SD_CMD_REJ)
	{
		memcpy(reason_code, "4020", 4);
	}
	else if ( response_code == 200 || response_code == 201 ||
			response_code == 202 || response_code == 207 ||
			response_code == 208 || response_code == 209 ||
			response_code == 210 || response_code == 206 ||
			response_code == 101)
	{
		switch(response_code)
		{
		case 106        : 
			memcpy(reason_code, "3706", 4);
			break;
		case 101        : 
			memcpy(reason_code, "3701", 4);
			break;
		case 206        : 
			memcpy(reason_code, "3706", 4);
			break;
		case 201        : 
			memcpy(reason_code, "3701", 4);
			break;
		case 202        : 
			memcpy(reason_code, "3708", 4);
			break;
		case 210        : 
			memcpy(reason_code, "3708", 4);
			break;
		case 200        : 
			memcpy(reason_code, "3700", 4);
			break;
		case 207        : 
			memcpy(reason_code, "3700", 4);
			break;
		case 208        : 
			memcpy(reason_code, "3700", 4);
			break;
		case 209        : 
			memcpy(reason_code, "3700", 4);
			break;
		default         : 
			memcpy(reason_code, "3700", 4);
			break;
		}
	}
	else if( status->fStatusInfo[0] == DI_CASH_HANDLER && status->fStatusInfo[1] == DV_SI_E_TS_NOTES_RETRACTED )
	{
		memcpy(reason_code, "4018", 4);

	}
	else if( status->fStatusInfo[0] == DI_CASH_HANDLER && status->fStatusInfo[1] == DV_SI_E_TS_NO_DISPENSE )
	{
		memcpy(reason_code, "4017", 4);
	}
	else if( status->fStatusDesc[0] == SD_DEVICE_FAULT)
	{
		trace_event("StatusInfo->fStatusDesc= Device fault",FATAL);
		if( status->fStatusInfo[0] == DI_CASH_HANDLER && status->fStatusInfo[1] == DV_SI_E_TS_NO_DISPENSE_CARD_NOT_EJ )
		{
			memcpy(reason_code, "4017", 4);
		}
		else if( status->fStatusInfo[0] == DI_CARD )
		{
			switch(status->fStatusInfo[1] )
			{
			case DV_SI_D_TS_NO_EXCP:			memcpy(reason_code, "4007", 4); break;
			case DV_SI_D_TS_CRD_NOT_TAKEN:		memcpy(reason_code, "4019", 4); break;
			case DV_SI_D_TS_EJECT_FAILURE:		memcpy(reason_code, "4014", 4); break;
			case DV_SI_D_TS_UPD_FAILURE:		memcpy(reason_code, "4016", 4); break;
			case DV_SI_D_TS_INV_TRACK_DATA:		memcpy(reason_code, "4020", 4); break;
			}
		}
#ifdef WNC
		else if( memcmp(status->fStatusInfo, "e1", 2) == 0 )
		{
			trace_event("StatusInfo->fStatusInfo==e1",FATAL);

			if(memcmp(status->fDiagStatus,"0",1)==0)
				memcpy(reason_code, "4600", 4);
			else if (memcmp(status->fDiagStatus,"1",1)==0)
				memcpy(reason_code, "4601", 4);
			else if (memcmp(status->fDiagStatus,"2",1)==0)
				memcpy(reason_code, "4602", 4);
		}
#else 
		else if( memcmp(status->fStatusInfo, "c", 1) == 0 )
		{
			memcpy(reason_code, "4020", 4);
		}
#endif
	}
	else if( status->fStatusDesc[0] == SD_SPEC_CMD_REJ)
	{
		memcpy(reason_code, "4006", 4);
	}

	sprintf(sLine, "End   Load_Reason_Code(%.4s)", reason_code);
	trace_event(sLine,PROCESSING);
	return(OK);
}

