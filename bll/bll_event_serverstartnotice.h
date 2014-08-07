/*
 * bll_event_serverstartnotice.h
 *
 *  Created on: 2012-4-27
 *      Author: jimm
 */

#ifndef BLL_EVENT_SERVERSTARTNOTICE_H_
#define BLL_EVENT_SERVERSTARTNOTICE_H_

#include "def/server_namespace.h"
#include "../../lightframe/frame_typedef.h"
#include "../../lightframe/frame_errordef.h"
#include "global.h"
#include "main_frame.h"
#include "bll_base.h"
#include "roomdispatcher_message_define.h"

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

class CServerStartNoticeMessageEvent : public CBllBase, public ISystemEvent
{
public:
	CServerStartNoticeMessageEvent()
	{
	}

	virtual ~CServerStartNoticeMessageEvent()
	{
	}

	virtual int32_t OnSystemEvent(uint16_t nEventID, void *pParam);

};

#define	CREATE_SERVERSTARTNOTICEEVENT_INSTANCE	CSingleton<CServerStartNoticeMessageEvent>::CreateInstance
#define	GET_SERVERSTARTNOTICEEVENT_INSTANCE		CSingleton<CServerStartNoticeMessageEvent>::GetInstance
#define	DESTROY_SERVERSTARTNOTICEEVENT_INSTANCE	CSingleton<CServerStartNoticeMessageEvent>::DestroyInstance

FRAME_ROOMDISPATCHER_NAMESPACE_END

#endif /* BLL_EVENT_SERVERSTARTNOTICE_H_ */
