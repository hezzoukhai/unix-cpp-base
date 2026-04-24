#ifndef tlv_add_data_int_H
#define tlv_add_data_int_H
/*This file is generated and should not be modified manually*/




typedef struct
{
 int nPresent [ F057_NB_FIELDS + 1];
 int nPosTlv [ F057_NB_FIELDS + 1];
 int nLength;
 char sTlvData [ F057_BUF_LEN ];

}
 TSTlvAddDataInt;
 void InitTlvAddDataInt2(TSTlvAddDataInt * tlvInfo);
 int AnalyseTlvAddDataInt(char *buffer,TSTlvAddDataInt *tlvInfo);
 int GetTlvAddDataInt (char *tlv_name, TSTlvAddDataInt *tlvInfo,char *data, int *length);
 int AddTlvAddDataInt (char *tlv_name, TSTlvAddDataInt *tlvInfo, char *data, int length);
 int PutTlvAddDataInt (char *tlv_name, TSTlvAddDataInt *tlvInfo, char *data, int length);
 void DumpAddDataInt(TSTlvAddDataInt *tlvInfo);
 int AddDataIntBuildTlv (char * buffer_snd, TSTlvAddDataInt *tlvInfo);

#endif
