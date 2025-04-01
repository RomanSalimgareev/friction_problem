#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

#define NDEBUG

#ifdef NDEBUG
	#define ASSERT(cond, msg) ((void)0)
#else
	#define ASSERT(cond, msg) assertError(cond, msg, __FILE__, __LINE__)
#endif

#define ERROR_DIVIDE_ZERO() errorDivideZero(__FILE__)
#define ERROR_SIZE_INDICES_INITIAL_ZERO() errorSizeIndicesInitialZero(__FILE__)
#define ERROR_SIZE_ACTIVE_INDICES_ZERO() errorSizeActiveIndicesZero(__FILE__)

#define WARNING_INDEX_OUT_OF_RANGE() warningIndexOutOfRange(__FILE__)
#define WARNING_FUNC_NOT_CALCULATE() warningFuncNotCalculate(__FILE__)

enum class LogLevel {WARNING, ERROR};

void assertError(bool condition, const std::string& msg, const char* file);

void log(LogLevel level, const std::string& message);

void errorDivideZero(const char* file);

void errorSizeIndicesInitialZero(const char* file);

void errorSizeActiveIndicesZero(const char* file);

void warningIndexOutOfRange(const char* file);

void warningFuncNotCalculate(const char* file);

#endif

