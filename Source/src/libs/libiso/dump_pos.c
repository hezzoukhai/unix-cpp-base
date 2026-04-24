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
#include <tlv_privfields.h>
#include <tlv_private.h>
#include <iso_pos.h>


#define SUCCES  0
#define ECHEC  -1

typedef struct s_tlv_print
{
   char *description;
   char *value;
} t_tlv_print;

static char    *sSecurityDesc[] = {
        "SECURITY FORMAT         " ,
        "PIN BLOCK FORMAT        " ,
        "PIN ENCRYPTION KEY INDEX" ,
        "MAC KEY INDEX           " ,
        "SOURCE PID              " ,
        "SOURCE RESOURCE         " ,
        "DESTINATION PID         " ,
        "DESTINATION RESOURCE    " ,
        "USER                    " ,
        "PURGE TIME              " 
};   

static t_tlv_print  sAdditRespDataDesc[] = {
      { "FILE_UPDATE_ERR_CODE..." ,      "A01" },
      { "E_COMMERCE_CERTIFICATE." ,      "A02" },
      { "SECURITY_LEVEL_IND....." ,      "A03" },
      { "EXTERNAL_CRYPT_RESULTS." ,      "A04" },
      { "ADDRESS_VERIF_REQ_TYPE." ,      "A05" },
      { "ADDRESS_VERIF_REP_TYPE." ,      "A06" },
      { "TELECODE_VERIF_REQ....." ,      "A07" },
      { "TELECODE_VERIF_RES....." ,      "A08" },
	  { "BAL_TYPE_TAG..........." ,      "A09" }
};

static t_tlv_print  sPrivateDataDepiso[] = {
		{ "MESSAGE_TEXT............" , "P02" },
		{ "HPS_LIMITS_ID..........." , "P03" },
		{ "HPS_LIMITS_AMOUNT_TAG..." , "P04" },
		{ "HPS_HOST_SCENARIO_CODE.." , "P05" },
		{ "CHECK_SET_ON_OFF_LIMIT.." , "P06" },
		{ "TELECODE_VERIF_REQ	   " , "P07" },
		{ "TELECODE_VERIF_RES	   " , "P08" },
		{ "KEY_EXCHANGE_BLOC_DATA  " , "P11" },
		{ "PRIS_INTERNAL_ID	   " , "P12" },
		{ "EXTERNAL_STAN   	   " , "P13" },
		{ "PVT_DATA	           " , "P15" },
		{ "TCC	                   " , "P48" },
		{ "EXTERNAL_RESP_SOURCE    " , "P85" },
		{ "EXTERNAL_CVV_RESULT_CODE" , "P87" },
		{ "EXTERNAL_CVV2_RESULT_CODE" , "P88" },
		{ "CVC1_TRACK_EDIT_ERROR   " , "P89" },
		{ "CVC2_DATA	           " , "P92" },
		{ "HSM_RESULT_CODE	       " , "P93" },
		{ "SECONDARY_RESOURCE      " , "P94" },
		{ "MALFUNCTION_STIP        " , "P96" },
		{ "ACQUIRER GENRE          " , "P97" }
};   


static t_tlv_print  sPrivateCpsDesc[] = {
		{ "AUTH_CHAR_IND..........." , "F01" },
		{ "TRANSACTION_IND........." , "F02" },
		{ "VALIDATION_CODE........." , "F03" },
		{ "MARKET_SPECIFIC_DATA...." , "F04" },
		{ "DURATION................" , "F05" },
		{ "PRESTIGEOUS_PROP_IND...." , "F06" },
		{ "MCI_INTERCHANGE_COMP...." , "F17" }
};

static t_tlv_print  sPrivateAddPosDesc[] = {
		{ "CARD_AUTH_REL_IND ......" , "L01" },
		{ "CHIP_CONDITION_CODE....." , "L02" },
		{ "CCPS_TRANSACTION_IND...." , "L03" }
};

static t_tlv_print  sPrivateNetworkDesc[] = {
		{ "N_POS_ENTRY_MODE........" , "022" },
		{ "N_POS_CONDITION_CODE...." , "025" },
		{ "N_RESPONSE_CODE........." , "039" },
		{ "N_POS_DATA_VISA........." , "060" },
		{ "N_POS_DATA_EURO........." , "061" }
};


void dump_internal( TSIsoInfo *isoinfo )
{
   char data[LG_MAX];

   trace_event("Start dump_internal",TRACE);
   IsoBuildMsg ( data , isoinfo );
   dump_p7_data( data );
   trace_event("End   dump_internal",TRACE);
}
   
