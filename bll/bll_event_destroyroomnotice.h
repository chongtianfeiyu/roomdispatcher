/*
 * bll_event_destroyroomnotice.h
 *
 *  Created on: 2012-4-24
 *      Author: jimm
 */

#ifndef BLL_EVENT_DESTROYROOMNOTICE_H_
#define BLL_EVENT_DESTROYROOMNOTICE_H_

#include <stdlib.h>
#include <time.h>
#include "common/common_api.h"
#include "def/server_namespace.h"
#include "../../lightframe/frame_typedef.h"
#include "../../lightframe/frame_errordef.h"
#include "global.h"
#include "main_frame.h"
#include "bll_base.h"
#include "dal/server_message.h"

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

class CDestroyRoomNoticeMessageEvent : public CBllBase
{
public:
	CDestroyRoomNoticeMessageEvent()
	{
	}

	virtual ~CDestroyRoomNoticeMessageEvent()
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

#define	CREATE_DESTROYROOMNOTICEEVENT_INSTANCE	CSingleton<CDestroyRoomNoticeMessageEvent>::CreateInstance
#define	GET_DESTROYROOMNOTICEEVENT_INSTANCE		CSingleton<CDestroyRoomNoticeMessageEvent>::GetInstance
#define	DESTROY_DESTROYROOMNOTICEEVENT_INSTANCE	CSingleton<CDestroyRoomNoticeMessageEvent>::DestroyInstance

FRAME_ROOMDISPATCHER_NAMESPACE_END

#endif /* BLL_EVENT_DESTROYROOMNOTICE_H_ */
