/*
 *  bll_event_rebulidok_notice.h
 *
 *  To do：
 *  Created on: 	2012-5-16
 *  Author: 		luocj
 */

#ifndef BLL_EVENT_REBULIDOK_NOTICE_H_
#define BLL_EVENT_REBULIDOK_NOTICE_H_

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

class CRebulidOKNoticeMessageEvent : public CBllBase
{
public:
	CRebulidOKNoticeMessageEvent()
	{
	}

	virtual ~CRebulidOKNoticeMessageEvent()
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

#define	CREATE_REBULIDOKNOTICEEVENT_INSTANCE		CSingleton<CRebulidOKNoticeMessageEvent>::CreateInstance
#define	GET_REBULIDOKNOTICEEVENT_INSTANCE			CSingleton<CRebulidOKNoticeMessageEvent>::GetInstance
#define	DESTROY_REBULIDOKNOTICEEVENT_INSTANCE		CSingleton<CRebulidOKNoticeMessageEvent>::DestroyInstance

FRAME_ROOMDISPATCHER_NAMESPACE_END

#endif /* BLL_EVENT_REBULIDOK_NOTICE_H_ */
