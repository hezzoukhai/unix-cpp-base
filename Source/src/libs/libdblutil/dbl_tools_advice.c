/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      dbl_tools_advice.c                                                     */
/*                                                                             */
/* Description                                                                 */
/*      ATM Advice Tools Management                                            */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001MK     		18-02-2002   		Mahjoub KARROUM    		Initiale Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>

#include <define.h>
#include <ascii.h>
#include <iso_hps.h>
#include <dbl_paramdef.h>
#include <atmopcode.h>
#include <langue.h>
#include <dbl_param.h>
#include <thr_dbl_extern.h> 

/* =-= -=- =-= -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- */
/* =-= -=- =-= -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- */
int	CardNotTaken(nIndexCtx,Coord_Nbr, P7Buffer, pTransLang)
int				nIndexCtx;
char            Coord_Nbr    [   4];
TSIsoInfo       *P7Buffer    ;
char			pTransLang;
{
	char      		sLine[MAX_LINE_TRC];
	dbl_param       prn;
	unsigned char            DBLBufferOut            [LG_MAX];
	char            DBLBufferIn             [LG_MAX];
	char            lReceiptCode            [     4];
	char            tmpField[LG_MAX];
	char            tmpField1[LG_MAX];
	int             lgField;
	int				nFdConnect;
	int				lPrintFlag;
	int             DBLBufferInLen;
	int             offset;
	int             retour;
	trace_event("Start CardNotTaken()", PROCESSING);
	sprintf(sLine,"  Start CardNotTaken(%s,%c)",tab_DblCtx[nIndexCtx].terminal, pTransLang);
	trace_event(sLine, PROCESSING);
 

	memset(DBLBufferOut,    0, sizeof(DBLBufferOut) );
	memset(DBLBufferIn,     0, sizeof(DBLBufferIn)  );
	memset(lReceiptCode,    0, sizeof(lReceiptCode) );
	offset = 0;

	memcpy(DBLBufferOut + offset, "4 ", 2);                         
	offset+=2;
	DBLBufferOut[offset] = FS;                                      
	offset+=1;
	memcpy(DBLBufferOut + offset, "000", 3);                        
	offset+=3;
	DBLBufferOut[offset] = FS;                                      
	offset+=1;
	DBLBufferOut[offset] = FS;                                      
	offset+=1;
	memcpy(DBLBufferOut + offset, "401", 3);                        
	offset+=3;
	DBLBufferOut[offset] = FS;                                      
	offset+=1;
	DBLBufferOut[offset] = FS;                                      
	offset+=1;
	memcpy(DBLBufferOut + offset, "12344", 5);                      
	offset+=5;
	DBLBufferOut[offset] = FS;                                      
	offset+=1;
	DBLBufferOut[offset] = Coord_Nbr[1];                            
	offset+=1;
	memcpy(DBLBufferOut + offset, "1", 1);                          
	offset+=1;

    /*** (63) ISO_NETWORK_DATA   ******/                                        
	/* -- START KR MODIF LE 16-09-2003 : Pour des raisons de suivie dans le journal --
    if ( GetIsoField(ISO_TERMINAL_NBR , P7Buffer, tmpField, &lgField) == SUCCES )
    {
        memset(tmpField1,'\0',sizeof(tmpField1));
        GetTermTransSeq(nIndexCtx+RESERVED_THREAD,tmpField,tmpField1);
        if (InsertIsoField(ISO_NETWORK_DATA , P7Buffer, tmpField1, 6) != SUCCES)
            PutIsoField(ISO_NETWORK_DATA , P7Buffer, tmpField1, 6);
        SetTermTransSeq(nIndexCtx+RESERVED_THREAD,tmpField);
    }
	--*/

	if ( GetIsoField(ISO_AUDIT_NBR , P7Buffer, tmpField, &lgField) == SUCCES ){
        if (InsertIsoField(ISO_NETWORK_DATA , P7Buffer, tmpField, lgField) != SUCCES)
            PutIsoField(ISO_NETWORK_DATA , P7Buffer, tmpField, lgField);
	}
	/*-- END KR MODIF LE 16-09-2003 : Pour des raisons de suivie dans le journal --*/

	/* - Donnees Tickets --------------------------------------------------- */
	lPrintFlag = NOK;

        sprintf(sLine, "DEBUG opCode=[%d,%d]", tab_DblCtx[nIndexCtx].OpReq, nIndexCtx);
        trace_event(sLine, TRACE);

	/* - Receipt Language & cancelation reason ----------------------------- */
	switch (pTransLang)
	{
	case ARABIC	:
		if (tab_DblCtx[nIndexCtx].OpReq == OR_ONUS_WITHDRAWAL) {
		   memcpy(lReceiptCode,	NP_TRANS_CANCELED_AR, 3);
		} else {
		   memcpy(lReceiptCode,	NP_TRANS_CANCELED_OFF_US_AR, 3);
		}
		PutIsoField(ISO_RESERVED_ISO_2 , P7Buffer,"\\O#G&U\" Y#_CZ", 13);
		break;
	case FRENSH	:
		memcpy(lReceiptCode,	NP_TRANS_CANCELED_FR, 3);
		PutIsoField(ISO_RESERVED_ISO_2 , P7Buffer,"CARTE OUBLIEE", 13);
		break;
	default		:
		if (tab_DblCtx[nIndexCtx].OpReq == OR_ONUS_WITHDRAWAL) {
		   memcpy(lReceiptCode,	NP_TRANS_CANCELED_ENG, 3);
		} else {
		   memcpy(lReceiptCode,	NP_TRANS_CANCELED_OFF_US_ENG, 3);
		}
		PutIsoField(ISO_RESERVED_ISO_2 , P7Buffer,"CARD NOT TAKEN", 14);
	}

	memset(prn.data, 0, sizeof(prn.data));
	retour = GetPrint(nIndexCtx+RESERVED_THREAD,tab_DblCtx[nIndexCtx].terminal, lReceiptCode, pTransLang, P7Buffer, &prn);
	if(retour == OK)
	{
		/* - Printer Flag Field -------------------------------- */
		memcpy(DBLBufferOut + offset, "2", 1);                  
		offset += 1;
		memcpy(DBLBufferOut + offset, prn.data, prn.len);       
		offset+=prn.len;

		/* - GROUP SEPARATOR ----------------------------------- */
		DBLBufferOut[offset] = FS;                              
		offset += 1;

		lPrintFlag = OK;
	}
	else
	{
		sprintf(sLine, "- Err-In GetPrint(%.3s) -----------------",
		    lReceiptCode);
		trace_event(sLine, ERROR);
	}

	/* - Donnees JOURNAL ------------------------------------------- */
	memset(prn.data,                0, sizeof(prn.data));
	retour = GetPrint(	nIndexCtx+RESERVED_THREAD,
						tab_DblCtx[nIndexCtx].terminal,
	    				NP_PICKUP_TIME_OUT_JOURNAL_ENG,
	    				ENGLISH,
	    				P7Buffer,
	    				&prn);
	if(retour == OK)
	{
		/* - Printer Flag Field -------------------------------- */
		memcpy(DBLBufferOut + offset, "1", 1);                      
		offset+=1;
		memcpy(DBLBufferOut + offset, prn.data, prn.len);           
		offset+=prn.len;

		lPrintFlag = OK;
	}

	if ( lPrintFlag == NOK )
		memcpy(DBLBufferOut + offset, "0", 1);                      
	offset+=1;

    dump_buffer(DBLBufferOut,offset,'A','L','O');
    nFdConnect = GetFdByOraCtx(nIndexCtx+RESERVED_THREAD);
/* - envoi du message au GAB -------------------------------------------- */
    retour = WriteLineMsg( nFdConnect,DBLBufferOut, offset );

	trace_event("End   CardNotTaken()", PROCESSING);
	return (OK);
}




