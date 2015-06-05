/*
 * SystemLog.hpp
 *
 *  Created on: Jun 5, 2015
 *      Author: jusabiaga
 */

#ifndef SYSTEMLOG_HPP_
#define SYSTEMLOG_HPP_

#include <string>		// std::string
#include <vector>		// std::vector
#include <map>			// std::map

namespace JU
{

class SystemLog
{
	private:

		class Log
		{
			public:
				void addMessage(const std::string &message) { data_.push_back(message); }
				void clear() { data_.clear(); }
				void print(std::string& output_string) const
				{
					std::vector<std::string>::const_iterator iter;
					for (iter = data_.begin(); iter != data_.end(); iter++)
						output_string.append(*iter);
				}
			private:
				std::vector<std::string> data_;
		};

	public:
		static void logMessage(const char* tag, const char* message);
		static void clearLog(const char* tag);
		static void clarAllLogs();
		static bool printLog(const char* tag);
		static void printAllLogs();

	private:
		// Type Definitions
		typedef std::map<std::string, Log> 	LogMap;
		typedef LogMap::iterator	   		LogMapIter;
		typedef LogMap::const_iterator	   	LogMapConsIter;

		// Member Variables
		static LogMap log_map_;
};

} /* namespace JU */

#endif /* SYSTEMLOG_HPP_ */
