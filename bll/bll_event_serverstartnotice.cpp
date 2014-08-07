/*
 * bll_event_serverstartnotice.cpp
 *
 *  Created on: 2012-4-27
 *      Author: jimm
 */

#include "common/common_datetime.h"
#include "bll_event_serverstartnotice.h"
#include "def/server_errordef.h"
#include "ctl/server_datacenter.h"
#include "daemon_message_define.h"

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN


int32_t CServerStartNoticeMessageEvent::OnSystemEvent(uint16_t nEventID, void *pParam)
{
	MessageHeadSS *pMsgHead = (MessageHeadSS *)pParam;
	if(NULL == pMsgHead)
	{
		WRITE_ERROR_LOG("null pointer:{nEventID=0x%08x}\n",nEventID);
		return E_NULLPOINTER;
	}

	WRITE_NOTICE_LOG("recv Server start noti!{nServerType=%d, nServerID=%d}\n", pMsgHead->nSourceType, pMsgHead->nSourceID);

	if(enmEntityType_Room != pMsgHead->nSourceType && enmEntityType_Media != pMsgHead->nSourceType)
	{
		WRITE_WARNING_LOG("start server is{nServerType=%d}\n", pMsgHead->nSourceType);
		return S_OK;
	}

	ServerID nStartServerID = pMsgHead->nSourceID;
	EntityType nStartEntityType = pMsgHead->nSourceType;

	ServerLoadInfo stInfo;
	stInfo.serverID = nStartServerID;
	stInfo.loadValue = 0;
	stInfo.nLastUpdateTime = CDateTime::CurrentDateTime().Seconds();

//	CServerLoadManager stServerLoadManager;
//	stServerLoadManager.AddServerLoad(stInfo);

	ServerLoadManagerMap& stMap = g_DataCenter.GetServerLoadManagerMap();
	stMap[nStartEntityType].AddServerLoad(stInfo);

	if(enmEntityType_Media == pMsgHead->nSourceType)
	{
		CMedia *pMedia = NULL;

		int32_t ret = g_MediaMgt.GetMedia(pMsgHead->nSourceID, pMedia);
		if(ret < 0 || pMedia == NULL)
		{
			ret = g_MediaMgt.CreateMedia(pMsgHead->nSourceID, pMedia);
			if(ret < 0 || pMedia == NULL)
			{
				WRITE_ERROR_LOG("creat media filed {nServerID=%d}!",pMsgHead->nSourceID);
				return ret;
			}
		}
	}

	return S_OK;
}

FRAME_ROOMDISPATCHER_NAMESPACE_END