/* =-= -=- =-= -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- */
/* =-= -=- =-= -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- */
CashNotTaken(nIndexCtx,Coord_Nbr)
int				nIndexCtx;
char            Coord_Nbr       [   4];
{
    char      		sLine[MAX_LINE_TRC];
	dbl_param       prn;
	unsigned char            DBLBufferOut            [LG_MAX];
	TSIsoInfo       P7Buffer                ;
	char            DBLBufferIn             [LG_MAX];
	int             DBLBufferInLen;
	int		nFdConnect;
	int             offset;
	int             retour;

	trace_event("Start CashNotTaken()", PROCESSING);

	memset(DBLBufferOut,    0, sizeof(DBLBufferOut) );
	memset(DBLBufferIn,     0, sizeof(DBLBufferIn)  );
	offset = 0;

	memcpy(DBLBufferOut + offset, "4 ", 2);                         
	offset+=2;
	DBLBufferOut[offset] = FS;                                      
	offset+=1;
	memcpy(DBLBufferOut + offset, "000", 3);                        
	offset+=3;
	DBLBufferOut[offset] = FS;                                      
	offset+=1;
	DBLBufferOut[offset] = FS;                                      
	offset+=1;
	memcpy(DBLBufferOut + offset, "411", 3);                        
	offset+=3;
	DBLBufferOut[offset] = FS;                                      
	offset+=1;
	DBLBufferOut[offset] = FS;                                      
	offset+=1;
	/*memcpy(DBLBufferOut + offset, "12345", 5);        test bcp              */
	memcpy(DBLBufferOut + offset, "12344", 5);                      
	offset+=5;
	DBLBufferOut[offset] = FS;                                      
	offset+=1;
	DBLBufferOut[offset] = Coord_Nbr[1];                            
	offset+=1;
	memcpy(DBLBufferOut + offset, "0", 1);                          
	offset+=1;

	/* - Donnees JOURNAL ------------------------------------------- */
	memset(prn.data,                0, sizeof(prn.data)); prn.len = 0;

	retour = GetPrint(nIndexCtx+RESERVED_THREAD, tab_DblCtx[nIndexCtx].terminal,
					NP_CASH_FORGET_JOURNAL_ENG,ENGLISH,&(tab_DblCtx[nIndexCtx].gContext.fP7Buff),&prn);
	if(retour == OK)
	{
		/* - Printer Flag Field -------------------------------- */
		memcpy(DBLBufferOut + offset, "1", 1);                      
		offset+=1;
		memcpy(DBLBufferOut + offset, prn.data, prn.len);           
		offset+=prn.len;

	}


/* - envoi du message au GAB -------------------------------------------- */
    	dump_buffer(DBLBufferOut,offset,'A','L','O');
    	nFdConnect = GetFdByOraCtx(nIndexCtx+RESERVED_THREAD);
    	retour = WriteLineMsg( nFdConnect,DBLBufferOut, offset );
 	trace_event("End   CashNotTaken()", PROCESSING);
	return (OK);
}

