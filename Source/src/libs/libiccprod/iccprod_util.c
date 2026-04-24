/*************************************************************************************************/
/*  (c) Hightech Payment Systems 2005                                         			 */
/*  NAME                                                                      			 */
/*      iccprod_util.c                                                        			 */
/*                                                                            			 */
/* FUNCTION                                                                    			 */
/*         Management emv fields that have tlv format (file card production)   			 */
/* NOTES                                                                       			 */
/*                                                                             			 */
/*        Y.LAMRANI   25/01/2005 -   V 2.2.0 : INITIAL VERSION		                 	 */
/*        Y.LAMRANI   17/07/2006 -   V 2.2.1 : See YL170706, Add Call fct BuildIccBufHead	 */
/*************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include <iso_iccprod.h>
#include <tools.h>
#include <define.h>
#include <emv_check_fields.h>
#include <emv_header_len.h>


#define TRAILER

#define EMV_NB_TAGS			76

int GetIccProdIcTag (char  *tag_name, TSTagIccProd *tagInfo, char *data, int *length)
{
   int nIndice ;
   int nLenTag ;
   int nDigits;
   char  sLine[MAX_LINE_TRC];

   if ( (nIndice = GetIccProdTagIndice(tag_name)) == -1)
      return(ECHEC);
   
   if (tagInfo->nPresent[nIndice] == NOT_PRESENT)
      return(ECHEC);
   
   if ( (*(tagInfo->sChipData + tagInfo->nPosTag[nIndice]) & 0x1F) == 0x1F)
      nLenTag = 2;
   else
      nLenTag = 1;
   *length = (*(tagInfo->sChipData + tagInfo->nPosTag[nIndice]+nLenTag ));

   switch (GetIccProdTagType(nIndice) )
   {
   case ICCPROD_TAG_FIX_BIN :
   case ICCPROD_TAG_FIX_ALPHA :
      memcpy( data,tagInfo->sChipData + tagInfo->nPosTag[nIndice] +nLenTag + 1,
                   *length);
      break;

   case ICCPROD_TAG_VAR_HEX :
      nDigits = HexToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] + 
                                               nLenTag+1, 
                            data, 
                            (*length)*2);
      *length = (*length)*2;
      break;
   case ICCPROD_TAG_VAR_BCD :
      nDigits = BcdToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] + 
                                               nLenTag+1, 
                            data, 
                            (*length)*2);
      *length = (*length)*2;
      break;

   case ICCPROD_TAG_FIX_BCD :
      nDigits = BcdToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] + 
                                               nLenTag+1, 
							data,
                            (*length)*2);
      *length = (*length)*2;
      break;

   case ICCPROD_TAG_VAR_BIN :
   case ICCPROD_TAG_VAR_ALPHA :
      memcpy( data, tagInfo->sChipData +tagInfo->nPosTag[nIndice] +nLenTag + 1,
                    *length);
      break;

   }
   return(SUCCES);
}
/*************************************************************************************/
int AddIccProdIcTag (char  *tag_name, TSTagIccProd *tagInfo, char *data, int length)
{
   int   nIndice;
   int   nLenTag;
   int   nLength;
   int   nDigits;
   int   nByte;
   char  sLine[MAX_LINE_TRC];
   int   Byte81I;
   int   Byte82I;
   int   TestI;

   if ( (nIndice = GetIccProdTagIndice(tag_name)) == -1)
      return(ECHEC);
   
   if (tagInfo->nPresent[nIndice] == PRESENT)
      return(ECHEC);
   
   AsciiToHex(tag_name, tagInfo->sChipData + tagInfo->nLength, 4);

   if ( (*(tagInfo->sChipData + tagInfo->nLength) & 0x1F ) == 0x1F)
      nLenTag = 2;
   else 
      nLenTag = 1;

    if(debug_option == 1 )
		sprintf(sLine,"TAG IS %s|%02X|%d",tag_name ,*(tagInfo->sChipData + tagInfo->nLength), nLenTag);
	else
	{
		if ( memcmp(tag_name, "5F34", 4) == 0 || memcmp(tag_name, "9F27", 4) == 0 || memcmp(tag_name, "57", 2) == 0)
		sprintf(sLine,"TAG IS %s|***************|%d",tag_name, nLenTag);
		else
		sprintf(sLine,"TAG IS %s|%02X|%d",tag_name ,*(tagInfo->sChipData + tagInfo->nLength), nLenTag);
	}			
   trace_event(sLine,TRACE);
   
   
   switch (GetIccProdTagType(nIndice) )
   {
   case ICCPROD_TAG_FIX_BIN :
   case ICCPROD_TAG_FIX_ALPHA :
      if ( GetIccProdTagLength(nIndice) != length)
         return(ECHEC);
      memcpy( tagInfo->sChipData + tagInfo->nLength +nLenTag + 1, data, length);
      nDigits = length;
      break;

   case ICCPROD_TAG_VAR_HEX :
      nDigits = AsciiToHex( data, 
                            tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
                            length);
      break;

   case ICCPROD_TAG_VAR_BCD :
      nDigits = AsciiToBcd( data, 
                            tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
                            length);
      break;

   case ICCPROD_TAG_FIX_BCD :
      if ( GetIccProdTagLength(nIndice) != length)
         return(ECHEC);
      nDigits = AsciiToBcd( data, 
                            tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
                            length);
      break;

   case ICCPROD_TAG_VAR_BIN :
   case ICCPROD_TAG_VAR_ALPHA :
      memcpy( tagInfo->sChipData + tagInfo->nLength +nLenTag + 1, data, length);
      nDigits = length;
      break;
/* Add cases ICCPROD_TAG_VAR_EBR2 Et ICCPROD_TAG_VAR_EBR3 */  

   case ICCPROD_TAG_VAR_ALPHA_EBR3 :
      memcpy( tagInfo->sChipData + tagInfo->nLength +nLenTag + 3, data, length);
      nDigits = length;

     Byte82I = atoi("130");
     *(tagInfo->sChipData + tagInfo->nLength + nLenTag)      = Byte82I;
     *(tagInfo->sChipData + tagInfo->nLength + nLenTag+1)    = (nDigits) / 256;
     *(tagInfo->sChipData + tagInfo->nLength + nLenTag+2)    = (nDigits) % 256;
     tagInfo->nPosTag [ nIndice ] = tagInfo->nLength;
     tagInfo->nPresent[ nIndice ] = PRESENT;
     tagInfo->nLength +=nDigits+nLenTag + 3;
     return(SUCCES); 

   case ICCPROD_TAG_VAR_EBR2 :
     nDigits = AsciiToHex( data,
                            tagInfo->sChipData + tagInfo->nLength + nLenTag+2,
                            length);  
     
     Byte81I = atoi("129");

     
     *(tagInfo->sChipData + tagInfo->nLength + nLenTag)      = Byte81I ;
     *(tagInfo->sChipData + tagInfo->nLength + nLenTag+1)    = nDigits;
     tagInfo->nPosTag [ nIndice ] = tagInfo->nLength;
     tagInfo->nPresent[ nIndice ] = PRESENT;
     tagInfo->nLength +=nDigits+nLenTag + 2;
     return(SUCCES); 

   case ICCPROD_TAG_VAR_EBR3 :
     nDigits = AsciiToHex( data,
                            tagInfo->sChipData + tagInfo->nLength + nLenTag+3,
                            length);

     Byte82I = atoi("130"); 


     *(tagInfo->sChipData + tagInfo->nLength + nLenTag)      = Byte82I;
     *(tagInfo->sChipData + tagInfo->nLength + nLenTag+1)    = (nDigits) / 256;
     *(tagInfo->sChipData + tagInfo->nLength + nLenTag+2)    = (nDigits) % 256;
     tagInfo->nPosTag [ nIndice ] = tagInfo->nLength;
     tagInfo->nPresent[ nIndice ] = PRESENT;
     tagInfo->nLength +=nDigits+nLenTag + 3;
     return(SUCCES);

   }

   *(tagInfo->sChipData + tagInfo->nLength + nLenTag)    = nDigits;
   tagInfo->nPosTag [ nIndice ] = tagInfo->nLength;
   tagInfo->nPresent[ nIndice ] = PRESENT;
   tagInfo->nLength +=nDigits+nLenTag + 1;
   return(SUCCES);
}
/*************************************************************************************/
int PutIccProdIcTag (char  *tag_name, TSTagIccProd *tagInfo, char *data, int length)
{
   int nIndice;
   int nLenTag;
   int nLength;
   int nDigits;
   int nPutPos, nTagPresent = NOT_PRESENT;
   char szLastPart  [ MAX_ICCPROD_CHIP_LEN ];
   int nOldTagLen, i;

   memset (szLastPart, '\0', sizeof(szLastPart));
   if ( (nIndice = GetIccProdTagIndice(tag_name)) == -1)
      return(ECHEC);
   
   if (tagInfo->nPresent[nIndice] == PRESENT)
   {
      nPutPos = tagInfo->nPosTag [nIndice];
      nTagPresent = PRESENT;
   }
   else
   {
      nPutPos = tagInfo->nLength;
   }
   
   if ( nTagPresent != PRESENT )
      AsciiToHex(tag_name, tagInfo->sChipData + tagInfo->nLength, 4);

   if ( (*(tagInfo->sChipData + nPutPos)) & 0x1F == 0x1F)
      nLenTag = 2;
   else 
      nLenTag = 1;

   if ( nTagPresent == PRESENT )
   {
      nOldTagLen = * ( tagInfo->sChipData + nPutPos + nLenTag );
      memcpy (szLastPart, 
              tagInfo->sChipData + nPutPos + nLenTag + nOldTagLen +1,
              tagInfo->nLength - nPutPos -nLenTag - nOldTagLen -1);
   }

   switch (GetIccProdTagType(nIndice) )
   {
   case ICCPROD_TAG_FIX_BIN :
   case ICCPROD_TAG_FIX_ALPHA :
      if ( GetIccProdTagLength(nIndice) != length)
         return(ECHEC);
      memcpy( tagInfo->sChipData + nPutPos +nLenTag + 1, data, length);
      nDigits = length;
      break;

   case ICCPROD_TAG_VAR_HEX :
      nDigits = AsciiToHex( data, 
                            tagInfo->sChipData + nPutPos + nLenTag+1, 
                            length);
      break;

   case ICCPROD_TAG_VAR_BCD :
      nDigits = AsciiToBcd( data, 
                            tagInfo->sChipData + nPutPos + nLenTag+1, 
                            length);
      break;

   case ICCPROD_TAG_FIX_BCD :
      if ( GetIccProdTagLength(nIndice) != length)
         return(ECHEC);
      nDigits = AsciiToBcd( data, 
                            tagInfo->sChipData + nPutPos + nLenTag+1, 
                            length);
      break;

   case ICCPROD_TAG_VAR_BIN :
   case ICCPROD_TAG_VAR_ALPHA :
      memcpy( tagInfo->sChipData + nPutPos +nLenTag + 1, data, length);
      nDigits = length;
      break;

   }

   *(tagInfo->sChipData + nPutPos + nLenTag)    = nDigits;
   if ( nTagPresent != PRESENT )
   {
      tagInfo->nPosTag [ nIndice ] = tagInfo->nLength;
      tagInfo->nPresent[ nIndice ] = PRESENT;
      tagInfo->nLength +=nDigits+nLenTag + 1;
   }
   else
   {
      memcpy (tagInfo->sChipData + nPutPos + nLenTag + nDigits +1,
              szLastPart, 
              tagInfo->nLength - nPutPos -nLenTag - nOldTagLen -1);
      tagInfo->nLength +=nDigits - nOldTagLen;
      for (i = nIndice + 1; i < MAX_ICCPROD_CHIP_TAG; i++)
      {
         if (tagInfo->nPresent[i] == PRESENT)
            tagInfo->nPosTag[i] += nDigits - nOldTagLen;
      }
   }
   return(SUCCES);
}


