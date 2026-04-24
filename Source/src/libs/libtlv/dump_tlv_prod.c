# include <stdio.h>
# include <stddef.h>
# include <locale.h>
# include <stdlib.h>
# include <define.h>
# include <tlv_prod.h>
# include <tlv_prodfields.h>
# include <dump.h>

typedef struct s_tlv_print
{
	char *description;
	char *value;
} t_tlv_print;


t_tlv_print sTlvProd[] = {
	{       "ERROR_CODE" , 	   "P01"  },
	{       "ERROR_TEXT" ,       "P02" },
	{       "FLAGS_BUFFER" ,     "P03" },
	{       "PAN" ,              "P04" },
	{       "CVKA" ,             "P05" },
	{       "CVKB" ,             "P06" },
	{       "SERVICE_CODE" ,     "P07" },
	{       "EXPIRY_DATE" ,      "P08" },
	{       "CVV2" ,    		   "P09" },
	{       "CVV1" ,    		   "P10" },
	{       "PVK_INDEX" ,        "P11" },
	{       "PVKA" ,    		   "P12" },
	{       "PVKB" ,    		   "P13" },
	{       "PIN_LMK" ,    	   "P14" },
	{       "PVV" ,    		   "P15" },
	{       "PRINT_FIELD_0" ,    "P16" },
	{       "PRINT_FIELD_1" ,    "P17" },
	{       "PRINT_FIELD_2" ,    "P18" },
	{       "PRINT_FIELD_3" ,    "P19" },
	{       "PRINT_FIELD_4" ,    "P20" },
	{       "PRINT_FIELD_5" ,    "P21" },
	{       "PRINT_FIELD_6" ,    "P22" }
};		


int PrintProdTlvBuffer ( char *sTlvBuffer )
{

	TSTlvProd      sTlvInfo;
	int  length;
	int i;
	char data[999];
	char ligne[999];
	int  nOffset = 0;

	memset  ( data ,0,  sizeof ( data ) );
	memset ( ligne ,0, sizeof ( ligne ) );
	InitTlvProd(&sTlvInfo);
	AnalyseTlvProd( sTlvBuffer , &sTlvInfo );
	sprintf(ligne ,"-----------------------------------------");
	trace_event ( ligne , PROCESSING );

	sprintf(ligne ,"%.3s\t %20s \t%s\t%s","TAG","DESC","LEN","VALUE" );
	trace_event ( ligne , PROCESSING );

	sprintf(ligne ,"%.3s\t-%20s-\t%s\t%s","---","-----","---","-----" );
	trace_event ( ligne , PROCESSING );


	for (i=0; i<(sizeof(sTlvProd)/sizeof(sTlvProd[0])); i++)
	{
		memset (  data ,0, sizeof ( data ) );
		if ( GetTlvProd(sTlvProd[i].value,&sTlvInfo,data,&length) == SUCCES )
		{

			if(debug_option != 1 )
			{
				if (	(memcmp(sTlvProd[i].value, "P04", 2) == 0) ||
					(memcmp(sTlvProd[i].value, "P05", 2) == 0) ||
					(memcmp(sTlvProd[i].value, "P06", 2) == 0) ||
					(memcmp(sTlvProd[i].value, "P07", 2) == 0) ||
					(memcmp(sTlvProd[i].value, "P08", 2) == 0) ||
					(memcmp(sTlvProd[i].value, "P09", 2) == 0) ||
					(memcmp(sTlvProd[i].value, "P10", 2) == 0) ||
					(memcmp(sTlvProd[i].value, "P12", 2) == 0) ||
					(memcmp(sTlvProd[i].value, "P13", 2) == 0) ||
					(memcmp(sTlvProd[i].value, "P14", 2) == 0) ||
					(memcmp(sTlvProd[i].value, "P15", 2) == 0) )
				{
					memset (  data ,0, sizeof ( data ) );				
					memcpy (data, "********************", 20);
				}
			}
			sprintf ( ligne ,"%.3s\t{%20s}\t%03d\t%s",  
				sTlvProd[i].value,
				sTlvProd[i].description,
				length,
				data );
			trace_event ( ligne , PROCESSING );

		}
		/*trace_event ( ligne , PROCESSING );*/

	}



	memset (  data ,0, sizeof ( data ) );
	sprintf(ligne ,"-----------------------------------------");
	trace_event ( ligne , PROCESSING );
	/*SKO290509*/ return(OK);
}



