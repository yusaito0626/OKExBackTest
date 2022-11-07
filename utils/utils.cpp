#include "utils.h"

void utils::setThreadName(DWORD threadId, const char* threadName)
{
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = threadName;
	info.dwThreadId = threadId;
	info.dwFlag = 0;
#pragma warning(push)
#pragma warning(disable:6320 6322)
	__try
	{
		RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{

	}
#pragma warning(pop)
}

unsigned int utils::getEpochTime(Date dt)
{
	tm  _tm;
	_tm.tm_year = dt.iday / 10000 - 1900;
	_tm.tm_mon = (dt.iday / 100 - (_tm.tm_year + 1900) * 100) - 1;
	_tm.tm_mday = dt.iday - (_tm.tm_year + 1900) * 10000 - (_tm.tm_mon + 1) * 100;
	_tm.tm_hour = 0;
	_tm.tm_min = 0;
	_tm.tm_sec = 0;
	time_t tm_t = mktime(&_tm);
	return (unsigned int)tm_t;
}

unsigned int utils::getEpochTime(int yyyy, int MM, int dd, int hh, int mm, int ss)
{
	tm  _tm;
	_tm.tm_year = yyyy - 1900;
	_tm.tm_mon = MM - 1;
	_tm.tm_mday = dd;
	_tm.tm_hour = hh;
	_tm.tm_min = mm;
	_tm.tm_sec = ss;
	time_t tm_t = mktime(&_tm);
	return (unsigned int)tm_t;
}

std::string utils::epochToStr(unsigned int time)
{
	time_t tm_t = (time_t)time;
	tm _tm;
	localtime_s(&_tm, &tm_t);
	std::string yyyy;
	std::string MM;
	std::string dd;
	std::string hh;
	std::string mm;
	std::string ss;
	std::string yyyy;
	yyyy = std::to_string(_tm.tm_year + 1900);
	if (_tm.tm_mon > 8)
	{
		MM = std::to_string(_tm.tm_mon + 1);
	}
	else
	{
		MM = "0" + std::to_string(_tm.tm_mon + 1);
	}
	if (_tm.tm_mday > 9)
	{
		dd = std::to_string(_tm.tm_mday);
	}
	else
	{
		dd = "0" + std::to_string(_tm.tm_mday);
	}
	if (_tm.tm_hour > 9)
	{
		hh = std::to_string(_tm.tm_hour);
	}
	else
	{
		hh = "0" + std::to_string(_tm.tm_hour);
	}
	if (_tm.tm_min > 9)
	{
		mm = std::to_string(_tm.tm_min);
	}
	else
	{
		mm = "0" + std::to_string(_tm.tm_min);
	}
	if (_tm.tm_sec > 9)
	{
		ss = std::to_string(_tm.tm_sec);
	}
	else
	{
		ss = "0" + std::to_string(_tm.tm_sec);
	}
	return yyyy + "-"
		+ MM + "-"
		+ dd + " "
		+ hh + ":"
		+ mm + ":"
		+ ss;
}
