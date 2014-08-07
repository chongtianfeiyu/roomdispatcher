/*
 * bll_event_createroomnotice.h
 *
 *  Created on: 2012-2-12
 *      Author: jimm
 */

#ifndef BLL_EVENT_CREATEROOMNOTICE_H_
#define BLL_EVENT_CREATEROOMNOTICE_H_

#include <stdlib.h>
#include <time.h>
#include "common/common_api.h"
#include "def/server_namespace.h"
#include "../../lightframe/frame_typedef.h"
#include "../../lightframe/frame_errordef.h"
#include "global.h"
#include "main_frame.h"
#include "bll_base.h"


FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

class CCreateRoomNoticeMessageEvent : public CBllBase
{
public:
	CCreateRoomNoticeMessageEvent()
	{
	}

	virtual ~CCreateRoomNoticeMessageEvent()
	{
	}

	//消息事件
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

	//会话事件激活
	virtual int32_t OnSessionEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			CFrameSession* pSession, const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

	int32_t OnSessionTimeOut(CFrameSession *pSession);

private:

};

#define	CREATE_CREATEROOMNOTICEEVENT_INSTANCE	CSingleton<CCreateRoomNoticeMessageEvent>::CreateInstance
#define	GET_CREATEROOMNOTICEEVENT_INSTANCE		CSingleton<CCreateRoomNoticeMessageEvent>::GetInstance
#define	DESTROY_CREATEROOMNOTICEEVENT_INSTANCE	CSingleton<CCreateRoomNoticeMessageEvent>::DestroyInstance

FRAME_ROOMDISPATCHER_NAMESPACE_END

#endif /* BLL_EVENT_CREATEROOMNOTICE_H_ */
