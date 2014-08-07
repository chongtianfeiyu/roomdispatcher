/*
 * bll_event_getmediainfo.h
 *
 *  Created on: 2012-1-11
 *      Author: jimm
 */

#include <stdlib.h>
#include <time.h>
#include "common/common_api.h"
#include "def/server_namespace.h"
#include "dal/server_message.h"
#include "../../lightframe/frame_typedef.h"
#include "../../lightframe/frame_errordef.h"
#include "global.h"
#include "main_frame.h"
#include "bll_base.h"


FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

class CGetMediaInfoMessageEvent : public CBllBase
{
public:
	CGetMediaInfoMessageEvent()
	{
	}

	virtual ~CGetMediaInfoMessageEvent()
	{
	}

	//消息事件
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

	//会话事件激活
	virtual int32_t OnSessionEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			CFrameSession* pSession, const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

	int32_t OnSessionTimeOut(CFrameSession *pSession);

};

#define	CREATE_GETMEDIAINFOEVENT_INSTANCE	CSingleton<CGetMediaInfoMessageEvent>::CreateInstance
#define	GET_GETMEDIAINFOEVENT_INSTANCE		CSingleton<CGetMediaInfoMessageEvent>::GetInstance
#define	DESTROY_GETMEDIAINFOEVENT_INSTANCE	CSingleton<CGetMediaInfoMessageEvent>::DestroyInstance

FRAME_ROOMDISPATCHER_NAMESPACE_END

