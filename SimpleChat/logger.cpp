#include "logger.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <fstream>
#include <regex>

namespace dsc {
	bool logger::log(std::string msg, sender who, severity level) {
		msg = logger::formatMessage(msg, who, level);
		bool result = logger::sendMessage(msg, level);
		logger::addTrace(msg, level);
		return result;
	}
	bool logger::msg(std::string msg, sender who) {
		msg = logger::formatMessage(msg, who, VERBOSE);
		bool result = logger::sendMessage(msg, VERBOSE);
		logger::addTrace(msg, VERBOSE);
		return result;
	}
	bool logger::info(std::string msg, sender who) {
		msg = logger::formatMessage(msg, who, INFO);
		bool result = logger::sendMessage(msg, INFO);
		logger::addTrace(msg, INFO);
		return result;
	}
	bool logger::debug(std::string msg, sender who) {
		msg = logger::formatMessage(msg, who, DEBUG);
		bool result = logger::sendMessage(msg, DEBUG);
		logger::addTrace(msg, DEBUG);
		return result;
	}
	bool logger::warning(std::string msg, sender who) {
		msg = logger::formatMessage(msg, who, WARNING);
		bool result = logger::sendMessage(msg, WARNING);
		logger::addTrace(msg, WARNING);
		return result;
	}
	bool logger::error(std::string msg, sender who) {
		msg = logger::formatMessage(msg, who, ERROR);
		bool result = logger::sendMessage(msg, ERROR);
		logger::addTrace(msg, ERROR);
		return result;
	}
	bool logger::fatal(std::string msg, sender who) {
		msg = logger::formatMessage(msg, who, FATAL);
		bool result = logger::sendMessage(msg, FATAL);
		logger::addTrace(msg, FATAL);
		logger::printTrace();
		return result;
	}

	bool logger::user_l(std::string msg, sender who) {
		return logger::log(msg, who, USER_LOW);
	}
	bool logger::user_m(std::string msg, sender who) {
		return logger::log(msg, who, USER_MEDIUM);
	}
	bool logger::user_h(std::string msg, sender who) {
		return logger::log(msg, who, USER_HIGH);

	}

	void logger::setLogLevel(severity level) {
		logger::logLevel = level;
		logger::info("Log level set to " + logger::getSeverityName(level), logger::SERVER);
		return;
	}
	logger::severity logger::getLogLevel() {
		return logger::logLevel;
	}

	void logger::setTraceLevel(severity level) {
		logger::traceLevel = level;
		logger::info("Trace level set to " + logger::getSeverityName(level), logger::SERVER);
		return;
	
	}
	logger::severity logger::getTraceLevel() {
		return logger::traceLevel;
	}

	bool logger::sendMessage(std::string msg, severity level) {
		if (level < logger::logLevel) return false;

		std::cout << msg;
		return true;
	}
	bool logger::addTrace(std::string trace, severity level) {
		if (logger::recordTrace && level < logger::traceLevel) 
			return false;

		logger::stackTrace.push_back(trace);
		return true;
	}
	bool logger::printTrace() {
		if (logger::printTrace) {
			std::cout << "\n\n";
			std::cout << "\033[0;31m" << "Showing the trace up to the program termination\n";
			std::cout << "\033[21;0m";
			for (std::string trace : logger::stackTrace) {
				std::cout << trace;
			}
		}
		std::ofstream file;
		file.open("trace.log", std::ios::out | std::ios::trunc);
		if (file.is_open()) {
			file << "Showing the trace up to the program termination\n\n";

			for (std::string trace : logger::stackTrace) {
				std::regex colorRegex("\033\\[[0-9;]*m");
				trace = std::regex_replace(trace, colorRegex, "");
				file << trace;
			}
			file.close();
			return true;
		}
		else {
			std::cout << "Failed to open trace file\n";
			return false;
		}
	}

	void logger::setRecordTrace(bool enable) {
		logger::recordTrace = enable;
		return;
	}

	std::string logger::getTimestamp() {
		std::time_t now = std::time(nullptr);
		char* time = new char[26];
		tm *timeInfo = new tm();
		localtime_s(timeInfo, &now);
		asctime_s(time, 26, timeInfo);
		time[24] = '\0';
		return time;
	}
	std::string logger::getSenderName(sender who) {
		return logger::senderNames[who];
	}
	std::string logger::getSeverityName(severity level) {
		return logger::severityNames[level];
	}
	std::string logger::getSeverityColor(severity level) {
		return "\033[1;" + logger::severityColors[level] + "m";
	}
	std::string logger::getTimeStampColor() {
		return logger::timeStampColor;
	}
	std::string logger::getMessageColor() {
		return logger::messageColor;
	}
	std::string logger::resetColor() {
		return logger::colorReset;
	}

	std::string logger::formatMessage(std::string msg, sender who, severity level) {
		std::string time = getTimestamp();
		std::string sender = getSenderName(who);
		std::string lvl = getSeverityName(level);

		return
			getTimeStampColor() + time + resetColor() + " " +
			getSeverityColor(level) + "[" + lvl + " / " + sender + "]: " + resetColor() +
			getMessageColor() + msg + resetColor() + "\n";
	}

#ifdef _DEBUG
	logger::severity logger::logLevel = DEBUG;
	bool logger::recordTrace = true;
	logger::severity logger::traceLevel = VERBOSE;
#else
	#ifdef NDEBUG
	logger::severity logger::logLevel = WARNING;
	bool logger::recordTrace = true;
	logger::severity logger::traceLevel = WARNING;
	#else
	logger::severity logger::logLevel = INFO;
	bool logger::recordTrace = true;
	logger::severity logger::traceLevel = WARNING;
	#endif // NDEBUG
#endif // DEBUG

	std::vector<std::string> logger::stackTrace = {};

	const std::vector<std::string> logger::senderNames = { "CLIENT", "SERVER" };
	const std::vector<std::string> logger::severityNames = { "MSG", "DEBUG", "INFO", "WARNING", "INFO", "ERROR", "INFO", "FATAL"};
	const std::vector<std::string> logger::severityColors = { "90", "94", "36", "33", "36", "31", "36", "31"};
	const std::string logger::colorReset = "\033[21;0m";
	const std::string logger::timeStampColor = "\033[4;35m";
	const std::string logger::messageColor = "\033[3;97m";

	logger::builder logger::buildMessage() {
		return logger::builder();
	}


	logger::builder logger::builder::add(std::string message) {
		this->message += message;
		return *this;
	}
	logger::builder logger::builder::setSender(logger::sender who) {
		this->who = who;
		return *this;
	}
	logger::builder logger::builder::setSeverity(logger::severity level) {
		this->level = level;
		return *this;
	}
	bool logger::builder::log() {
		return logger::log(this->message, this->who, this->level);
	}
}