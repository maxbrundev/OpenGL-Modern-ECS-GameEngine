#include "utils_stdafx.h"

#include <Debug/Log.h>

using namespace Debug;

ConsoleLogger Log::console = ConsoleLogger();
FileLogger Log::logFile = FileLogger();

Log::Log(const LogLevel p_level): m_level(p_level)
{}

void Log::operator()(const std::string& p_message, const char* p_function, const char* p_file, const int p_line, const LogStream p_flags) const
{
	if (p_flags != 0)
	{
		if ((p_flags & LogStream::CONSOLELOG) != 0)
			console.LogLine(p_message, p_function, p_file, p_line, m_level);
		if((p_flags & LogStream::FILELOG) != 0)
			logFile.LogLine(p_message, p_function, p_file, p_line, m_level);			
	}	
}

Log& Log::Info()
{
	static Log logger(LogLevel::INFO);
	return logger;
}

Log& Log::Debug()
{
	static Log logger(LogLevel::DEBUG_);
	return logger;
}

Log& Log::Warning()
{
	static Log logger(LogLevel::WARNING);
	return logger;
}

Log& Log::Error()
{
	static Log logger(LogLevel::ERROR_);
	return logger;
}

Log& Log::Fatal()
{
	static Log logger(LogLevel::FATAL);
	return logger;
}