void dump_pos_buffer( char *data )
{
   char sub_data[4096];
   char buffer[4096];

   char TI[2];
   int nLength;
   int i,j,k;
   TSPosInfo msgInfo;
   TSTlvPrivate tlvInfo;

   InitPosInfo(&msgInfo);

   AnalysePos  ( data , &msgInfo);

   fprintf(stderr ,"\n------------------------------------");
   fprintf(stderr ,"\n- M.T.I      : %04d" , msgInfo.nMsgType ); 

   fprintf(stderr ,"\n------------------------------------");

   fprintf(stderr ,"\n-FLD (FIELD): CONTENT             \n");

   fprintf(stderr ,"------------------------------------");


 for( i = 1 ; i < 127 ; i++)
 {
    switch (i+1)
    {

    case 44 :
       if ( GetPosField(i, &msgInfo, data, &nLength ) == SUCCES )
       {
            fprintf(stderr ,"\n- FLD (%03d) :" , i+1 );

            InitTlvPrivate(&tlvInfo);
			AnalyseTlvPrivate( data,nLength,&tlvInfo);
			for (j=0; j<(sizeof(sAdditRespDataDesc)/sizeof(sAdditRespDataDesc [0])); j++)
   			{
    			if ( GetTlvPrivate(sAdditRespDataDesc [j].value,&tlvInfo,sub_data,&nLength) == SUCCES )
    			{
                    sub_data[nLength] = 0;
       				fprintf ( stderr ,"\n       >Tag: '%.3s' : (%25s) : [%s]",
                                        sAdditRespDataDesc [j].value,
                                        sAdditRespDataDesc [j].description,
                                        sub_data );
    			}
            }
       }
       break;


    case 48 :
       if ( GetPosField(i, &msgInfo, data, &nLength ) == SUCCES )
       {
            fprintf(stderr ,"\n- FLD (%03d) :" , i+1 );

            InitTlvPrivate(&tlvInfo);
			AnalyseTlvPrivate( data,nLength,&tlvInfo);
			for (j=0; j<(sizeof(sPrivateDataDepiso)/sizeof(sPrivateDataDepiso[0])); j++)
   			{
    			if ( GetTlvPrivate(sPrivateDataDepiso[j].value,&tlvInfo,sub_data,&nLength) == SUCCES )
    			{
                    sub_data[nLength] = 0;
       				fprintf ( stderr ,"\n       >Tag: '%.3s' : (%25s) : [%s]",
                                        sPrivateDataDepiso[j].value,
                                        sPrivateDataDepiso[j].description,
                                        sub_data );
    			}
            }
       }
       break;

    case 60 :
       if ( GetPosField(i, &msgInfo, data, &nLength ) == SUCCES )
       {
            fprintf(stderr ,"\n- FLD (%03d) :" , i+1 );

            InitTlvPrivate(&tlvInfo);
			AnalyseTlvPrivate( data,nLength,&tlvInfo);
			for (j=0; j<(sizeof(sPrivateAddPosDesc)/sizeof(sPrivateAddPosDesc[0])); j++)
   			{
    			if ( GetTlvPrivate(sPrivateAddPosDesc[j].value,&tlvInfo,sub_data,&nLength) == SUCCES )
    			{
                    sub_data[nLength] = 0;
       				fprintf ( stderr ,"\n       >Tag: '%.3s' : (%25s) : [%s]",
                                        sPrivateAddPosDesc[j].value,
                                        sPrivateAddPosDesc[j].description,
                                        sub_data );
    			}
            }
       }
       break;

    case 61 :
       if ( GetPosField(i, &msgInfo, data, &nLength ) == SUCCES )
       {
            fprintf(stderr ,"\n- FLD (%03d) :" , i+1 );

            InitTlvPrivate(&tlvInfo);
			AnalyseTlvPrivate( data,nLength,&tlvInfo);
			for (j=0; j<(sizeof(sPrivateNetworkDesc)/sizeof(sPrivateNetworkDesc[0])); j++)
   			{
    			if ( GetTlvPrivate(sPrivateNetworkDesc[j].value,&tlvInfo,sub_data,&nLength) == SUCCES )
    			{
                    sub_data[nLength] = 0;
       				fprintf ( stderr ,"\n       >Tag: '%.3s' : (%25s) : [%s]",
                                        sPrivateNetworkDesc[j].value,
                                        sPrivateNetworkDesc[j].description,
                                        sub_data );
    			}
            }
       }
       break;

    case 62 :
       if ( GetPosField(i, &msgInfo, data, &nLength ) == SUCCES )
       {
            fprintf(stderr ,"\n- FLD (%03d) :" , i+1 );

            InitTlvPrivate(&tlvInfo);
			AnalyseTlvPrivate( data,nLength,&tlvInfo);
			for (j=0; j<(sizeof(sPrivateCpsDesc)/sizeof(sPrivateCpsDesc[0])); j++)
   			{
    			if ( GetTlvPrivate(sPrivateCpsDesc[j].value,&tlvInfo,sub_data,&nLength) == SUCCES )
    			{
                    sub_data[nLength] = 0;
       				fprintf ( stderr ,"\n       >Tag: '%.3s' : (%25s) : [%s]",
                                        sPrivateCpsDesc[j].value,
                                        sPrivateCpsDesc[j].description,
                                        sub_data );
    			}
            }
       }
       break;
/*
    case 53 :
       if ( GetPosField(i, &msgInfo, data, &nLength ) == SUCCES )
       {
            fprintf(stderr ,"\n- FLD (%03d) :" , i+1 );

             for ( j=0; j< 10 ; j ++ )
             {
			    GetSecurityData (j,data,sub_data,&nLength);
                fprintf(stderr ,"\n       > %.25s ..............: [%s] " , sSecurityDesc [ j ] , sub_data    );

             }
       }
       break;
*/

	case 2  :
	case 35 :
	case 51 :
        if ( GetPosField(i, &msgInfo, data, &nLength ) == SUCCES )
		if (debug_option == 1) 
          fprintf(stderr ,"\n- FLD (%03d) : [%s] Length [%06d]" , i+1 , data , nLength );
		else
          fprintf(stderr ,"\n- FLD (%03d) : [***********] Length [%06d]" , i+1  , nLength );

       break;
    default :
        if ( GetPosField(i, &msgInfo, data, &nLength ) == SUCCES )
          fprintf(stderr ,"\n- FLD (%03d) : [%s] Length [%06d]" , i+1 , data , nLength );

       break;
    }
 }

   fprintf(stderr ,"\n------------------------------------");
   fprintf ( stderr , "\n");
}
