/*
 * bll_event_getroomserverid.h
 *
 *  Created on: 2011-12-6
 *      Author: jimm
 */

#ifndef BLL_EVENT_GETROOMSERVERID_H_
#define BLL_EVENT_GETROOMSERVERID_H_

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

class CGetRoomServerIDMessageEvent : public CBllBase
{
public:
	CGetRoomServerIDMessageEvent()
	{
	}

	virtual ~CGetRoomServerIDMessageEvent()
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

#define	CREATE_GETROOMSERVERIDEVENT_INSTANCE	CSingleton<CGetRoomServerIDMessageEvent>::CreateInstance
#define	GET_GETROOMSERVERIDEVENT_INSTANCE		CSingleton<CGetRoomServerIDMessageEvent>::GetInstance
#define	DESTROY_GETROOMSERVERIDEVENT_INSTANCE	CSingleton<CGetRoomServerIDMessageEvent>::DestroyInstance

FRAME_ROOMDISPATCHER_NAMESPACE_END


#endif /* BLL_EVENT_GETROOMSERVERID_H_ */
