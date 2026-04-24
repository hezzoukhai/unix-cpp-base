/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      pin_chg_key_mng.c                                                      */
/*                                                                             */
/* Description                                                                 */
/*      Pin Change Tools Management                                        */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
0001MK     		18-02-2002   		Mahjoub KARROUM    		* Initiale Version	
0001MAW     		26-05-2002   		M.A.WAHBI    		      * Generate the Offset with the PinKey, i.e PVK set 
linked to PVKi of Card Range
0002MAW                26/06/2003              M.A.WAHBI                     * Alining the libndcutil and libprocessing versions
of the pin_chd_key_mng.c file
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#ifdef SOLARIS
#include <limits.h>
#endif
#include <define.h>
#include <paramdef.h>
#include <resources.h>
#include <hsm_inc.h>
#include <tlv_autho.h>
#include <tlv_prod.h>
#include <security_data.h>
#include <tlv_privfields.h>
#include <tlv_private.h>

#include <p7_define.h>

/***********************************************************************
**  nom       : ComputePinOffset                                      **
**  desc      : calcul du pin offset a partir du pin block            **
**  entree    : pPinBlk   -> PIN Block sous Tpk           **
**  sortie    : PIN offset calcule                                    **
***********************************************************************/
int ComputePinOffset(
	int			nIndexCtx,
TSTlvAutho  *sTlvInfo,
char        *pOffset,
char        pvki,
int         pPinLength)
{
	TSTlvPrivate    sTlvPrivateInfo;
	char            sTlvPrivateBuffer [ BUF_LG_MAX + 1 ];
	char 	        sLine		  	[MAX_LINE_TRC];
	char            sSecurityData 	[ 100 + 1 ];
	/*char   			sSecurityFormat	[2 + 1];*/
	char    		sKey        [SIZE_KEY_TDES];/*NAB18022019*/
	char    		lPinBlkFmt  [ 3];
	char    		lPvka       [SIZE_KEY_TDES];/*NAB18022019*/
	/*char    		lPvkb       [SIZE_KEY_TDES];*/
	/*char    		lPinLmk     [17];*/
	/*char    		lPin        [ 9];*/
	/*char			lPvki		[ 3];*/
	char    		lPinBlk     [17];
	char    		lCardNbr    [23];
	/*char			sTlvBuffer 	[LG_MAX];*/
	int     		lRet = NOK;
	int 			nLength;

	trace_event("Start ComputePinOffset()", PROCESSING);

	InitTlvPrivate (&sTlvPrivateInfo);
	memset (sTlvPrivateBuffer,    '\0',  sizeof(sTlvPrivateBuffer) );
	memset(sKey,                  '\0', sizeof(sKey)     );
	memset(lPinBlkFmt,            '\0', sizeof(lPinBlkFmt)   );
	memset(lPvka,                 '\0', sizeof(lPvka)    );
/*	memset(lPvkb,                 '\0', sizeof(lPvkb)    );*/
/*	memset(lPinLmk,               '\0', sizeof(lPinLmk)  );*/
/*	memset(lPin,                  '\0', sizeof(lPin)     );*/
	memset(lPinBlk,               '\0', sizeof(lPinBlk)  );
	memset(lCardNbr,              '\0', sizeof(lCardNbr) );
	memset(sSecurityData,         '\0', sizeof(sSecurityData ));
	/*memset(sSecurityFormat,       '\0', sizeof(sSecurityFormat ));*/
	/*memset(sTlvBuffer,            '\0', sizeof(sTlvBuffer ));*/
	/* - Preparation des donnees ------------------------------------------- */

	if(GetTlvAutho(AUTO_PAN ,sTlvInfo,lCardNbr,&nLength)!= SUCCES)
	{
		trace_event(" Extract Card Number Error ", PROCESSING);
		trace_event("End   ComputePinOffset(ERROR)", PROCESSING);
		return(ERROR);
	}
	
	if(GetTlvAutho(AUTO_ACQUIRER_REF_DATA,sTlvInfo,lPinBlk,&nLength)!= SUCCES)
	{
		trace_event(" Extract New Pin Block Data Error ", PROCESSING);
		trace_event("End   ComputePinOffset(ERROR)", PROCESSING);
		return(ERROR);
	}
	GetTlvAutho(AUTO_PVK_V1,sTlvInfo,sKey,&nLength);/*ZKO26032019*/
	/*if(sKey[0]=='S')/*NAB18022019 Key Block*/
	if(strlen(sKey) > 33) /*IBO solution temporaire pour HSM Atalla à changer*/
    {
		memcpy(lPvka, sKey, nLength);	
	}
    else
    {		 GetTlvAutho(AUTO_PVK_KEY,sTlvInfo,sKey,&nLength);
		memcpy(lPvka, sKey, SIZE_KEY_DES);
	/*	memcpy(lPvkb, sKey + SIZE_KEY_DES, SIZE_KEY_DES);*//*ZKO26032019 not used*/
    }
	if ( GetTlvAutho( AUTO_ADD_DATA_PRIVATE , sTlvInfo,sTlvPrivateBuffer , &nLength ) != SUCCES ) 
	{
		trace_event(" Extract Priv Data Error ", PROCESSING);
		trace_event("End   ComputePinOffset(ERROR)", PROCESSING);
		return(ERROR);
	}

	AnalyseTlvPrivate(sTlvPrivateBuffer   ,nLength,&sTlvPrivateInfo);
	GetTlvPrivate   ( ENCR_PIN_BLOCK_KEY_TAG,&sTlvPrivateInfo,sKey,&nLength);

	GetTlvAutho     ( AUTO_SECURITY_DATA,sTlvInfo,sSecurityData,&nLength);
	/*GetSecurityData ( SECURITY_FORMAT,sSecurityData,sSecurityFormat,&nLength);*/
	GetSecurityData (PIN_BLOCK_FORMAT,sSecurityData,lPinBlkFmt,&nLength);


	/* - Translation du PinBlk --------------------------------------------- */
	/*if ( memcmp(sSecurityFormat,"01",2) == 0)  
		lRet = PinFromTpkToLmk(nIndexCtx,sKey, lPinBlk, lPinBlkFmt, lCardNbr, lPinLmk );
	else
		lRet = PinFromZpkToLmk(nIndexCtx,sKey, lPinBlk, lPinBlkFmt, lCardNbr, lPinLmk);

	if(lRet != OK) 
	{
		trace_event("End   ComputePinOffset(NOK)", PROCESSING);
		return(NOK);
	}


	AuthoBuildTlv(sTlvBuffer, sTlvInfo);

	lRet =  FormatTheOffsetTlvBuffer (	lCardNbr,
										lPvka,
										lPinLmk,
										pPinLength, 
										sTlvBuffer );
	if(lRet == OK)
	{
		lRet = Production( nIndexCtx , sTlvBuffer );
	}

	if(lRet != OK) 
	{
		trace_event("End   ComputePinOffset(NOK)", PROCESSING);
		return(NOK);
	}   

	if( GetProdTlvBuffer( PROD_OFFSET_TAG ,sTlvBuffer, pOffset ,&nLength) != SUCCES) 
	{
		trace_event("End   ComputePinOffset(NOK)", PROCESSING);
		return(NOK);
	}*/

	lRet = ProdPinOffsetOfcustomerPIN(nIndexCtx, 
								sKey,
								lPvka,
								lPinBlk,
								lPinBlkFmt,
								lCardNbr,
								pOffset,
								pPinLength);

	trace_event("End   ComputePinOffset(OK)", PROCESSING);
	return(lRet);
}

