#ifndef P7_IAS_SERVICES___H
#define P7_IAS_SERVICES___H

#define		IAS_SERVICE_LEN				4

#define		GET_SA_UNPAID_AMOUNT		"1000"
#define		GET_SA_UNPAID_AMOUNT_N		1000



int IASGetSAUnpaidAmnt (int		nIndexCtx, char*	sAccountNumber,char*	pAmount,char*	pCurrency);


#endif

