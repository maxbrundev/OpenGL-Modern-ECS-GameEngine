#pragma once

#include <fstream>
#include <filesystem>

#include "UtilsAPI.h"
#include "LogSink.h"

#pragma warning(push)
#pragma warning(disable:4251)

namespace Debug
{
	namespace fs = std::experimental::filesystem;

	class UTILS_API FileLogger : public LogSink
	{
	private:
		fs::path m_logDirectory;
		std::string m_filename;
		std::ofstream m_out;
		int m_counter {0};

	public:
		FileLogger();
		FileLogger(FileLogger&& p_fileLogger) noexcept;
		~FileLogger();
		void LogLine(const std::string& p_message, const char* p_function, const char* p_file, const int p_line,
			const LogLevel p_level) override;
		void Flush();

	private:
		static std::string TimeStampPerso();
		std::string	PathToSessionLogFile();
	};
}

#pragma warning(pop)