/************************************************************************************************************/
/************************************************************************************************************/
/************************************************************************************************************/
/************************************************************************************************************/
void DumpIccProdTag (char *sIccData, int nIccLen)
{

TSTagIccProd   v_IccProdInfo;
typedef struct{
    char *sTagDesc;
    char *sTagValue;
} ICCPRODTAG;

ICCPRODTAG aIccProdTag [] =
{
	{"ICC_TAG_TRACK_2_DATA         "       ,       "5700"  }, 
	{"ICC_TAG_CARDHOLDER_NAME      "       ,       "5F20"  },
	{"ICC_TAG_TRACK_1_DATA         "       ,       "9F1F"  },
	{"ICC_TAG_APPL_EXP_DATE        "       ,       "5F24"  },
	{"ICC_TAG_APPL_PRIM_ACCOUN     "       ,       "5A00"  },
	{"ICC_TAG_APPL_VERSION_NBR     "       ,       "9F08"  },
	{"ICC_TAG_APPL_CURR_CODE_1     "       ,       "9F42"  },
	{"ICC_TAG_APPL_EFFECT_DATE     "       ,       "5F25"  },
        {"ICC_TAG_APPL_SEQ_NBR         "       ,       "5F34"  },
	{"ICC_TAG_APPL_USAGE_CTRL      "       ,       "9F07"  },
	{"ICC_TAG_ISS_COUNTRY_CODE     "       ,       "5F28"  },
        {"ICC_TAG_ISS_COUNTRY_CODE1    "       ,       "9F57"  }, 
	{"ICC_TAG_APPL_SERVICE_CODE    "       ,       "5F30"  },
	{"ICC_TAG_APPL_CURRENCY_EXP    "       ,       "9F44"  },
	{"ICC_TAG_DDOL                 "       ,       "9F49"  },
	{"ICC_TAG_ISS_CODE_TABLE_IDX   "       ,       "9F11"  },
	{"ICC_TAG_LANGUAGE_PREFERENCE  "       ,       "5F2D"  },
	{"ICC_TAG_APPLICATION_ID       "       ,       "4F00"  },
	{"ICC_TAG_APPLICATION_LABEL    "       ,       "5000"  },
	{"ICC_TAG_APPL_PRIORITY_IND    "       ,       "8700"  },
	{"ICC_TAG_APPL_PREFERRED_NAME  "       ,       "9F12"  },
        {"ICC_TAG_APPL_INTERCHG_PRFIL  "       ,       "8200"  },
	{"ICC_TAG_APPL_CURR_CODE_2     "       ,       "9F51"  },
	{"ICC_TAG_APPL_DEFAULT_ACTION  "       ,       "9F52"  },
	{"ICC_TAG_APPL_TRX_COUNTER     "       ,       "9F36"  },
	{"ICC_TAG_CONSEC_TRX_LIM_INT_T "       ,       "9F53"  },
	{"ICC_TAG_CONSEC_TRX_LIM_INT_C "       ,       "9F72"  },
	{"ICC_TAG_CUMUL_TRX_AMT_LIM_TOT"       ,       "9F54"  },
        {"ICC_TAG_CUMUL_TRX_AMT_UPLIM_TOT"     ,       "9F5C"  },
	{"ICC_TAG_GEOGRAPHIC_INDICATOR "       ,       "9F55"  },

	{"ICC_TAG_ISSUER_AUTHENT_IND   "       ,       "9F56"  },
	{"ICC_TAG_LOWER_CONSEC_OFF_LIM "       ,       "9F58"  },
	{"ICC_TAG_LOWER_OFFLINE_LIM    "       ,       "9F14"  },
	{"ICC_TAG_UPPER_CONSEC_OFFLIM  "       ,       "9F59"  },
        {"ICC_TAG_UPPER_CONSEC_OFFLIM1 "       ,       "9F23"  },
        {"ICC_TAG_LAST_ONLINE_ATC_RGSTR"       ,       "9F13"  },
        {"ICC_TAG_PIN_TRY_COUNTER      "       ,       "9F17"  },
	{"ICC_TAG_SPECIFIC_DATA        "       ,       "FF60"  },
        {"ICC_TAG_KEK_KEY              "       ,       "FF22"  },
        {"ICC_TAG_KCV_KEK              "       ,       "FF29"  },
        
        {"ICC_TAG_APPLICATION_CTRL     "       ,       "D500"  },
        {"ICC_N_DOM_CTRL_FACT_EXP      "       ,       "CE00"  },
	{"ICC_TAG_LOWER_CUM_OFF_TRX_AMT"       ,       "CA00"  },
	{"ICC_TAG_UPPER_CUM_OFF_TRX_AMT"       ,       "CB00"  },
	{"ICC_TAG_CARD_TVR_ACTION_CODE "       ,       "C600"  },
	{"ICC_TAG_CARDISS_ACTCOD_OFLIN "       ,       "C400"  },
	{"ICC_TAG_CARDISS_ACTCOD_ONLIN "       ,       "C500"  },
	{"ICC_TAG_CARDISS_ACTCOD_DECLN "       ,       "C300"  },
        {"ICC_TAG_PEK_KEY              "       ,       "FF23"  }, 

	{"ICC_TAG_CARD_RISK_MANAG_CDOL1"       ,       "8C00"  },
	{"ICC_TAG_CARD_RISK_MANAG_CDOL2"       ,       "8D00"  },
	{"ICC_TAG_CA_PUBLIC_KEY_INDEX  "       ,       "8F00"  },
	{"ICC_TAG_ISS_PUBLIC_KEY_CERTIF"       ,       "9000"  },
	{"ICC_TAG_PUBLIC_KEY_CERTIF    "       ,       "9F46"  },
	{"ICC_TAG_ISS_PUBLIC_KEY_EXP   "       ,       "9F32"  },
	{"ICC_TAG_PUBLIC_KEY_EXP       "       ,       "9F47"  },
	{"ICC_TAG_SIGNED_SAD           "       ,       "9300"  },
        {"ICC_DYNAMIC_DATA             "       ,       "9F4B"  },
	{"ICC_TAG_ISS_PUBLIC_KEY_REM   "       ,       "9200"  },
	{"ICC_TAG_PUBLIC_KEY_REM       "       ,       "9F48"  },
        {"ICC_TAG_AFL                  "       ,       "9400"  },
	{"ICC_TAG_STATDATA_AUTF_TAGLIST"       ,       "9F4A"  },
	{"ICC_TAG_CVM_LIST             "       ,       "8E00"  },
	{"ICC_TAG_ISS_ACTCOD_DEFAULT   "       ,       "9F0D"  },
	{"ICC_TAG_ISS_ACTCOD_DENIAL    "       ,       "9F0E"  },
	{"ICC_TAG_ISS_ACTCOD_ONLINE    "       ,       "9F0F"  },
        {"ICC_TAG_CRYPTOGRAM_DATA      "       ,       "9F10"  },
        {"ICC_TAG_PIN_BLOCK            "       ,       "9F62"  },
	{"ICC_TAG_KEY BUFFER           "       ,       "9F60"  },

};

int  i = 0;
int  nLen = 0;
char sWork[LG_MAX];
char sLine[MAX_LINE_TRC];

trace_event("Start DumpIccProdTag()",PROCESSING);

memset(sWork, 0, sizeof(sWork));

InitIccProdIcTag ( &v_IccProdInfo );


AnalyseIccProdIc(sIccData, nIccLen, &v_IccProdInfo);



trace_event("         Field                         |  Len  |              Value",TRACE);

for (i=0; i<(sizeof(aIccProdTag)/sizeof(aIccProdTag[0])); i++)
{
	memset(sWork, 0, sizeof(sWork));

	if ( GetIccProdIcTag(aIccProdTag[i].sTagValue , &v_IccProdInfo,sWork,&nLen) == SUCCESS) 
	{
		if (debug_option != 1)
		{
			if (   memcmp(aIccProdTag[i].sTagValue, "5F34", 4) == 0 ||
                                memcmp(aIccProdTag[i].sTagValue, "9F27", 4) == 0 ||
                                memcmp(aIccProdTag[i].sTagValue, "5A", 2) == 0 ||
                                memcmp(aIccProdTag[i].sTagValue, "57", 2) == 0)
				sprintf(sLine," >> %s|%s : [%03d] : [*****************]", aIccProdTag[i].sTagValue,aIccProdTag[i].sTagDesc,nLen);
			else
				sprintf(sLine," >> %s|%s : [%03d] : [%s]", aIccProdTag[i].sTagValue,aIccProdTag[i].sTagDesc,nLen,sWork);

		}
		else	
			sprintf(sLine," >> %s|%s : [%03d] : [%s]", aIccProdTag[i].sTagValue,aIccProdTag[i].sTagDesc,nLen,sWork);
		trace_event(sLine,TRACE);
	}
}

trace_event("End   DumpIccProdTag()",PROCESSING);

}

