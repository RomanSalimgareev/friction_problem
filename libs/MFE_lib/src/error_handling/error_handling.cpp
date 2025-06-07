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
		std::tm* localTime = std::localtime(&now);

		std::ostringstream timeStream;
		timeStream << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
		std::string timestamp = timeStream.str();

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

		logFile << "[" << timestamp << "] : " << levelStr << " : " 
				<< message << "\n\n";
	}
}

void error(std::string msg, const char* file, const long& line)
{
	msg += "Location error: ";
	msg += std::string(file) + "\t";

	msg += "Line error: ";
	msg += std::to_string(line) + "\n\n";

	log(LogLevel::ERROR, msg);
	throw std::runtime_error(msg);
}

void warning(std::string msg, const char* file, const long& line)
{
	msg += "Location error: ";
	msg += std::string(file) + "\t";

	msg += "Line error: ";
	msg += std::to_string(line) + "\n\n";
	log(LogLevel::WARNING, msg);
}

static std::string messageOutOfRange()
{
	return " Out of range. ";
}

static std::string messageDivideZero()
{
	return " Dividing by zero. ";
}
