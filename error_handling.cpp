#include "error_handling.h"

void assertError(bool condition, const std::string& msg, const char* file)
{
	if (!(condition))									
	{												
		std::cerr << "Error : " << msg << " Location error: " << file << "\n";	
		std::exit(EXIT_FAILURE);					
	}
}

void log(LogLevel level, const std::string& message)
{
	std::ofstream logFile("error_log.txt", std::ios::app);
	if (logFile.is_open())
	{
		std::time_t now = std::time(nullptr);
		char buffer[26];
		std::string levelStr = "Nothing";
		switch (level)
		{
		case LogLevel::WARNING:
			levelStr = "WARNING";
			break;
		case LogLevel::ERROR:
			levelStr = "ERROR";
			break;
		}
		ctime_s(buffer, sizeof(buffer), &now);
		logFile << "[" << buffer << "] : " << levelStr << "" <<
			message << "\n";
	}
}

void errorDivideZero(const char* file)
{
	std::string msg = "Dividing by 0. ";
	msg += std::string(file) + "\n";
	log(LogLevel::ERROR, msg);
	throw std::runtime_error(msg);
}

void errorSizeIndicesInitialZero(const char* file)
{
	std::string msg = "The size of the INDICES_INITIAL is 0. ";
	msg += std::string(file) + "\n";
	log(LogLevel::ERROR, msg);
	throw std::runtime_error(msg);
}

void errorSizeActiveIndicesZero(const char* file)
{
	std::string msg = "The size of the ACTIVE_INDICES is 0. ";
	msg += std::string(file) + "\n";
	log(LogLevel::ERROR, msg);
	throw std::runtime_error(msg);
}

void warningIndexOutOfRange(const char* file)
{
	std::string msg = "The index goes beyond the array. ";
	msg += std::string(__FILE__) + "\n";
	log(LogLevel::WARNING, msg);
}

void warningFuncNotCalculate(const char* file)
{
	std::string msg = "The function did not calculate the value. ";
	msg += std::string(__FILE__) + "\n";
	log(LogLevel::WARNING, msg);
}