/***********************************************************************
**  nom       : ComputePvv                                      **
**  desc      : calcul du pvv a partir du pin block            **
**  entree    : pPinBlk   -> PIN Block sous Tpk           **
**  sortie    : PIN offset calcule                                    **
***********************************************************************/
int ComputePvv(int	nIndexCtx,TSTlvAutho  *sTlvInfo,char        *pPvv,char        pvki)
{
	TSTlvPrivate    sTlvPrivateInfo;
	char            sTlvPrivateBuffer [ LG_MAX + 1 ];
	char 	        sLine		  	[MAX_LINE_TRC];
	char            sSecurityData 	[ 100 + 1 ];
	/*char   			sSecurityFormat	[2 + 1];*/
	char    		sKey        [256];
	char    		lPinBlkFmt  [ 3];
	char    		lPvka       [SIZE_KEY_TDES];/*NAB18022019*/
	char    		lPvkb       [SIZE_KEY_TDES];
	/*char    		lPinLmk     [17];*/
	/*char    		lPin        [ 9];*/
	char			lPvki		[ 3];
	char    		lPinBlk     [17];
	char    		lCardNbr    [23];
	/*char			sTlvBuffer 	[LG_MAX];*/
	int     		lRet = NOK;
	int 			nLength;

	trace_event("Start ComputePvv()", PROCESSING);

	InitTlvPrivate (&sTlvPrivateInfo);
	memset(sKey,          0, sizeof(sKey)     );
	memset (sTlvPrivateBuffer   , 0,  sizeof(sTlvPrivateBuffer) );
	memset(lPinBlkFmt,    0, sizeof(lPinBlkFmt)   );
	memset(lPvka,         0, sizeof(lPvka)    );
	memset(lPvkb,         0, sizeof(lPvkb)    );
	/*memset(lPinLmk,       0, sizeof(lPinLmk)  );*/
	/*memset(lPin,          0, sizeof(lPin)     );*/
	memset(lPinBlk,       0, sizeof(lPinBlk)  );
	memset(lCardNbr,      0, sizeof(lCardNbr) );
	memset(lPvki,      0, sizeof(lPvki) );
	memset(sSecurityData, 0, sizeof ( sSecurityData ));
	/*memset(sSecurityFormat  , 0, sizeof ( sSecurityFormat ));
	memset(sTlvBuffer    , 0, sizeof ( sTlvBuffer ));*/


	/* - Preparation des donnees ------------------------------------------- */

	if(GetTlvAutho(AUTO_PAN ,sTlvInfo,lCardNbr,&nLength) != SUCCES)
	{
		trace_event(" Extract Card Number Error ", PROCESSING);
		trace_event("End   ComputePvv(ERROR)", PROCESSING);
		return(ERROR);
	}
	
	if(GetTlvAutho(AUTO_ACQUIRER_REF_DATA,sTlvInfo,lPinBlk,&nLength) != SUCCES)
	{
		trace_event(" Extract New Pin Block Data Error ", PROCESSING);
		trace_event("End   ComputePvv(ERROR)", PROCESSING);
		return(ERROR);
	}

	GetTlvAutho( AUTO_PVK_KEY , sTlvInfo, sKey, &nLength );
    /*NAB18022019 Key Block*/
	/*IBO if(sKey[0]=='S')*/
	/* IBO For HSM Atalla*/
	if(strlen(sKey) > 33)
	{
		memcpy(lPvka, sKey, nLength);
	}
	else 
	{
		memcpy(lPvka, sKey, SIZE_KEY_DES);
		memcpy(lPvkb, sKey + SIZE_KEY_DES, SIZE_KEY_DES);
	}

	if ( GetTlvAutho( AUTO_ADD_DATA_PRIVATE , sTlvInfo,sTlvPrivateBuffer , &nLength ) != SUCCES ) 
	{
		trace_event(" Extract Priv Data Error ", PROCESSING);
		trace_event("End   ComputePvv(ERROR)", PROCESSING);
		return(ERROR);
	}

	AnalyseTlvPrivate(sTlvPrivateBuffer   ,nLength,&sTlvPrivateInfo);

	GetTlvPrivate   ( ENCR_PIN_BLOCK_KEY_TAG,&sTlvPrivateInfo,sKey,&nLength);

	GetTlvAutho     ( AUTO_SECURITY_DATA,sTlvInfo,sSecurityData,&nLength);
	/*GetSecurityData ( SECURITY_FORMAT,sSecurityData,sSecurityFormat,&nLength);*/
	GetSecurityData (PIN_BLOCK_FORMAT,sSecurityData,lPinBlkFmt,&nLength);


	/* - Translation du PinBlk --------------------------------------------- */
/*	if ( memcmp(sSecurityFormat,"01",2) == 0)
	{
		lRet = PinFromTpkToLmk(nIndexCtx,sKey, lPinBlk, lPinBlkFmt, lCardNbr, lPinLmk );
	}
	else
	{
		lRet = PinFromZpkToLmk(nIndexCtx,sKey, lPinBlk, lPinBlkFmt, lCardNbr, lPinLmk);
	}

	if(lRet != OK) 
	{
		trace_event("End   ComputePvv(NOK): Error on translate PIN to LMK", PROCESSING);
		return(NOK);
	}*/

	/* - Generation du PIN Offset ------------------------------------------- */
	/*AuthoBuildTlv(sTlvBuffer, sTlvInfo);
	lPvki[0] = pvki;
	lRet =  FormatThePvvtlvBuffer ( lCardNbr,
									lPvki,
									lPvka,
									lPvkb,
									lPinLmk,
									sTlvBuffer );
	if(lRet == OK)
	{
		lRet = Production( nIndexCtx , sTlvBuffer );
	}

	if(lRet != OK) 
	{
		trace_event("End   ComputePvv(NOK)", PROCESSING);
		return(NOK);
	}         
	
	if( GetProdTlvBuffer( PROD_PVV_TAG ,sTlvBuffer, pPvv ,&nLength) != SUCCES) 
	{
		trace_event("End   ComputePvv(NOK)", PROCESSING);
		return(NOK);
	}*/

	lPvki[0] = pvki;
	lRet = generate_ABA_PVV(nIndexCtx, sKey, lPvka, lPinBlk, lPinBlkFmt, lCardNbr, lPvki, pPvv);

	if( debug_option == 1 )
	{
		sprintf(sLine, "End   ComputePvv(%s)", pPvv);
		trace_event(sLine, PROCESSING);
	}
	else
	{
		trace_event("End   ComputePvv(OK)", PROCESSING);
	}
	return(lRet);
}