/* =-= -=- =-= -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- */
/* =-= -=- =-= -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- */
EjectCardOrder(nIndexCtx,Coord_Nbr, P7Buffer, pTransLang)
int			nIndexCtx;
char        Coord_Nbr       [   4];
TSIsoInfo  	*P7Buffer;
char		pTransLang;
{
    char      sLine[MAX_LINE_TRC];
	dbl_param	prn;
	unsigned char            DBLBufferOut            [LG_MAX];
	char            DBLBufferIn             [LG_MAX];
	char            lReceiptCode            [     4];
	int             DBLBufferInLen;
	int		nFdConnect;
	int             offset;
	int             retour;
	int		lPrintFlag;
	char      	trace_audit_nbr    [     7];
	int       	lgField;

	trace_event("Start EjectCardOrder()", PROCESSING);

	memset(DBLBufferOut,    0, sizeof(DBLBufferOut) );
	memset(DBLBufferIn,     0, sizeof(DBLBufferIn)  );
	memset(lReceiptCode,    0, sizeof(lReceiptCode) );
	memset(trace_audit_nbr,      0, sizeof(trace_audit_nbr)   );
	offset = 0;

	memcpy(DBLBufferOut + offset, "4 ", 2);                         
	offset+=2;
	DBLBufferOut[offset] = FS;                                      
	offset+=1;
	memcpy(DBLBufferOut + offset, "000", 3);                        
	offset+=3;
	DBLBufferOut[offset] = FS;                                      
	offset+=1;
	DBLBufferOut[offset] = FS;                                      
	offset+=1;
	memcpy(DBLBufferOut + offset, "401", 3);           
	offset+=3;
	DBLBufferOut[offset] = FS;                                      
	offset+=1;
	DBLBufferOut[offset] = FS;                                      
	offset+=1;
	/*BEGIN AHM 03/09/2005*/
	/*memcpy(DBLBufferOut + offset, "1234E", 5);                      
	offset+=5;
	*/
	if (GetIsoField(ISO_AUDIT_NBR, P7Buffer, trace_audit_nbr, &lgField) != SUCCES )
        	strcpy(trace_audit_nbr,"000000");
       /* - Transaction serial Number - */
        memcpy(DBLBufferOut + offset, trace_audit_nbr + 2, 4);
        offset+=4;

	DBLBufferOut[offset] = 'D';                                      
	offset+=1;
	/*END AHM*/
	DBLBufferOut[offset] = FS;                                      
	offset+=1;
	DBLBufferOut[offset] = Coord_Nbr[1];                            
	offset+=1;
	memcpy(DBLBufferOut + offset, "0", 1);                         
	offset+=1;
	trace_event("Construction de Message ", PROCESSING);

	/* - Donnees Tickets --------------------------------------------------- */
	lPrintFlag = NOK;

	/* - Receipt Language & cancelation reason ----------------------------- */
	switch (pTransLang)
	{
	case ARABIC     :
		if (tab_DblCtx[nIndexCtx].OpReq == OR_ONUS_WITHDRAWAL) {
                   memcpy(lReceiptCode, NP_TRANS_CANCELED_AR, 3);
                } else {
                   memcpy(lReceiptCode, NP_TRANS_CANCELED_OFF_US_AR, 3);
                }
		PutIsoField(ISO_RESERVED_ISO_2 , P7Buffer,"A#],U\" ^M TU=", 13);
		break;
	case FRENSH     :
		memcpy(lReceiptCode,        NP_TRANS_CANCELED_FR, 3);
		PutIsoField(ISO_RESERVED_ISO_2 , P7Buffer,"ERREUR DISTRIBUTEUR", 19);
		break;
	default         :
                if (tab_DblCtx[nIndexCtx].OpReq == OR_ONUS_WITHDRAWAL) {
                   memcpy(lReceiptCode, NP_TRANS_CANCELED_ENG, 3);
                } else {
                   memcpy(lReceiptCode, NP_TRANS_CANCELED_OFF_US_ENG, 3);
                }
		PutIsoField(ISO_RESERVED_ISO_2 , P7Buffer, "CASH DISPENSER ERROR", 20);
	}

	memset(prn.data, 0, sizeof(prn.data));
	retour = GetPrint(nIndexCtx+RESERVED_THREAD,tab_DblCtx[nIndexCtx].terminal, lReceiptCode, pTransLang, P7Buffer, &prn);
	if(retour == OK)
	{
		/* - Printer Flag Field -------------------------------- */
		memcpy(DBLBufferOut + offset, "2", 1);                  
		offset += 1;
		memcpy(DBLBufferOut + offset, prn.data, prn.len);       
		offset+=prn.len;

		/* - GROUP SEPARATOR ----------------------------------- */
		DBLBufferOut[offset] = FS;                              
		offset += 1;

		lPrintFlag = OK;
	}
	else
	{
		sprintf(sLine, "- Err-In GetPrint(%.3s) -----------------",
		    lReceiptCode);
		trace_event(sLine, ERROR);
	}

	/* - Donnees JOURNAL ------------------------------------------- */
	memset(prn.data,                0, sizeof(prn.data));
	retour = GetPrint(nIndexCtx+RESERVED_THREAD,tab_DblCtx[nIndexCtx].terminal,
	    NP_PICKUP_TIME_OUT_JOURNAL_ENG,
	    ENGLISH,
	    P7Buffer,
	    &prn);
	if(retour == OK)
	{
		/* - Printer Flag Field -------------------------------- */
		memcpy(DBLBufferOut + offset, "1", 1);                      
		offset+=1;
		memcpy(DBLBufferOut + offset, prn.data, prn.len);           
		offset+=prn.len;

		lPrintFlag = OK;
	}

	if ( lPrintFlag == NOK )
		memcpy(DBLBufferOut + offset, "0", 1);                      
	offset+=1;


	/* - envoi du message au GAB -------------------------------------------- */
    	dump_buffer(DBLBufferOut,offset,'A','L','O');
    	nFdConnect = GetFdByOraCtx(nIndexCtx+RESERVED_THREAD);
    	retour = WriteLineMsg( nFdConnect,DBLBufferOut, offset );

	trace_event("End   EjectCardOrder(OK)", PROCESSING);
	return (OK);
}





