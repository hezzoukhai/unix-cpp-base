/****************************************************************************/
/***************************************************************************/
/*            UTILS_ISO.H  : UTILITAIRE POUR COPIER ET AFFICHER            */
/*                           MESSAGE  ISO 8583  EURO    (*F.M*)            */
/***************************************************************************/
/***************************************************************************/
#include <stdio.h>
#include <header_sms.h>
#include <define.h>
#include <iso_hps.h>
#include <iso_pos.h>
#include <tlv_privfields.h>
#include <tlv_private.h>

#define SUCCES  0
#define ECHEC  -1

typedef struct s_tlv_print
{
	char *description;
	char *value;
} t_tlv_print;


typedef struct s_pos_param
{
	char *label;
	int  pos;
	int  len;
} t_pos_param;

/* START BIM & MRA -- 13102008 -- RECTIFICATION AFFICHAGE FICHIER POS PARAMETER */

static t_pos_param  sPosParam[] = {

	{"Numero de terminal              ",   1,  12},
	{"Type de terminal                ",  13,   2},
	{"Type d application              ",  15,   2},
	{"Nom application                 ",  17,  10},
	{"Enseigne point de vente         ",  27,  24},
	{"Adresse                         ",  51,  24},
	{"MCC                             ",  75,   4},
	{"Ville                           ",  79,  13},
	{"Mot de passe                    ",  92,   4},
	{"Masque reseaux d acceptation    ",  96,   9},
	{"Numero de serie du terminal     ", 105,  10},
	{"Numero de registre de commerce  ", 115,   9},
	{"Heure de fin de journee (HHMM)  ", 124,   4},
	{"Affichage terminal et ticket    ", 128,  11},
	{"Seuil de transaction offline    ", 139,   2},
	{"Nbr de trans d avis maximum     ", 141,   2},
	{"Capture date (MMJJ)             ", 143,   4},
	{"Indic Pin Pad     (0=Non/1=Oui) ", 147,   1},
	{"Saisie manuelle   (0=Non/1=Oui) ", 148,   1},
	{"Gestion timeout   (0=Non/1=Oui) ", 149,   1},
	{"Annulation/timeout(0=Non/1=Oui) ", 150,   1},
	{"Accep carte � puce(0=Non/1=Oui) ", 151,   1},
	{"Indic impression  (0=Non/1=Oui) ", 152,   1},
	{"Indic annulation  (0=Non/1=Oui) ", 153,   1},
	{"For�age           (0=Non/1=Oui) ", 154,   1},
	{"Chgt domiciliation(0=Non/1=Oui) ", 155,   1},
	{"Pre autorisation  (0=Non/1=Oui) ", 156,   1},
	{"Type d appel (0=Tonal/1=Impuls) ", 157,   1},
	{"Commerce offshore (0=Non/1=Oui) ", 158,   1},
	{"Indic Refund      (0=Non/1=Oui) ", 159,   1},
	{"Indic Cash back   (0=Non/1=Oui) ", 160,   1},
	{"Indic TIP         (0=Non/1=Oui) ", 161,   1},
	{"Indic Instalment  (0=Non/1=Oui) ", 162,   1},
	{"Indic offline     (0=Non/1=Oui) ", 163,   1},
	{"Type d application              ", 164,   1},
	{"Indic paiem fact  (0=Non/1=Oui) ", 165,   1},
	{"Indic rechar  GSM (0=Non/1=Oui) ", 166,   1},
	{"Indic rechar carte(0=Non/1=Oui) ", 167,   1},
	{"Indic Dmd Solde   (0=Non/1=Oui) ", 168,   1},
	{"Indic Act Carte   (0=Non/1=Oui) ", 169,   1},
	{"Reserves                        ", 170,   7},
	{"Numero d appel serveur   autor  ", 177,  15},
	{"Adr X25 ou IP du serveur autor  ", 192,  16},
	{"Numero d appel serveur de teleco", 208,  15},
	{"Adr X25 ou IP serveur de telec  ", 223,  16},
	{"Numero du serveur application   ", 239,  15},
	{"Adr X25 ou IP srv application   ", 254,  16},
	{"Numero appel srv autor- Secours ", 270,  15},
	{"Adr X25 ou IP srv autor�Secours ", 285,  16},
	{"Numero appel srv tele - Secours ", 301,  15},
	{"Adr X25 ou IP srv telec�Secours ", 316,  16},
	{"Numero du srv applica - Secours ", 332,  15},
	{"Adr X25 ou srv applica- Secours ", 347,  16},
	{"Monnaie acceptation 1-Numerique ", 363,   3},
	{"Monnaie acceptation 1-Alpha     ", 366,   3},
	{"Monnaie acceptation 1-Exposant  ", 369,   1},
	{"Montant minimum - monnaie 1     ", 370,   4},
	{"Monnaie acceptation 2-Numerique ", 374,   3},
	{"Monnaie acceptation 2-Alpha     ", 377,   3},
	{"Monnaie acceptation 2- Exposant ", 380,   1},
	{"Montant minimum - monnaie 2     ", 381,   4},
	{"Monnaie acceptation 3-Numerique ", 385,   3},
	{"Monnaie acceptation 3-Alpha     ", 388,   3},
	{"Monnaie acceptation 3- Exposant ", 391,   1},
	{"Montant minimum - monnaie 3     ", 392,   4},
	{"Monnaie acceptation 4-Numerique ", 396,   3},
	{"Monnaie acceptation 4-Alpha     ", 399,   3},
	{"Monnaie acceptation 4- Exposant ", 402,   1},
	{"Montant minimum - monnaie 4     ", 403,   4},
	{"Monnaie acceptation 5-Numerique ", 407,   3},
	{"Monnaie acceptation 5-Alpha     ", 410,   3},
	{"Monnaie acceptation 5- Exposant ", 413,   1},
	{"Montant minimum - monnaie 5     ", 414,   4},
	{"Monnaie acceptation 6-Numerique ", 418,   3},
	{"Monnaie acceptation 6-Alpha     ", 421,   3},
	{"Monnaie acceptation 6- Exposant ", 424,   1},
	{"Montant minimum - monnaie 6     ", 425,   4},
	{"Monnaie de reglement1-Numerique ", 429,   3},
	{"Monnaie de reglement1-Alpha     ", 432,   3},
	{"Monnaie de reglement1 Exposant  ", 435,   1},
	{"Monnaie de reglement2-Numerique ", 436,   3},
	{"Monnaie de reglement2-Alpha     ", 439,   3},
	{"Monnaie de reglement2 Exposant  ", 442,   1},
	{"Monnaie de reglement3-Numerique ", 443,   3},
	{"Monnaie de reglement3-Alpha     ", 446,   3},
	{"Monnaie de reglement3 Exposant  ", 449,   1},
	{"Monnaie de reglement4-Numerique ", 450,   3},
	{"Monnaie de reglement4-Alpha     ", 453,   3},
	{"Monnaie de reglement4 Exposant  ", 456,   1},
	{"Monnaie de reglement5-Numerique ", 457,   3},
	{"Monnaie de reglement5-Alpha     ", 460,   3},
	{"Monnaie de reglement5 Exposant  ", 463,   1},
	{"Reserve                         ", 464,   5},
	{"Langue                          ", 472,   1},
	{"Valeur time out (s)             ", 473,   3},
	{"Valeur Nombre d essai maximum   ", 476,   1},
	{"Valeur intervalle entre les conn", 477,   3},
	{"Indic Saisie 4digit(0=Non/1=Oui)", 480,   1},
	{"Reserve                         ", 481,  20}
};



