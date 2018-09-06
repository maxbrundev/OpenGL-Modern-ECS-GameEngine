#pragma once

#include "utils_stdafx.h"
#include "UtilsAPI.h"

#define LOG(Logger_, Message_, ...)						\
	Debug::Log::Logger_(								\
		static_cast<std::ostringstream&>(				\
			std::ostringstream().flush() << Message_	\
			).str(),									\
		__FUNCTION__,									\
		__FILE__,										\
		__LINE__, 										\
		__VA_ARGS__										\
	);

#include <Debug/ConsoleLogger.h>
#include "LogLevel.h"
#include "FileLogger.h"

namespace Debug
{
	enum class LogStream : uint8_t
	{
		CONSOLELOG	 = (1 << 0),
		FILELOG		 = (1 << 2)
	};

	class UTILS_API Log
	{
	private:
		LogLevel m_level;

		static ConsoleLogger console;
		static FileLogger logFile;

	public:
		explicit Log(const LogLevel p_level);

		void operator()(const std::string& p_message, const char* p_function, 
			const char* p_file, const int p_line, LogStream p_flags = LogStream::CONSOLELOG) const;
		
		static Log& Info();
		static Log& Debug();
		static Log& Warning();
		static Log& Error();
		static Log& Fatal();
	};

	inline LogStream operator|(LogStream lhs, LogStream rhs)
	{
		return static_cast<LogStream>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
	}

	inline LogStream operator&(LogStream lhs, LogStream rhs)
	{
		return static_cast<LogStream>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
	}

	inline  bool operator!=(LogStream lhs, const uint8_t rhs)
	{
		return static_cast<uint8_t>(lhs) != rhs;
	}
}