EjectCardOrder_NotUsed(nIndexCtx,Coord_Nbr, P7Buffer, pTransLang)
int			nIndexCtx;
char        Coord_Nbr       [   4];
TSIsoInfo  	*P7Buffer;
char		pTransLang;
{
    char      sLine[MAX_LINE_TRC];
	dbl_param	prn;
	unsigned char            DBLBufferOut            [LG_MAX];
	char            DBLBufferIn             [LG_MAX];
	char            lReceiptCode            [     4];
	int             DBLBufferInLen;
	int		nFdConnect;
	int             offset;
	int             retour;
	int		lPrintFlag;
	char      	trace_audit_nbr    [     7];
	int       	lgField;

	trace_event("Start EjectCardOrder()", PROCESSING);

	memset(DBLBufferOut,    0, sizeof(DBLBufferOut) );
	memset(DBLBufferIn,     0, sizeof(DBLBufferIn)  );
	memset(lReceiptCode,    0, sizeof(lReceiptCode) );
	memset(trace_audit_nbr,      0, sizeof(trace_audit_nbr)   );
	offset = 0;

	memcpy(DBLBufferOut + offset, "4 ", 2);                         
	offset+=2;
	DBLBufferOut[offset] = FS;                                      
	offset+=1;
	memcpy(DBLBufferOut + offset, "000", 3);                        
	offset+=3;
	DBLBufferOut[offset] = FS;                                      
	offset+=1;
	DBLBufferOut[offset] = FS;                                      
	offset+=1;
	memcpy(DBLBufferOut + offset, "401", 3);           
	offset+=3;
	DBLBufferOut[offset] = FS;                                      
	offset+=1;
	DBLBufferOut[offset] = FS;                                      
	offset+=1;
	/*BEGIN AHM 03/09/2005*/
	/*memcpy(DBLBufferOut + offset, "1234E", 5);                      
	offset+=5;
	*/
	if (GetIsoField(ISO_AUDIT_NBR, P7Buffer, trace_audit_nbr, &lgField) != SUCCES )
        	strcpy(trace_audit_nbr,"000000");
       /* - Transaction serial Number - */
        memcpy(DBLBufferOut + offset, trace_audit_nbr + 2, 4);
        offset+=4;

	DBLBufferOut[offset] = 'E';                                      
	offset+=1;
	/*END AHM*/
	DBLBufferOut[offset] = FS;                                      
	offset+=1;
	DBLBufferOut[offset] = Coord_Nbr[1];                            
	offset+=1;
	memcpy(DBLBufferOut + offset, "00", 2);                         
	offset+=2;
	trace_event("Construction de Message ", PROCESSING);

	/* - Donnees Tickets --------------------------------------------------- */
	lPrintFlag = NOK;

	/* - Receipt Language & cancelation reason ----------------------------- */
	switch (pTransLang)
	{
	case ARABIC     :
                if (tab_DblCtx[nIndexCtx].OpReq == OR_ONUS_WITHDRAWAL) {
                   memcpy(lReceiptCode, NP_TRANS_CANCELED_AR, 3);
                } else {
                   memcpy(lReceiptCode, NP_TRANS_CANCELED_OFF_US_AR, 3);
                }
		/*PutIsoField(ISO_RESERVED_ISO_2 , P7Buffer,"013A#],U\" ^M TU=", 16);*/
		PutIsoField(ISO_RESERVED_ISO_2 , P7Buffer,"019ERREUR DISTRIBUTEUR", 22);
		break;
	case FRENSH     :
		memcpy(lReceiptCode,        NP_TRANS_CANCELED_FR, 3);
		PutIsoField(ISO_RESERVED_ISO_2 , P7Buffer,"019ERREUR DISTRIBUTEUR", 22);
		break;
	default         :
                if (tab_DblCtx[nIndexCtx].OpReq == OR_ONUS_WITHDRAWAL) {
                   memcpy(lReceiptCode, NP_TRANS_CANCELED_ENG, 3);
                } else {
                   memcpy(lReceiptCode, NP_TRANS_CANCELED_OFF_US_ENG, 3);
                }
		PutIsoField(ISO_RESERVED_ISO_2 , P7Buffer, "020CASH DISPENSER ERROR", 23);
	}

	memset(prn.data, 0, sizeof(prn.data));
	retour = GetPrint(nIndexCtx+RESERVED_THREAD,tab_DblCtx[nIndexCtx].terminal, lReceiptCode, pTransLang, P7Buffer, &prn);
	if(retour == OK)
	{
		/* - Printer Flag Field -------------------------------- */
		memcpy(DBLBufferOut + offset, "2", 1);                  
		offset += 1;
		memcpy(DBLBufferOut + offset, prn.data, prn.len);       
		offset+=prn.len;

		/* - GROUP SEPARATOR ----------------------------------- */
		DBLBufferOut[offset] = GS;                              
		offset += 1;

		lPrintFlag = OK;
	}
	else
	{
		sprintf(sLine, "- Err-In GetPrint(%.3s) -----------------",
		    lReceiptCode);
		trace_event(sLine, ERROR);
	}

	/* - Donnees JOURNAL ------------------------------------------- */
	memset(prn.data,                0, sizeof(prn.data));
	retour = GetPrint(nIndexCtx+RESERVED_THREAD,tab_DblCtx[nIndexCtx].terminal,
	    NP_PICKUP_TIME_OUT_JOURNAL_ENG,
	    ENGLISH,
	    P7Buffer,
	    &prn);
	if(retour == OK)
	{
		/* - Printer Flag Field -------------------------------- */
		memcpy(DBLBufferOut + offset, "1", 1);                      
		offset+=1;
		memcpy(DBLBufferOut + offset, prn.data, prn.len);           
		offset+=prn.len;

		lPrintFlag = OK;
	}

	if ( lPrintFlag == NOK )
		memcpy(DBLBufferOut + offset, "0", 1);                      
	offset+=1;


	/* - envoi du message au GAB -------------------------------------------- */
    	dump_buffer(DBLBufferOut,offset,'A','L','O');
    	nFdConnect = GetFdByOraCtx(nIndexCtx+RESERVED_THREAD);
    	retour = WriteLineMsg( nFdConnect,DBLBufferOut, offset );

	trace_event("End   EjectCardOrder(OK)", PROCESSING);
	return (OK);
}