static t_tlv_print  sAdditRespDataDescpiso[] = {
	{ "BAL_TYPE_TAG..........." ,      "A09" }
};

static t_tlv_print  sPrivateDataDescpiso[] = {
	{ "FILE_UPDATE_ERR_CODE...." , "P01" },
	{ "MESSAGE_TEXT............" , "P02" },
	{ "HPS_LIMITS_ID..........." , "P03" },
	{ "HPS_LIMITS_AMOUNT_TAG..." , "P04" },
	{ "HPS_HOST_SCENARIO_CODE.." , "P05" },
	{ "CHECK_SET_ON_OFF_LIMIT.." , "P06" },
	{ "TELECODE_VERIF_REQ......" , "P07" },
	{ "TELECODE_VERIF_RES......" , "P08" },
	{ "HPS_ACQUIRER_BANK......." , "P09" },
	{ "KEY_EXCHANGE_BLOC_DATA.." , "P11" },
	{ "PRIS_INTERNAL_ID........" , "P12" },
	{ "EXTERNAL_STAN..........." , "P13" },
	{ "PVT_DATA................" , "P15" },
	{ "E_COMMERCE_CERTIFICATE.." , "P40" },
	{ "SECURITY_LEVEL_IND......" , "P41" },
	{ "EXTERNAL_CRYPT_RESULTS.." , "P44" },
	{ "TCC....................." , "P48" },
	{ "TERM_INFO_DATA.........." , "P70" },	/* ELKAM20230524 */
	{ "ADDRESS_VERIF_REQ_TYPE.." , "P82" },
	{ "ADDRESS_VERIF_REP_TYPE.." , "P83" },
	{ "EXTERNAL_RESP_SOURCE    " , "P85" },
	{ "EXTERNAL_CVV_RESULT_CODE" , "P87" },
	{ "EXTERNAL_CVV2_RESULT_CODE", "P88" },
	{ "CVC1_TRACK_EDIT_ERROR   " , "P89" },
	{ "CVC2_DATA	           " , "P92" },
	{ "HSM_RESULT_CODE	       " , "P93" },
	{ "SECONDARY_RESOURCE      " , "P94" },
	{ "MALFUNCTION_STIP        " , "P96" },
	{ "ACQUIRER GENRE          " , "P97" },
	{ "TPK/AWK                 " , "P10" },
	{ "DINERS NET REF ID       " , "P80" } /* ELKAM20230217 PLUTONL-5914 */
};   


