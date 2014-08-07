/*
 * bll_event_updateinfonotice.h
 *
 *  Created on: 2012-1-10
 *      Author: jimm
 */

#ifndef BLL_EVENT_UPDATEINFONOTICE_H_
#define BLL_EVENT_UPDATEINFONOTICE_H_

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
#include "roomserver_message_define.h"
#include "hallserver_message_define.h"


FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

class CUpdateInfoToHallMessageEvent : public CBllBase
{
public:
	CUpdateInfoToHallMessageEvent()
	{
	}

	virtual ~CUpdateInfoToHallMessageEvent()
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
	int32_t OnRoomUpdateInfo(MessageHeadSS *pMsgHead, CUpdateInfoToHallNotice *pUpdateInfo);

	int32_t OnMediaUpdateInfo(MessageHeadSS *pMsgHead, CUpdateInfoToHallNotice *pUpdateInfo);

};

#define	CREATE_UPDATEINFOTOHALLEVENT_INSTANCE	CSingleton<CUpdateInfoToHallMessageEvent>::CreateInstance
#define	GET_UPDATEINFOTOHALLEVENT_INSTANCE		CSingleton<CUpdateInfoToHallMessageEvent>::GetInstance
#define	DESTROY_UPDATEINFOTOHALLEVENT_INSTANCE	CSingleton<CUpdateInfoToHallMessageEvent>::DestroyInstance

FRAME_ROOMDISPATCHER_NAMESPACE_END

#endif /* BLL_EVENT_UPDATEINFONOTICE_H_ */
