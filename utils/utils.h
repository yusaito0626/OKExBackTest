#pragma once

#include "includes.h"
#include "calendar.h"

namespace utils
{
	const DWORD MS_VC_EXCEPTION = 0x406D1388;
#pragma pack(push,8)
	typedef struct tagTHREADNAME_INFO
	{
		DWORD dwType;
		LPCSTR szName;
		DWORD dwThreadId;
		DWORD dwFlag;
	} THREADNAME_INFO;
#pragma pack(pop)
	void setThreadName(DWORD threadId, const char* threadName);
	
	unsigned int getEpochTime(Date dt);

	unsigned int getEpochTime(int yyyy, int MM, int dd, int hh, int mm, int ss);

	std::string epochToStr(unsigned int time);
};