/*****************************************************************************************************/
void TlvProdToIccProd(  char *szBufferIn,  int nLengthIn,char *szBufferOut, int *nLengthOut)
{
    TSTagIccProd          v_IccProdInfo;
    char                  sIccBuffer[LG_MAX];
    char                  sWork[LG_MAX];
    int                   nLen,i=0,j=0;
    TSTlvPrivate          TlvInfo;
    extern tlv_info_t     tab_TlvPrivateEmvInfo[EMV_NB_TAGS];
    extern tag_info_t     tab_IccProdTagInfo[];
    char                  BufHeader[LG_MAX];
    char                  BufTrailer[LG_MAX];
    char                  BufHeader2[LG_MAX];
    char                  BufHeaderHex[LG_MAX];
    TSTagIccProd          v_IccProdInfo1;
    char                  sWork1[LG_MAX];
    TSTlvPrivate          TlvInfo1;
    extern tlv_info_t     tab_TlvPrivateEmvInfo1[11];
    extern tag_info_t     tab_IccProdTagInfo1[];
    char                  sIccBuffer1[LG_MAX];
    int                   BufHeaderHexLen;
    char                  szBufferOut1[LG_MAX];
    char                  sLine[MAX_LINE_TRC];
    char                  szBufferOuttemp[8 + 1]; 
    int			  nLength; 
	int				nSize;


	
	

    trace_event("Start TlvProdToIccProd()",PROCESSING);
    memset(szBufferOut1, 0, sizeof(szBufferOut1));
    memset(sLine, 0, sizeof(sLine));
    InitIccProdIcTag( &v_IccProdInfo );
    memset(sIccBuffer, 0, sizeof(sIccBuffer));
    InitTlvPrivateEmv (&TlvInfo);
    AnalyseTlvPrivateEmv(szBufferIn + 4 ,nLengthIn,&TlvInfo);

	nSize = sizeof(tab_TlvPrivateEmvInfo) /sizeof(tlv_info_t);
    for(i = 0; i< nSize; i++)
    {


        memset(sWork       , 0 , sizeof(sWork));
       if ( GetTlvPrivateEmv ( tab_TlvPrivateEmvInfo[i].sTlvName, &TlvInfo, sWork, &nLen ) == SUCCESS )
       {
          if (i == 0)
          {
              
			  
				
				memset(BufHeader   , 0 , sizeof(BufHeader));
              memset(BufHeader2  , 0 , sizeof(BufHeader2));
              memset(BufHeaderHex, 0 , sizeof(BufHeaderHex)); 
              memset(szBufferOuttemp, 0 , sizeof(szBufferOuttemp));
            
              memcpy(BufHeader,sWork,nLen);
			  
			  memcpy(BufHeader2,BufHeader+nLenH1+nLenH2,strlen(BufHeader)- nLenH1-nLenH2);
                        						
              BufHeaderHexLen = AsciiToHex(BufHeader2 , BufHeaderHex , strlen(BufHeader2));
              CopyAscii ( BufHeaderHex ,BufHeaderHexLen, BufHeader+nLenH1+nLenH2 );
          }
          #ifdef TRAILER          
              else if (i == nSize - 1)
              {
                 memset(BufTrailer   , 0 , sizeof(BufTrailer)); 
                 memcpy(BufTrailer,sWork,nLen);
              }
          #endif
          else
          if (EmvField[i].nPresence == 'P')
          {

             if (i == 38)/*Start of Specific Data Buffer*/
             {
                InitTlvPrivateEmv (&TlvInfo1);
                InitIccProdIcTag(&v_IccProdInfo1);
               
                dump_buffer(sWork,nLen, 'X', 'X', 'X' );
               
                AnalyseTlvPrivateEmv(sWork + 4 , nLen , &TlvInfo1);
                for(j = 0; j< sizeof(tab_TlvPrivateEmvInfo1) /sizeof(tlv_info_t); j++)
                {
                   memset(sWork1, 0 , sizeof(sWork1));
                   if ( GetTlvPrivateEmv ( tab_TlvPrivateEmvInfo1[j].sTlvName, &TlvInfo1, sWork1, &nLen ) == SUCCESS )
                        AddIccProdIcTag (tab_IccProdTagInfo1[j].sTagName,&v_IccProdInfo1 , sWork1, nLen);
                   i++;
                }
                /*Build une chaine de caractére a partir de la structure v_IccProdInfo1*/
                nLen = IsoBuildIcFld (sIccBuffer1 , &v_IccProdInfo1);
                DumpIccProdTag(sIccBuffer1,nLen);
                DumpIccBuffer (sIccBuffer1,nLen);
                AddIccProdIcTag (tab_IccProdTagInfo[37].sTagName,&v_IccProdInfo ,sIccBuffer1 , nLen);
             }
             else
             {   
				sprintf(sLine," >> %s     |    %s ", tab_IccProdTagInfo[i-1].sTagName ,sWork);
				trace_event(sLine,TRACE);
				
				
				AddIccProdIcTag (tab_IccProdTagInfo[i-1].sTagName,&v_IccProdInfo , sWork, nLen);
			 
			 }
          }
       }
    }

    nLen = IsoBuildIcFld ( sIccBuffer, &v_IccProdInfo);
   /*DumpIccProdTag(sIccBuffer,nLen);*/
    DumpIccBuffer (sIccBuffer, nLen);
    dump_buffer(sIccBuffer,strlen(sIccBuffer), 'X', 'X', 'X' );
   /*Start YL170706*/
    nLength = BuildIccBufHead (sIccBuffer, nLen, BufHeader, BufTrailer, BufHeaderHexLen, szBufferOut, nLengthOut);
				

				/*End YL170706*/	

    trace_event("End   TlvProdToIccProd(void)",PROCESSING);
}


