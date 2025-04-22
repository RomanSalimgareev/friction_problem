#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>


#ifdef NDEBUG
	#define ASSERT(cond, msg) ((void)0)
#else
	#define ASSERT(cond, msg) assertError(cond, msg, __FILE__, __LINE__)
#endif

#define ERROR(msg) error(msg, __FILE__, __LINE__)
 
#define WARNING(msg) warning(msg ,__FILE__, __LINE__)


enum class LogLevel {WARNING, ERROR};

void assertError(bool condition, const std::string& msg, const char* file);

void log(LogLevel level, const std::string& message);

void error(std::string msg, const char* file, const long& line);

void warning(std::string msg, const char* file, const long& line);

std::string messageOutOfRange();

std::string messageDivideZero();

#endif

