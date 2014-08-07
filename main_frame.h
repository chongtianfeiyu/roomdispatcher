/*
 * main_frame.h
 *
 *  Created on: 2011-12-2
 *      Author: jimm
 */

#ifndef MAIN_FRAME_H_
#define MAIN_FRAME_H_

#include "common/common_errordef.h"
#include "common/common_singleton.h"
#include "def/server_namespace.h"
#include "frame_logengine.h"
#include "lightframe.h"

FRAME_ROOMDISPATCHER_NAMESPACE_BEGIN

#define  StaticLogIndex  1

class CMainFrame : public CLightFrame
{
public:
	CMainFrame();
	virtual ~CMainFrame();

public:
	//框架初始话
	int32_t Initialize();
	//恢复框架
	int32_t Resume();
	//销毁框架
	int32_t Terminate();
//	void _WriteLog(char * szName,LogLevel logLevel,const char *szFunc, int32_t nLineNO,const char* szFormat, ...)
//	{
//		char szLog[enmMaxLogInfoLength];
//
//		//生成日志信息
//		va_list ap;
//		va_start(ap, szFormat);
//		vsprintf(szLog, szFormat, ap);
//		va_end(ap);
//		WRITE_THREAD_LOG(StaticLogIndex%enmMaxQueueIndex, logLevel, szName, szFunc, nLineNO, szLog); //进程ID可能不是0-7，取余得到
//	}
protected:
	void RegistMsg();
};

#define	CREATE_MAINFRAME_INSTANCE	CSingleton<CMainFrame>::CreateInstance
#define	GET_MAINFRAME_INSTANCE		CSingleton<CMainFrame>::GetInstance
#define	DESTROY_MAINFRAME_INSTANCE	CSingleton<CMainFrame>::DestroyInstance

#define g_Frame	GET_MAINFRAME_INSTANCE()


#define WRITE_LOG(level, fmt, ...) \
	do{ g_Frame.WriteLog(level, __FILE__,  __LINE__,""fmt"\n", ##__VA_ARGS__); } while (false)

#define WRITE_DEBUG_LOG(fmt, ...)		WRITE_LOG(enmLogLevel_Debug, fmt, ##__VA_ARGS__)
#define WRITE_INFO_LOG(fmt, ...)	WRITE_LOG(enmLogLevel_Info, fmt, ##__VA_ARGS__)
#define WRITE_NOTICE_LOG(fmt, ...)	WRITE_LOG(enmLogLevel_Notice, fmt, ##__VA_ARGS__)
#define WRITE_WARNING_LOG(fmt, ...)	WRITE_LOG(enmLogLevel_Warning, fmt, ##__VA_ARGS__)
#define WRITE_ERROR_LOG(fmt, ...)	WRITE_LOG(enmLogLevel_Error, fmt, ##__VA_ARGS__)

#define _WRITE_LOG(level, fmt, ...)\
		do{ g_Frame.WriteUserLog("server_distributed",level, __FILE__, __LINE__,""fmt"\n", ##__VA_ARGS__); } while (false)

#define WRITE_DISTRIBUTED_DEBUG_LOG(fmt, ...)	_WRITE_LOG(enmLogLevel_Debug, fmt, ##__VA_ARGS__)
#define WRITE_DISTRIBUTED_INFO_LOG(fmt, ...)	_WRITE_LOG(enmLogLevel_Info, fmt, ##__VA_ARGS__)
#define WRITE_DISTRIBUTED_NOTICE_LOG(fmt, ...)	_WRITE_LOG(enmLogLevel_Notice, fmt, ##__VA_ARGS__)
#define WRITE_DISTRIBUTED_WARNING_LOG(fmt, ...)	_WRITE_LOG(enmLogLevel_Warning, fmt, ##__VA_ARGS__)
#define WRITE_DISTRIBUTED_ERROR_LOG(fmt, ...)	_WRITE_LOG(enmLogLevel_Error, fmt, ##__VA_ARGS__)

FRAME_ROOMDISPATCHER_NAMESPACE_END


#endif /* MAIN_FRAME_H_ */
