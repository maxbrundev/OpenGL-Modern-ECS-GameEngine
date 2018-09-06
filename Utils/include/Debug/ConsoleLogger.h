#pragma once

#include "UtilsAPI.h"

#include <Debug/LogSink.h>

namespace Debug
{
	class UTILS_API ConsoleLogger : public LogSink
	{
	public:
		ConsoleLogger();
		~ConsoleLogger();
		void LogLine(const std::string& p_message, const char* p_function, 
			const char* p_file, const int p_line, const LogLevel p_level) override;
	};
}