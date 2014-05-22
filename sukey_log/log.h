/*! \file log.h
		\brief 需要被包含的头文件

		LOG文档
*/

#ifndef _LOG_H_
#define _LOG_H_

#include "wchar_logging.h"
#include "logging.h"

/*!
	\def LOG(severity)
	stream型LOG宏
	severity 有四个等级:INFO WARNING ERROR FATAL
	例子:LOG(INFO)<<"test"<<_T("t_test")<<a
*/
#define LOG(severity) if(FLAGS_log_open) COMPACT_SUKEY_LOG_##severity.stream()

/*!
	格式化LOG输出
	severity 有四个等级:INFO WARNING ERROR FATAL
	例子:LOG_PRINTF(WARNING,"test%s test%d",a,b)
*/
#define LOG_PRINTF(severity,format,...)\
	do{\
		char buf[600]={0};\
		LogPrintf(buf,sizeof(buf)/sizeof(char)-1,format,##__VA_ARGS__);\
		LOG(severity)<<buf;\
	}while(0)\

/*!
	\def DLOG(severity)
 由于有时候release下不需要打印LOG 所以不需要把代码编译进去
*/

/*!
	\def DLOG_PRINTF(severity,format,...)
	和DLOG一样
*/
#ifndef NDEBUG
#define DLOG(severity) LOG(severity)
#define DLOG_PRINTF(severity,format,...) LOG_PRINTF(severity,format,##__VA_ARGS__)
#else
#define DLOG(severity)\
	(true) ? (void)0 : sukey::LogMessageVoidify() & LOG(severity)
#define DLOG_PRINTF(severity,format,...)\
	(ture) ? (void)0 : LOG_PRINTF(severity,format,##__VA_ARGS__)
#endif

#endif