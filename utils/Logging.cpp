#include "Logging.h"

Logging* logWriter = Logging::getInstance(10000);

std::string LogStruct::ToString(void)
{
	std::string header;
	switch (type)
	{
	case Enums::logType::_INFO:
		header = "[INFO] ";
		break;
	case Enums::logType::_WARNING:
		header = "[WARNING] ";
		break;
	case Enums::logType::_ERROR:
		header = "[ERROR] ";
		break;
	case Enums::logType::_CRITICAL:
		header = "[CRITICAL] ";
		break;
	case Enums::logType::_NONE:
	default:
		header = "";
		break;
	}
	return header + msg;
}
void LogStruct::init()
{
	type = Enums::logType::_NONE;
	msg = "";
}

Logging::Logging(int _poolSize) 
{
	logQueue = new LockFreeQueue::MIMOQueue<LogStruct*>();
	logPool = new PoolingStack::PoolingStack<LogStruct*>();
	poolSize = _poolSize;
	for (int i = 0; i < poolSize; ++i)
	{
		LogStruct* log = new LogStruct();
		logPool->push(log);
	}
}
Logging::~Logging()
{

}
void Logging::addLog(Enums::logType type, std::string msg)
{
	LogStruct* log = logPool->pop();
	if (!log)
	{
		int newPoolSize = poolSize * 1.5;
		int addSize = newPoolSize - poolSize;
		for (int i = 0; i < addSize; ++i)
		{
			LogStruct* newObj = new LogStruct();
			logPool->push(newObj);
		}
		poolSize = newPoolSize;
		log = logPool->pop();
	}
	log->type = type;
	log->msg = msg;
	logQueue->Enqueue(log);
}

Enums::logType Logging::getLog(std::string& msg)
{
	LogStruct* log = logQueue->Dequeue();
	Enums::logType type = Enums::logType::_NONE;
	if (log)
	{
		type = log->type;
		msg = log->ToString();
		log->init();
		logPool->push(log);
		return type;
	}
	else
	{
		msg = "";
		return type;
	}
}