/*****************************************************************************************************/
void TlvProdToIccProdCi(  char *szBufferIn,  int nLengthIn,char *szBufferOut, int *nLengthOut)
{
    TSTagIccProd          v_IccProdInfo;
    char                  sIccBuffer[LG_MAX];
    char                  sWork[LG_MAX];
    int                   nLen,i=0,j=0;
    TSTlvPrivate          TlvInfo;
    extern tlv_info_t     tab_TlvPrivateEmvInfo[EMV_NB_TAGS];
    extern tag_info_t     tab_IccProdTagInfo[];
    char                  BufHeader[LG_MAX];
    char                  BufTrailer[LG_MAX];
    char                  BufHeader2[LG_MAX];
    char                  BufHeaderHex[LG_MAX];
    TSTagIccProd          v_IccProdInfo1;
    char                  sWork1[LG_MAX];
    TSTlvPrivate          TlvInfo1;
    extern tlv_info_t     tab_TlvPrivateEmvInfo1[11];
    extern tag_info_t     tab_IccProdTagInfo1[];
    char                  sIccBuffer1[LG_MAX];
    int                   BufHeaderHexLen;
    char                  szBufferOut1[LG_MAX];
    char                  sLine[MAX_LINE_TRC];
    char                  szBufferOuttemp[8 + 1]; 
    int			  nLength; 
	int				nSize;


	
	

    trace_event("Start TlvProdToIccProdCi()",PROCESSING);
    memset(szBufferOut1, 0, sizeof(szBufferOut1));
    memset(sLine, 0, sizeof(sLine));
    InitIccProdIcTag( &v_IccProdInfo );
    memset(sIccBuffer, 0, sizeof(sIccBuffer));
    InitTlvPrivateEmv (&TlvInfo);
    AnalyseTlvPrivateEmv(szBufferIn + 4 ,nLengthIn,&TlvInfo);

	nSize = sizeof(tab_TlvPrivateEmvInfo) /sizeof(tlv_info_t);
    for(i = 0; i< nSize; i++)
    {


        memset(sWork       , 0 , sizeof(sWork));
       if ( GetTlvPrivateEmv ( tab_TlvPrivateEmvInfo[i].sTlvName, &TlvInfo, sWork, &nLen ) == SUCCESS )
       {
          if (i == 0)
          {
              
			  
				sprintf(sLine," Header is :  %s    ",sWork);
				trace_event(sLine,TRACE);
				
				
				
				memset(BufHeader   , 0 , sizeof(BufHeader));
              memset(BufHeader2  , 0 , sizeof(BufHeader2));
              memset(BufHeaderHex, 0 , sizeof(BufHeaderHex)); 
              memset(szBufferOuttemp, 0 , sizeof(szBufferOuttemp));
            
              memcpy(BufHeader,sWork,nLen);
			  
			  memcpy(BufHeader2,BufHeader+nLenH1+nLenH2,strlen(BufHeader)- nLenH1-nLenH2);
                        
				sprintf(sLine," BufHeader2 is :  %s    ",BufHeader2);
				trace_event(sLine,TRACE);						
						
              BufHeaderHexLen = AsciiToHex(BufHeader2 , BufHeaderHex , strlen(BufHeader2));
              CopyAscii ( BufHeaderHex ,BufHeaderHexLen, BufHeader+nLenH1+nLenH2 );
          }
          #ifdef TRAILER          
              else if (i == nSize - 1)
              {
                 memset(BufTrailer   , 0 , sizeof(BufTrailer)); 
                 memcpy(BufTrailer,sWork,nLen);
              }
          #endif
          else
          if (EmvField[i].nPresence == 'P')
          {

             if (i == 38)/*Start of Specific Data Buffer*/
             {
                InitTlvPrivateEmv (&TlvInfo1);
                InitIccProdIcTag(&v_IccProdInfo1);
               
                dump_buffer(sWork,nLen, 'X', 'X', 'X' );
               
                AnalyseTlvPrivateEmv(sWork + 4 , nLen , &TlvInfo1);
                for(j = 0; j< sizeof(tab_TlvPrivateEmvInfo1) /sizeof(tlv_info_t); j++)
                {
                   memset(sWork1, 0 , sizeof(sWork1));
                   if ( GetTlvPrivateEmv ( tab_TlvPrivateEmvInfo1[j].sTlvName, &TlvInfo1, sWork1, &nLen ) == SUCCESS )
                        AddIccProdIcTag (tab_IccProdTagInfo1[j].sTagName,&v_IccProdInfo1 , sWork1, nLen);
                   i++;
                }
                /*Build une chaine de caractére a partir de la structure v_IccProdInfo1*/
                nLen = IsoBuildIcFld (sIccBuffer1 , &v_IccProdInfo1);
                DumpIccProdTag(sIccBuffer1,nLen);
                DumpIccBuffer (sIccBuffer1,nLen);
                AddIccProdIcTag (tab_IccProdTagInfo[37].sTagName,&v_IccProdInfo ,sIccBuffer1 , nLen);
             }
             else
             {   
				sprintf(sLine," >> %s     |    %s ", tab_IccProdTagInfo[i-1].sTagName ,sWork);
				trace_event(sLine,TRACE);
				
				
				AddIccProdIcTag (tab_IccProdTagInfo[i-1].sTagName,&v_IccProdInfo , sWork, nLen);
			 
			 }
          }
       }
    }

    nLen = IsoBuildIcFld ( sIccBuffer, &v_IccProdInfo);
   /*DumpIccProdTag(sIccBuffer,nLen);*/
    DumpIccBuffer (sIccBuffer, nLen);
    dump_buffer(sIccBuffer,strlen(sIccBuffer), 'X', 'X', 'X' );
   /*Start YL170706*/
    nLength = BuildIccBufHeadCi (sIccBuffer, nLen, BufHeader, BufTrailer, BufHeaderHexLen, szBufferOut, nLengthOut);
				
				
				trace_event(" XXXXXXX----------------------------------START ",TRACE);


				
				sprintf(sLine," sIccBuffer >> %s   ", sIccBuffer);
				trace_event(sLine,TRACE);

				trace_event(" XXXXXXX----------------------------------",TRACE);


				sprintf(sLine," nLen >> %d   ", nLen);
				trace_event(sLine,TRACE);

				trace_event(" XXXXXXX----------------------------------",TRACE);
				
				sprintf(sLine," BufHeader >> %s   ", BufHeader);
				trace_event(sLine,TRACE);

				trace_event(" XXXXXXX----------------------------------",TRACE);

				sprintf(sLine," BufTrailer >> %s   ", BufTrailer);
				trace_event(sLine,TRACE);
				
				trace_event(" XXXXXXX----------------------------------",TRACE);

				
				sprintf(sLine," BufHeaderHexLen >> %d   ", BufHeaderHexLen);
				trace_event(sLine,TRACE);

				trace_event(" XXXXXXX----------------------------------",TRACE);
				
				sprintf(sLine," szBufferOut >> %s   ", szBufferOut);
				trace_event(sLine,TRACE);
				trace_event(" XXXXXXX----------------------------------",TRACE);

				
				sprintf(sLine," nLengthOut >> %d   ", *nLengthOut);
				trace_event(sLine,TRACE);
				trace_event(" XXXXXXX----------------------------------END ",TRACE);
				

				/*End YL170706*/	

    trace_event("End   TlvProdToIccProdCi(void)",PROCESSING);
}


