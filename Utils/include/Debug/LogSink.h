#pragma once

#include "utils_stdafx.h"
#include "UtilsAPI.h"

#include <Debug/LogLevel.h>

namespace Debug
{
	class UTILS_API LogSink
	{
	public:
		virtual ~LogSink(){}

		virtual void LogLine(const std::string& p_message, const char* p_function, 
			const char* p_file, int p_line, LogLevel p_level) = 0;
	};
}