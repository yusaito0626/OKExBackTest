#pragma once

#include "includes.h"
#include "LockFreeQueue.h"

namespace Enums
{
	enum struct logType
	{
		_NONE = -1,
		_INFO = 1,
		_WARNING = 2,
		_ERROR = 3,
		_CRITICAL = 4
	};
}

struct LogStruct
{
    Enums::logType type;
    std::string msg;
    LogStruct() :type(Enums::logType::_NONE), msg("") {};
    ~LogStruct() {};
	std::string ToString(void);
	void init();
};

class Logging
{
	Logging(int _poolSize);
	Logging(const Logging&) {};
	Logging& operator=(const Logging&) {};

	int poolSize;
	LockFreeQueue::MIMOQueue<LogStruct*>* logQueue;
	PoolingStack::PoolingStack<LogStruct*>* logPool;
public:
	~Logging();
	void addLog(Enums::logType type, std::string msg);
	Enums::logType getLog(std::string& msg);

	static Logging* getInstance(int _poolSize = 10000)
	{
		static Logging* singleton;
		if (!singleton)
		{
			singleton = new Logging(_poolSize);
		}
		return singleton;
	}
};

extern Logging* logWriter;