static t_tlv_print  sPrivateCpsDescpiso[] = {
	{ "AUTH_CHAR_IND..........." , "F01" },
	{ "TRANSACTION_IND........." , "F02" },
	{ "VALIDATION_CODE........." , "F03" },
	{ "MARKET_SPECIFIC_DATA...." , "F04" },
	{ "DURATION................" , "F05" },
	{ "PRESTIGEOUS_PROP_IND...." , "F06" },
	{ "MCI_INTERCHANGE_COMP...." , "F17" }
};

static t_tlv_print  sPrivateAddPosDescpiso[] = {
	{ "CARD_AUTH_REL_IND ......" , "L01" },
	{ "CHIP_CONDITION_CODE....." , "L02" },
	{ "CCPS_TRANSACTION_IND...." , "L03" }
};

static t_tlv_print  sPrivateNetworkDescpiso[] = {
	{ "N_POS_ENTRY_MODE........" , "022" },
	{ "N_POS_CONDITION_CODE...." , "025" },
	{ "N_RESPONSE_CODE........." , "039" },
	{ "N_POS_DATA_VISA........." , "060" },
	{ "N_POS_DATA_EURO........." , "061" },
	{ "N_FILE_UPDATE_CODE......" , "091" },
	{ "N_UF_ACTION_CODE........" , "127" }
};



static t_pos_param  sPosVersion[] = {
	{"MAC Key..................",		1,		3},
	{"Terminal Master Key......",		4,		3},
	{"EMV tables...............",		7,		3},
	{"BIN_LOCAUX & PREFIXES....",		10,		3},
	{"Stoplist.................",		13,		4},
	{"POS PARAMETERS...........",		17,		3},
	{"Software.................",		20,		4},
	{"Domiciliation accounts...",		24,		3},
	{"Contactless..............",		27,		3},
	{"Paywave..................",		30,		3},
	/*{"DCC rates................",		26,		4},*/
};


