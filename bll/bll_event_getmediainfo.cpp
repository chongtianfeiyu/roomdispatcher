/*
 * bll_event_getmediainfo.cpp
 *
 *  Created on: 2012-1-11
 *      Author: jimm
 */

#include "bll_event_getmediainfo.h"
#include "def/server_errordef.h"
#include "ctl/server_datacenter.h"
#include "common/common_datetime.h"
#include "hallserver_message_define.h"
#include "dal/dal_media.h"
#include <arpa/inet.h>

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

int32_t CGetMediaInfoMessageEvent::OnSessionEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		CFrameSession* pSession, const uint16_t nOptionLen, const void *pOptionData)
{
	return S_OK;
}
int32_t CGetMediaInfoMessageEvent::OnSessionTimeOut(CFrameSession *pSession)
{
	return S_OK;
}
int32_t CGetMediaInfoMessageEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	if(pMsgBody==NULL || pMsgHead==NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgBody=0x%08x, pMsgHead=0x%08x}\n",pMsgBody,pMsgHead);
		return E_NULLPOINTER;
	}

	CGetMediaInfoResp stResp;

	CGetMediaInfoReq *pGetMediaInfoReq = dynamic_cast<CGetMediaInfoReq *>(pMsgBody);
	if(NULL == pGetMediaInfoReq)
	{
		WRITE_ERROR_LOG("null pointer:{pGetMediaInfoReq=0x%08x, nRoomID=%d}\n",pGetMediaInfoReq,pMsgHead->nRoomID);
		stResp.nResult = E_NULLPOINTER;
		SendMessageResponse(MSGID_HSRS_GETMEIDAINFO_RESP, pMsgHead, &stResp, enmTransType_P2P, nOptionLen, (char *)pOptionData);

		return E_NULLPOINTER;
	}
	WRITE_DEBUG_LOG("oh,shit!recv roomserver get mediainfo request{nRoleID=%d,nRoomID=%d}\n",pMsgHead->nRoleID, pMsgHead->nRoomID);

	//获取最空闲的mediaserver
	CMedia *arrMediaServer[enmNetType_Count] = {NULL};
	int32_t ret = g_DataCenter.GetIdleMediaServer(arrMediaServer, enmNetType_Count);
	if(ret < 0)
	{
		WRITE_ERROR_LOG("get idle media server failed{ret=0x%08x}\n",ret);
		stResp.nResult = ret;
	}
	else
	{
		char strTelIP[16]={'\0'};
		char strCncIP[16]={'\0'};
		stResp.nResult = S_OK;
		if(arrMediaServer[enmNetType_Tel] != NULL && arrMediaServer[enmNetType_Cnc] != NULL)
		{
			//电信
			stResp.nTelMediaIP = arrMediaServer[enmNetType_Tel]->GetServerAddress();
			stResp.nTelMediaPort = arrMediaServer[enmNetType_Tel]->GetServerPort();
			stResp.nTelServerID = arrMediaServer[enmNetType_Tel]->GetServerID();
			//网通
			stResp.nCncMediaIP = arrMediaServer[enmNetType_Cnc]->GetServerAddress();
			stResp.nCncMediaPort = arrMediaServer[enmNetType_Cnc]->GetServerPort();
			stResp.nCncServerID = arrMediaServer[enmNetType_Cnc]->GetServerID();
		}
		else
		{
			//电信
			stResp.nTelMediaIP = 0;
			stResp.nTelMediaPort = 0;
			stResp.nTelServerID = enmInvalidServerID;
			//网通
			stResp.nCncMediaIP = 0;
			stResp.nCncMediaPort = 0;
			stResp.nCncServerID = enmInvalidServerID;
		}
		sprintf(strTelIP,"%s",inet_ntoa_f(stResp.nTelMediaIP));
		sprintf(strCncIP,"%s",inet_ntoa_f(stResp.nCncMediaIP));
		WRITE_DISTRIBUTED_NOTICE_LOG("Room:%d  MediaServerID_Tel:%s:%d:%d MediaServerID_Cnc:%s:%d:%d",
				pMsgHead->nRoomID,
				strTelIP,stResp.nTelMediaPort,
				stResp.nTelServerID,strCncIP,
				stResp.nCncMediaPort,stResp.nCncServerID);
	}
	SendMessageResponse(MSGID_HSRS_GETMEIDAINFO_RESP, pMsgHead, &stResp, enmTransType_P2P, nOptionLen, (char *)pOptionData);
	return S_OK;
}

FRAME_ROOMDISPATCHER_NAMESPACE_END

