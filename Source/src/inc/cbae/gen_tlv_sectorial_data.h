#ifndef tlv_sectorial_data_H
#define tlv_sectorial_data_H
/*This file is generated and should not be modified manually*/




typedef struct 
{
 int nPresent [ F113_NB_FIELDS + 1];
 int nPosTlv [ F113_NB_FIELDS + 1];
 int nLength;
 char sTlvData [ F113_BUF_LEN ];
 
}
 TSTlvSectorialData;
 void InitTlvSectorialData(TSTlvSectorialData * tlvInfo);
 int AnalyseTlvSectorialData(char *buffer,TSTlvSectorialData *tlvInfo);
 int GetTlvSectorialData (char *tlv_name, TSTlvSectorialData *tlvInfo,char *data, int *length);
 int AddTlvSectorialData (char *tlv_name, TSTlvSectorialData *tlvInfo, char *data, int length);
 int PutTlvSectorialData (char *tlv_name, TSTlvSectorialData *tlvInfo, char *data, int length);
 void DumpSectorialData(TSTlvSectorialData *tlvInfo);
 int SectorialDataBuildTlv (char * buffer_snd, TSTlvSectorialData *tlvInfo);

#endif
