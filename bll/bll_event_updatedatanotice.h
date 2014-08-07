/*
 * bll_event_updatedatanotice.h
 *
 *  Created on: 2012-4-23
 *      Author: jimm
 */

#ifndef BLL_EVENT_UPDATEDATANOTICE_H_
#define BLL_EVENT_UPDATEDATANOTICE_H_

#include "common/common_api.h"
#include "def/server_namespace.h"
#include "../../lightframe/frame_typedef.h"
#include "../../lightframe/frame_errordef.h"
#include "global.h"
#include "main_frame.h"
#include "bll_base.h"
#include "roomserver_message_define.h"


FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

class CUpdateDataNoticeMessageEvent : public CBllBase
{
public:
	CUpdateDataNoticeMessageEvent()
	{
	}

	virtual ~CUpdateDataNoticeMessageEvent()
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
	void UpdateRoomCache(MessageHeadSS * pMsgHead, CUpdateDataNoti* pUpdateDataNoti);

	void UpdatePlayerCache(MessageHeadSS * pMsgHead, CUpdateDataNoti* pUpdateDataNoti);

};

#define	CREATE_UPDATEDATANOTICEEVENT_INSTANCE	CSingleton<CUpdateDataNoticeMessageEvent>::CreateInstance
#define	GET_UPDATEDATANOTICEEVENT_INSTANCE		CSingleton<CUpdateDataNoticeMessageEvent>::GetInstance
#define	DESTROY_UPDATEDATANOTICEEVENT_INSTANCE	CSingleton<CUpdateDataNoticeMessageEvent>::DestroyInstance

FRAME_ROOMDISPATCHER_NAMESPACE_END

#endif /* BLL_EVENT_UPDATEDATANOTICE_H_ */
