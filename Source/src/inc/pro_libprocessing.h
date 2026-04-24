/* #exception_processing.c */
/*
int IssuerTimeOutProcessing (   int nIndiceCtx, char *sMyResource, char *sTlvBuffer, TSIsoInfo *sIsoMsg);
int SystemMalfunctionProcessing (   int nIndiceCtx, char *sMyResource, char *sTlvBuffer , TSIsoInfo *sIsoMsg);
int AuthorProcessing(   int nIndiceCtx, char *sMyResource, char *sTlvBuffer , TSIsoInfo *sIsoMsg , char *sReasonCode);
*/
/* #check_security */
/*
void FillSecurityResult ( char result , char *sSecurityResults , int position );
void PutHsmRjectEvent ( TSTlvAutho *sTlvInfo , char result , int position );
int HsmDecline ( TSTlvAutho *sTlvInfo , char *sSecurityResults , char *sHsmResultCode , char position );
int CheckSecurity ( int nIndiceCtx, char *sTlvBuffer  );
*/
