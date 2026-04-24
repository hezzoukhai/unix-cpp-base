#include <p7_common.h>
#include <iso8583/msg_layout.h>

/******************************************************************************/
void InitMsgLayout(TSMsgLayout* pkMsgLayout)
{
	TSFieldPresence		kFieldPrensece;
	int					i;
	int					nSize;

	pkMsgLayout->nProtocolId	= 0;
	pkMsgLayout->id			= 0;

	kFieldPrensece.nFieldNo=0;
	kFieldPrensece.presence_info=0;
	kFieldPrensece.nConditionNo=0;

	nSize = sizeof(pkMsgLayout->tab_msgFields)/sizeof(pkMsgLayout->tab_msgFields[0]);
	for(i=0; i < nSize; i++ )
	{
		pkMsgLayout->tab_msgFields[i]=kFieldPrensece;
	}

}
/******************************************************************************/
int	GetFieldLayout(msg_layout_id_t msgLayoutId, int nFieldNo, int nStartSearchPos, int* nFieldIndex, const TSFieldPresence** pkFieldPresence)
{
	pkFieldPresence = &gs_tab_MsgLayouts[msgLayoutId].tab_msgFields[nFieldNo];
	(*nFieldIndex) = nFieldNo;
	return OK;
}
/******************************************************************************/
const TSFieldPresence*	GetFieldLayoutByIndex(msg_layout_id_t msgLayoutId, int nFieldIndex)
{
	return &gs_tab_MsgLayouts[msgLayoutId].tab_msgFields[nFieldIndex];
}
/******************************************************************************/