void dump_pos_struct( TSPosInfo *pisoInfo )
{

	char sWork[LG_MAX];
	char sLine[MAX_LINE_TRC];
	int nLength;
	int iMapCtr,i,j,k;
	int nPos = 0;
	TSTlvPrivate tlvInfo;
	char	CardTrunc[22 + 1];
	char	field[256];
	char	sub_data[BUF_LG_MAX]; /*AMER 20160603*/
	int		nSize;

	trace_event("Start dump_pos_struct()",PROCESSING);

	memset(sWork, 0, sizeof(sWork));
	memset(CardTrunc, 0, sizeof(CardTrunc));
        memset(sub_data,     '\0', sizeof(sub_data));	


	iMapCtr = 1;
	while ( ( pisoInfo->sBitMap [(iMapCtr - 1) * 8] & 0x80)  && (iMapCtr < 3) )
		++iMapCtr;


	memcpy(field, (char*)&pisoInfo->sHeader, sizeof(TSPosHeader));
	field[sizeof(TSPosHeader)]='\0';

	trace_event("------------------------------------",TRACE);
	sprintf(sLine,"- MSG HEADER : %.16s" , field ); 
	trace_event(sLine,TRACE);
	trace_event("------------------------------------",TRACE);
	sprintf(sLine,"- M.T.I      : %04d" , pisoInfo->nMsgType ); 
	trace_event(sLine,TRACE);
	trace_event("------------------------------------",TRACE);
	memcpy(sLine,"- BIT MAP  : ",13); nPos+=13;
	for (i=0; i<iMapCtr * 8;i++)
	{
		sprintf(sLine + nPos," %02X", pisoInfo->sBitMap[i]);
		nPos += 3;
	}
	trace_event(sLine,TRACE);
	trace_event("------------------------------------",TRACE);
	trace_event("- FLD (FIELD): LENGTH :  DATA       ",TRACE);
	trace_event("------------------------------------",TRACE);



	for( i = 1 ; i < 128 ; i++)
	{
		switch (i+1)
		{
		case 2: 
			if ( GetPosField(i, pisoInfo, sWork, &nLength ) == SUCCES )
			{
				if (debug_option == 1)
					sprintf(sLine,"- FLD (%03d) : (%03d) : [%s] " , i+1 , nLength, sWork );
				else{
					TrunCard(sWork, CardTrunc);
					sprintf(sLine,"- FLD (%03d) : (%03d) : [%s]" , i+1 , nLength, CardTrunc);
				}
			}
			trace_event(sLine,TRACE);
			break;
		case 44:
			if ( GetPosField(POS_ADTNL_RESP_DATA, pisoInfo, sWork, &nLength ) == SUCCES )
			{
				sprintf(sLine,"- FLD (%03d) : (%03d) : [%s] " , i+1 , nLength ,sWork );
				trace_event(sLine,TRACE);

				sprintf(sLine,"       > %.33s......: [%c] " , "EMV tables..........." ,sWork[0] );trace_event(sLine,TRACE);
				sprintf(sLine,"       > %.33s......: [%c] " , "Application.........." ,sWork[1] );trace_event(sLine,TRACE);
				sprintf(sLine,"       > %.33s......: [%c] " , "DCC_RATES............" ,sWork[2] );trace_event(sLine,TRACE);
				sprintf(sLine,"       > %.33s......: [%c] " , "BINS................." ,sWork[3] );trace_event(sLine,TRACE);
				sprintf(sLine,"       > %.33s......: [%c] " , "Stoplist............." ,sWork[4] );trace_event(sLine,TRACE);
				sprintf(sLine,"       > %.33s......: [%c] " , "Master key..........." ,sWork[5] );trace_event(sLine,TRACE);
				sprintf(sLine,"       > %.33s......: [%c] " , "MAC key.............." ,sWork[6] );trace_event(sLine,TRACE);
				sprintf(sLine,"       > %.33s......: [%c] " , "domiciliation account" ,sWork[7] );trace_event(sLine,TRACE);
				sprintf(sLine,"       > %.33s......: [%c] " , "POS parameters......." ,sWork[8] );trace_event(sLine,TRACE);

			}
			break;
		case 35: 
		case 45: 
		case 52:
			if ( GetPosField(i, pisoInfo, sWork, &nLength ) == SUCCES )
			{
				if (debug_option == 1)
					sprintf(sLine,"- FLD (%03d) : (%03d) : [%s] " , i+1 , nLength, sWork );
				else
					sprintf(sLine,"- FLD (%03d) : (%03d) : [*******************] " , i+1 , nLength );
				trace_event(sLine,TRACE);
			}
			break;
		/*AMER 20160603 Start*/
        case 48 :
                if ( GetPosField(i, pisoInfo, sWork, &nLength ) == SUCCES )
                {
                        sprintf(sLine,"- FLD (%03d) : (%03d) : " , i+1 , nLength );
                        trace_event(sLine,TRACE);

                        InitTlvPrivate(&tlvInfo);
                        AnalyseTlvPrivate( sWork,nLength,&tlvInfo);
                        for (j=0; j<(sizeof(sPrivateDataDescpiso)/sizeof(sPrivateDataDescpiso[0])); j++)
                        {
                                if ( GetTlvPrivate(sPrivateDataDescpiso[j].value,&tlvInfo,sub_data,&nLength) == SUCCES )
                                {
                                        sub_data[nLength] = 0;
                                        if( memcmp(sPrivateDataDescpiso[j].value, "P10", 3) == 0 ||
										    memcmp(sPrivateDataDescpiso[j].value, "P15", 3) == 0 ||
											memcmp(sPrivateDataDescpiso[j].value, "P16", 3) == 0 ||
											memcmp(sPrivateDataDescpiso[j].value, "P92", 3) == 0)
                                        {
                                                 if (debug_option == 1)
                                                        sprintf(sLine ,"       >Tag: '%.3s' : (%25s) : [%s]",
                                                                sPrivateDataDescpiso[j].value,
                                                                sPrivateDataDescpiso[j].description,
                                                                sub_data );
                                                else
                                                        sprintf(sLine ,"       >Tag: '%.3s' : (%25s) : [****************]",
                                                                sPrivateDataDescpiso[j].value,
                                                                sPrivateDataDescpiso[j].description);
                                                trace_event(sLine,TRACE);
                                        }
                                        else
                                        {
                                                sprintf(sLine ,"       >Tag: '%.3s' : (%25s) : [%s]",
                                                sPrivateDataDescpiso[j].value,
                                                sPrivateDataDescpiso[j].description,
                                                sub_data);
                                                trace_event(sLine,TRACE);
                                        }
                                }
                        }
                }
                break;
        /*AMER 20160603 End*/

		case 55 :
			if ( GetPosField(i, pisoInfo, sWork, &nLength ) == SUCCES )
			{
				char sTag [ 4 + 1], sValue[512];
				int  nLen;
				unsigned char leftTag,rightTag;

				sprintf(sLine,"- FLD (%03d) : (%03d) : " , i+1 , nLength );
				trace_event(sLine,TRACE);
				for( j = 0 ; j < nLength; )
				{
					memset(sTag, 0, sizeof(sTag));
					memset(sValue, 0, sizeof(sValue));

					if (((unsigned char)sWork[j] & 0x1F) == 0x1F)
					{
						leftTag=(unsigned char)sWork[j++];
						rightTag=(unsigned char)sWork[j++];
						sprintf(sTag, "%02X%02X", leftTag, rightTag);
					}
					else
					{
						leftTag=(unsigned char)sWork[j++];
						sprintf(sTag, "%02X  ", leftTag);
					}

					nLen = sWork[j++];

					HexToAscii(sWork+j,sValue,nLen*2);
					/*AMER 20160520: 'memcmp(sTag, "9F27", 4) == 0' removed from the if test, because "9F27" ["ISO_TAG_CRYPTO_					  INFO_DATA"] is not a sensitive data*/ 
					if (debug_option != 1)
					{
						if ( memcmp(sTag, "5F34", 4) == 0 || memcmp(sTag, "57", 2) == 0 || memcmp(sTag, "5A", 2) == 0)
							sprintf(sLine,"      > TAG (%.4s) ....: [**** Len %lu ****]" , sTag  , strlen(sValue)   );
						else
							sprintf(sLine,"      > TAG (%.4s) ....: [%s]" , sTag , sValue    );
					}
					else
						sprintf(sLine,"      > TAG (%.4s) ....: [%s]" , sTag , sValue    );
					trace_event(sLine,TRACE);


					j += nLen;
				}
				memset(sValue, 0, sizeof(sValue));
			}
			break;

		case 60 :
			if ( GetPosField(i, pisoInfo, sWork, &nLength ) == SUCCES )
			{
				sprintf(sLine,"- FLD (%03d) : (%03d) : " , i+1 , nLength );
				trace_event(sLine,TRACE);

				InitTlvPrivate(&tlvInfo);
				AnalyseTlvPrivate( sWork,nLength,&tlvInfo);
				for (j=0; j<(sizeof(sPrivateAddPosDescpiso)/sizeof(sPrivateAddPosDescpiso[0])); j++)
				{
					if ( GetTlvPrivate(sPrivateAddPosDescpiso[j].value,&tlvInfo,sWork,&nLength) == SUCCES )
					{
						sWork[nLength] = 0;
						sprintf ( sLine ,"       >Tag: '%.3s' : (%25s) : [%s]",
							sPrivateAddPosDescpiso[j].value,
							sPrivateAddPosDescpiso[j].description,
							sWork );
						trace_event(sLine,TRACE);
					}
				}
			}
			break;

		case 61 :
			if ( GetPosField(i, pisoInfo, sWork, &nLength ) == SUCCES )
			{
				sprintf(sLine,"- FLD (%03d) : (%03d) : " , i+1 , nLength );
				trace_event(sLine,TRACE);

				InitTlvPrivate(&tlvInfo);
				AnalyseTlvPrivate( sWork,nLength,&tlvInfo);
				for (j=0; j<(sizeof(sPrivateNetworkDescpiso)/sizeof(sPrivateNetworkDescpiso[0])); j++)
				{
					if ( GetTlvPrivate(sPrivateNetworkDescpiso[j].value,&tlvInfo,sWork,&nLength) == SUCCES )
					{
						sWork[nLength] = 0;
						sprintf ( sLine ,"       >Tag: '%.3s' : (%25s) : [%s]",
							sPrivateNetworkDescpiso[j].value,
							sPrivateNetworkDescpiso[j].description,
							sWork );
						trace_event(sLine,TRACE);
					}
				}
			}
			break;
		case 72 :
			if ( GetPosField(ISO_FILE_NAME, pisoInfo, sWork, &nLength ) == SUCCES )
			{
				if(strcmp(sWork, "POS PARAMETER") == 0)
				{	
					if ( GetPosField(i, pisoInfo, sWork, &nLength ) == SUCCES )
					{
						sprintf(sLine,"- FLD (%03d) : (%03d) : " , i+1 , nLength );
						trace_event(sLine,TRACE);

						for (j=0; j<(sizeof(sPosParam)/sizeof(sPosParam[0])); j++)
						{
							memcpy (field,sWork + sPosParam[j].pos - 1, sPosParam[j].len);
							field[sPosParam[j].len]='\0';
							sprintf(sLine ,"       > %.33s ......: [%s] " ,  sPosParam[j].label  , field    );
							trace_event(sLine,TRACE);

						}
						break;
					}
				}
			}
			if ( GetPosField(i, pisoInfo, sWork, &nLength ) == SUCCES )
			{
				sprintf(sLine,"- FLD (%03d) : (%03d) : [%s] " , i+1 , nLength ,sWork );
				trace_event(sLine,TRACE);
			}
			break;
		case 63:
			if ( GetPosField(POS_VERSIONS, pisoInfo, sWork, &nLength ) == SUCCES )
			{
				sprintf(sLine,"- FLD (%03d) : (%03d) : " , i+1 , nLength );
				trace_event(sLine,TRACE);
				nSize = (sizeof(sPosVersion)/sizeof(sPosVersion[0]));
				for (j=0; j<nSize; j++)
				{
					memcpy (field,sWork + sPosVersion[j].pos - 1, sPosVersion[j].len);
					field[sPosVersion[j].len]='\0';
					sprintf(sLine ,"       > %.33s......: [%s] " ,  sPosVersion[j].label, field);
					trace_event(sLine,TRACE);
				}
			}
			break;
		default :
			if ( GetPosField(i, pisoInfo, sWork, &nLength ) == SUCCES )
			{
				sprintf(sLine,"- FLD (%03d) : (%03d) : [%s] " , i+1 , nLength ,sWork );
				trace_event(sLine,TRACE);
			}

			break;
		}
	}

	memset(sWork, 0, sizeof(sWork));
	memset(sLine, 0, sizeof(sLine));
	memset(CardTrunc, 0, sizeof(CardTrunc));
	memset(sub_data, '\0', sizeof(sub_data));
	InitTlvPrivate(&tlvInfo);

	trace_event("------------------------------------",TRACE);
	trace_event("End   dump_pos_struct()",PROCESSING);



}


void DumpPosIso( char *data )
{
	
	TSPosInfo msgInfo;
	trace_event("Start DumpPosIso()",PROCESSING);
	InitPosInfo(&msgInfo);
	AnalysePos  ( data , &msgInfo);
	dump_pos_struct(&msgInfo);
        InitPosInfo(&msgInfo);
	trace_event("End   DumpPosIso()",PROCESSING);
}


