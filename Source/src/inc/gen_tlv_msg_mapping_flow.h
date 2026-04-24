#ifndef tlv_msg_mapping_flow_H
#define tlv_msg_mapping_flow_H
/*This file is generated and should not be modified manually*/




typedef struct 
{
 int nPresent [ 31 + 1];
 int nPosTlv [ 31 + 1];
 int nLength;
 char sTlvData [ 1024 ];
 
}
 TSTlvMsgMappingFlow;
 void InitTlvMsgMappingFlow(TSTlvMsgMappingFlow * tlvInfo);
 int AnalyseTlvMsgMappingFlow(char *buffer, int nLength, TSTlvMsgMappingFlow *tlvInfo);
 int GetTlvMsgMappingFlow (char *tlv_name, TSTlvMsgMappingFlow *tlvInfo,char *data, int *length);
 int AddTlvMsgMappingFlow (char *tlv_name, TSTlvMsgMappingFlow *tlvInfo, char *data, int length);
 int PutTlvMsgMappingFlow (char *tlv_name, TSTlvMsgMappingFlow *tlvInfo, char *data, int length);
 int GetTlvMsgMappingFlowType ( int nIndice );
 int GetTlvMsgMappingFlowLength ( int nIndice );
 const char* GetTlvMsgMappingFlowName ( int nIndice );
 void DumpMsgMappingFlow(TSTlvMsgMappingFlow *tlvInfo);
 int MsgMappingFlowBuildTlv (char * buffer_snd, TSTlvMsgMappingFlow *tlvInfo);

#endif