/*--------------------------------------------------------------------------*/
/*	Formatage data en format TLV                                            */
/*--------------------------------------------------------------------------*/
int FormatThePvvtlvBuffer ( 
	char	*v_card_number,
	char	*v_pvkI_key,
	char	*v_pvkA_key,
	char	*v_pvkB_key,
	char	*v_pin_block,
	char	*sTlvBuffer )
{

	TSTlvProd tlvInfo;
	char sFlagsBuffer [ 10 + 1 ];
	
	InitTlvProd(&tlvInfo);
	memset (  sFlagsBuffer , 0 , sizeof ( sFlagsBuffer ));

	/**MKB271106**/
	/*memcpy (  sFlagsBuffer  , "NNNNNYNNNN", 10 );*/
	/*PROD00053984: we just need position 3
	memcpy (  sFlagsBuffer  , "NNYNNYNNNN", 10 );*/
	memcpy (  sFlagsBuffer  , "NNYNNNNNNN", 10 );
	PutTlvProd ( PROD_FLAGS_BUFFER_TAG , &tlvInfo, sFlagsBuffer ,strlen ( sFlagsBuffer));
	PutTlvProd ( PROD_PAN_TAG, &tlvInfo, v_card_number,strlen ( v_card_number ));
	PutTlvProd ( PROD_PVK_INDEX_TAG, &tlvInfo, v_pvkI_key, strlen ( v_pvkI_key ));
	PutTlvProd ( PROD_PVKA_TAG, &tlvInfo, v_pvkA_key,      strlen ( v_pvkA_key ));
	PutTlvProd ( PROD_PVKB_TAG, &tlvInfo, v_pvkB_key,      strlen ( v_pvkB_key ));
	PutTlvProd ( PROD_PIN_LMK_TAG , &tlvInfo, v_pin_block,      strlen ( v_pin_block ));
	ProdBuildTlv  ( sTlvBuffer, &tlvInfo);
	return ( OK );
}
/*--------------------------------------------------------------------------*/
/*	Formatage data en format TLV                                            */
/*--------------------------------------------------------------------------*/
int FormatTheOffsetTlvBuffer ( 
	char	*v_card_number,
	char	*v_pvkA_key,
	char	*v_pin_block,
	int     vPinLength,
	char	*sTlvBuffer )
{

	TSTlvProd tlvInfo;
	char sFlagsBuffer [ 10 + 1 ];
	char sPinLen      [3];
	char sLine [ MAX_LINE_TRC ];

	InitTlvProd(&tlvInfo);


	memset  (sPinLen,  0, sizeof(sPinLen));
	sprintf (sPinLen, "%02d", vPinLength);

	memset (  sFlagsBuffer , 0 , sizeof ( sFlagsBuffer ));
	memcpy (  sFlagsBuffer  , "NNNNNNYNNN", 10 );

	PutTlvProd ( PROD_FLAGS_BUFFER_TAG , &tlvInfo, sFlagsBuffer ,strlen ( sFlagsBuffer));
	PutTlvProd ( PROD_PAN_TAG, &tlvInfo, v_card_number,strlen ( v_card_number ));
	PutTlvProd ( PROD_PVKA_TAG, &tlvInfo, v_pvkA_key,      strlen ( v_pvkA_key ));
	PutTlvProd ( PROD_PIN_LMK_TAG , &tlvInfo, v_pin_block,      strlen ( v_pin_block ));
	PutTlvProd ( PROD_PIN_LENGTH , &tlvInfo, sPinLen,      strlen ( sPinLen )); 

	ProdBuildTlv  ( sTlvBuffer, &tlvInfo);
	return ( OK );
}

