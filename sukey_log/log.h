/*! \file log.h
		\brief ��Ҫ��������ͷ�ļ�

		LOG�ĵ�
*/

#ifndef _LOG_H_
#define _LOG_H_

#include "wchar_logging.h"
#include "logging.h"

/*!
	\def LOG(severity)
	stream��LOG��
	severity ���ĸ��ȼ�:INFO WARNING ERROR FATAL
	����:LOG(INFO)<<"test"<<_T("t_test")<<a
*/
#define LOG(severity) if(FLAGS_log_open) COMPACT_SUKEY_LOG_##severity.stream()

/*!
	��ʽ��LOG���
	severity ���ĸ��ȼ�:INFO WARNING ERROR FATAL
	����:LOG_PRINTF(WARNING,"test%s test%d",a,b)
*/
#define LOG_PRINTF(severity,format,...)\
	do{\
		char buf[600]={0};\
		LogPrintf(buf,sizeof(buf)/sizeof(char)-1,format,##__VA_ARGS__);\
		LOG(severity)<<buf;\
	}while(0)\

/*!
	\def DLOG(severity)
 ������ʱ��release�²���Ҫ��ӡLOG ���Բ���Ҫ�Ѵ�������ȥ
*/

/*!
	\def DLOG_PRINTF(severity,format,...)
	��DLOGһ��
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