/* =-= -=- =-= -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- */
int TicketNotAvailable(nIndexCtx,Coord_Nbr)
int				nIndexCtx;
char        	Coord_Nbr       [   4];
{
    	
	dbl_param       	prn;
	TSIsoInfo       	P7Buffer                ;
	unsigned char            	DBLBufferOut            [LG_MAX];
	char            	DBLBufferIn             [LG_MAX];
	char      			sLine[MAX_LINE_TRC];
	int             	DBLBufferInLen;
	int					nFdConnect;
	int             	offset;
	int             	retour;

	trace_event("Start TicketNotAvailable()", PROCESSING);

	memset(DBLBufferOut,    0, sizeof(DBLBufferOut) );
	memset(DBLBufferIn,     0, sizeof(DBLBufferIn)  );
	offset = 0;

	memcpy(DBLBufferOut + offset, "4 ", 2);                         
	offset+=2;
	DBLBufferOut[offset] = FS;                                      
	offset+=1;
	memcpy(DBLBufferOut + offset, "000", 3);                        
	offset+=3;
	DBLBufferOut[offset] = FS;                                      
	offset+=1;
	DBLBufferOut[offset] = FS;                                      
	offset+=1;
	memcpy(DBLBufferOut + offset, "035", 3); /* --- il faut mettre un etat qui contient un message ---*/
	offset+=3;
	DBLBufferOut[offset] = FS;                                      
	offset+=1;
	DBLBufferOut[offset] = FS;                                      
	offset+=1;
	memcpy(DBLBufferOut + offset, "12345", 5);                      
	offset+=5;
	DBLBufferOut[offset] = FS;                                      
	offset+=1;
	DBLBufferOut[offset] = Coord_Nbr[1];                            
	offset+=1;
	memcpy(DBLBufferOut + offset, "0", 1);                          
	offset+=1;

	/* - Donnees JOURNAL ------------------------------------------- */
	memset(prn.data,                0, sizeof(prn.data));

	retour = GetPrint(nIndexCtx+RESERVED_THREAD,tab_DblCtx[nIndexCtx].terminal,    /*  il faut changer le code d'impression */
	    			  NP_PICKUP_TIME_OUT_JOURNAL_ENG,
	    			  ENGLISH,P7Buffer,&prn);
	if(retour == OK)
	{
		/* - Printer Flag Field -------------------------------- */
		memcpy(DBLBufferOut + offset, "1", 1);                      
		offset+=1;
		memcpy(DBLBufferOut + offset, prn.data, prn.len);           
		offset+=prn.len;

	}


/* - envoi du message au GAB -------------------------------------------- */
    	dump_buffer(DBLBufferOut,offset,'A','L','O');
    	nFdConnect = GetFdByOraCtx(nIndexCtx+RESERVED_THREAD);
    	retour = WriteLineMsg( nFdConnect,DBLBufferOut, offset );
 	trace_event("End   TicketNotAvailable()", PROCESSING);
	return (OK);
}
/* =-= -=- =-= -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- -=- =-= -=- */
void    Load_Reason_Code(	Status_Descriptor,
							Device_Status,
							response_code,
							reason_code)
