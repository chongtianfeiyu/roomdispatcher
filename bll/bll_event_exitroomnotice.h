/*
 * bll_event_exitroomnotice.h
 *
 *  Created on: 2012-2-15
 *      Author: jimm
 */

#ifndef BLL_EVENT_EXITROOMNOTICE_H_
#define BLL_EVENT_EXITROOMNOTICE_H_

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

class CExitRoomNoticeMessageEvent : public CBllBase
{
public:
	CExitRoomNoticeMessageEvent()
	{
	}

	virtual ~CExitRoomNoticeMessageEvent()
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

#define	CREATE_EXITROOMNOTICEEVENT_INSTANCE	CSingleton<CExitRoomNoticeMessageEvent>::CreateInstance
#define	GET_EXITROOMNOTICEEVENT_INSTANCE		CSingleton<CExitRoomNoticeMessageEvent>::GetInstance
#define	DESTROY_EXITROOMNOTICEEVENT_INSTANCE	CSingleton<CExitRoomNoticeMessageEvent>::DestroyInstance

FRAME_ROOMDISPATCHER_NAMESPACE_END

#endif /* BLL_EVENT_EXITROOMNOTICE_H_ */
