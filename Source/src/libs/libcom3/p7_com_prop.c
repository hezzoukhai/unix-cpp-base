#include <p7_common.h>
#include <p7_types.h>
#include <p7_com_prop.h>
/******************************************************************************/
void InitComInfoProperties(TSComInfoProperties* pkComInfoProperties)
{
	pkComInfoProperties->eConnMode = E_CM_CLIENT;
	pkComInfoProperties->eConnType = E_CONN_TCP;
	pkComInfoProperties->ePacketLengthType = CLTP_NO_LENGTH;
	pkComInfoProperties->nLengthLen = 0;
	pkComInfoProperties->bExcludeLength = NOK;
	pkComInfoProperties->nMaxClients = 0;
	pkComInfoProperties->nDivFactor = 1;
	pkComInfoProperties->bMultiConnPerClient = NOK;
	pkComInfoProperties->nLengthOffset=0;
	pkComInfoProperties->nTotalLengthLen=0;
	memset(pkComInfoProperties->szPropertiesName,'\0',sizeof(pkComInfoProperties->szPropertiesName));
}
/******************************************************************************/



