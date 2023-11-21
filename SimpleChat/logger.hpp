#pragma once

#include <string>
#include <vector>

namespace dsc {
	class logger {
		class builder;
	public:
		enum sender {
			CLIENT,
			SERVER
		};
		enum severity {
			VERBOSE		= 0,
			DEBUG		= 1,
			INFO		= 2,
			USER_LOW	= 2,
			WARNING		= 3,
			USER_MEDIUM = 4,
			ERROR		= 5,
			USER_HIGH	= 6,
			FATAL		= 7
		};

		static builder buildMessage();

		static bool log(std::string, sender, severity);
		static bool msg(std::string, sender);
		static bool info(std::string, sender);
		static bool debug(std::string, sender);
		static bool warning(std::string, sender);
		static bool error(std::string, sender);
		static bool fatal(std::string, sender);

		static bool user_l(std::string, sender);
		static bool user_m(std::string, sender);
		static bool user_h(std::string, sender);

		static void setLogLevel(severity);
		static severity getLogLevel();

		static void setTraceLevel(severity);
		static severity getTraceLevel();

		static void setRecordTrace(bool);

	private:
		static bool sendMessage(std::string, severity);
		static bool addTrace(std::string, severity);
		static bool printTrace();

		static std::string getTimestamp();
		static std::string getSenderName(sender);
		static std::string getSeverityName(severity);
		static std::string getSeverityColor(severity);
		static std::string getTimeStampColor();
		static std::string getMessageColor();
		static std::string resetColor();

		static std::string formatMessage(std::string, sender, severity);

	private:
		static severity logLevel;
		static bool recordTrace;
		static severity traceLevel;
		static std::vector<std::string> stackTrace;
		
		static const std::vector<std::string> senderNames;
		static const std::vector<std::string> severityNames;
		static const std::vector<std::string> severityColors;
		static const std::string colorReset;
		static const std::string timeStampColor;
		static const std::string messageColor;

		// make a static constexpr array of pointers to the functions
		static constexpr bool (*severityFunctions[])(std::string, sender) = {
			debug, error, warning, info, fatal
		};

	private:
		class builder {
			std::string message = "";
			sender who = logger::SERVER;
			severity level = logger::INFO;
		public:
			builder add(std::string);
			builder setSender(sender);
			builder setSeverity(severity);
			bool log();
			std::string getMessage();
			std::string getFormatedMessage();
		};
	};
}