char    *Status_Descriptor;
char    *Device_Status;
int     response_code;
char    *reason_code;
{
    char      sLine[MAX_LINE_TRC];
	sprintf(sLine, "Start Load_Reason_Code(%.1s,%.5s,%d)",
	    Status_Descriptor, Device_Status, response_code);
	trace_event(sLine,PROCESSING);


	if( memcmp(Status_Descriptor, "A", 1) == 0 )
	{
		memcpy(reason_code, "4020", 4);
	}
	else
		if ( response_code == 200 || response_code == 201 ||
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
		else
			if( memcmp(Device_Status, "E5", 2) == 0 )
			{
				memcpy(reason_code, "4018", 4);

			}
			else
				if( memcmp(Device_Status, "E2", 2) == 0 )
				{
					memcpy(reason_code, "4017", 4);

				}
				else
					if( memcmp(Status_Descriptor, "8", 1) == 0 )
					{
						if( (memcmp(Device_Status, "E4", 2) == 0) || 
						    (memcmp(Device_Status, "E3", 2) == 0))
						{
							memcpy(reason_code, "4017", 4);
						}
						else if( memcmp(Device_Status, "D0", 2) == 0 )
						{
							memcpy(reason_code, "4007", 4);
						}
						else if( memcmp(Device_Status, "D1", 2) == 0 )
						{
							memcpy(reason_code, "4019", 4);
						}
						else if( memcmp(Device_Status, "D2", 2) == 0 )
						{
							memcpy(reason_code, "4014", 4);
						}
						else if( memcmp(Device_Status, "D3", 2) == 0 )
						{
							memcpy(reason_code, "4016", 4);
						}
						else if( memcmp(Device_Status, "D4", 2) == 0 )
						{
							memcpy(reason_code, "4020", 4);
						}
					}
					else
						if( memcmp(Status_Descriptor, "C", 1) == 0 )
						{
							memcpy(reason_code, "4006", 4);
						}

	sprintf(sLine, "End   Load_Reason_Code(%.4s)", reason_code);
	trace_event("End   Load_Reason_Code()",PROCESSING);
	return;
}


