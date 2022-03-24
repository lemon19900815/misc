#include "engine.h"

#include <time.h>
#include <fstream>
using namespace std;

#ifndef PLATFORM_WINDOWS
#include <sys/time.h>
#include <unistd.h>
#endif

#include <stdarg.h>

namespace Engine
{
	// get current system time-milli second
	uint64_t getTimeNow()
	{
		uint64_t now = 0;

#ifdef PLATFORM_WINDOWS
		time_t curTime;
		time(&curTime);

		now = (uint64_t)curTime * 1000;

		SYSTEMTIME sys;
		GetLocalTime( &sys );

		now += sys.wMilliseconds;
#else
		struct timeval current;
		gettimeofday(&current, 0);

		now = (uint64_t)current.tv_sec * 1000 + current.tv_usec / 1000;
#endif

		return now;
	}

	// get current system time-second
	int32_t getCurrentTime()
	{
		time_t curTime;
		time(&curTime);
		return (int32_t)curTime;
	}

	// split string by delimiters
	void splitString(const std::string& str, const std::string& delimiters, std::vector<std::string>& tokens)
	{
		// Skip delimiters at beginning. 
		std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
		// Find first "non-delimiter". 
		std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

		while (std::string::npos != pos || std::string::npos != lastPos)
		{
			// Found a token, add it to the vector. 
			tokens.push_back(str.substr(lastPos, pos - lastPos));
			// Skip delimiters.  Note the "not_of" 
			lastPos = str.find_first_not_of(delimiters, pos);
			// Find next "non-delimiter" 
			pos = str.find_first_of(delimiters, lastPos);
		}
	}

	TabFile::TabFile()
	{
		mRowNum = 0;
		mColNum = 0;
	}

	TabFile::~TabFile()
	{
	}

	bool TabFile::load(const char* filename)
	{
		ifstream fin(filename);
		if (!fin) return false;

	#define LINE_BUFFER  4096*2

		char buffer[LINE_BUFFER];
		char field[512] = {0};
		bool firstLine = true;
		while(fin.getline(buffer, LINE_BUFFER))
		{
			ValueCol fields;
			char* tmp = buffer;

			int32_t len = (int32_t)strlen(buffer);
			if(len - 1 >= 0 && buffer[len - 1] == '\n')
				buffer[len - 1] = '\0';
			if(len - 2 >= 0 && buffer[len - 2] == '\r')
				buffer[len - 2] = '\0';

			char* end = buffer+LINE_BUFFER-1;
			char* pField = tmp;
			while(tmp != end)
			{
				if (*tmp == '\t')
				{
					*tmp = '\0';
					if (tmp-pField >= 0)
					{
						fields.push_back(pField);
						tmp++;
						pField = tmp;
					}else
					{
						printf("file %s format error buffer %s \n", filename, buffer);
						fin.close();
						return false;
					}
					continue;
				}
				tmp++;
			}
			if (pField != end)
			{
				fields.push_back(pField);
			}
			if (firstLine)
			{
				mColNum = fields.size();
				firstLine = false;
			}else
			{
				if (mColNum != fields.size())
				{
					//two rows has different cols error
					printf("format error different cols\n");
					fin.close();
					return false;
				}
				mValues.push_back(fields);
			}
			memset(buffer, 0, sizeof(buffer));
		}
		mRowNum = mValues.size();
		fin.close();
		return true;
	}

	bool TabFile::loadFromData(const char* data)
	{
		ValueCol row;
		splitString(data, "\n", row);

		if(row.size() == 0)
			return false;

		mValues.clear();
		mColNum = 0;
		mRowNum = 0;

		for(uint32_t i = 0; i < row.size(); ++i)
		{
			std::string str = row[i];
			uint32_t strLength = str.length();
			if(strLength - 1 >= 0 && str[strLength - 1] == '\n')
				str[strLength - 1] = '\0';
			if(strLength - 2 >= 0 && str[strLength - 2] == '\r')
				str[strLength - 2] = '\0';

			ValueCol rowData;
			splitString(str, "\t", rowData);
			if((int32_t)rowData.size() == 0)
				break;

			if(mColNum != 0) {
				if(mColNum != (int32_t)rowData.size()) // different column
					return false;

				mValues.push_back(rowData);
			} else { // first line
				mColNum = (int32_t)rowData.size();
			}
		}

		mRowNum = (int32_t)mValues.size();

		return mRowNum > 0;
	}

	const char* TabFile::getString(int32_t row, int32_t col, const char* defaultValue)
	{
		if(row < mRowNum && col < mColNum && mValues[row][col].size()) {
			return mValues[row][col].c_str();
		}
		return defaultValue;
	}

	int32_t TabFile::getInteger(int32_t row, int32_t col, const int32_t defaultValue)
	{
		if(row < mRowNum && col < mColNum && mValues[row][col].size()) {
			return atoi(mValues[row][col].c_str());
		}
		return defaultValue;
	}

	float TabFile::getFloat(int32_t row, int32_t col, const float defaultValue)
	{
		if(row < mRowNum && col < mColNum && mValues[row][col].size()) {
			return (float)atof(mValues[row][col].c_str());
		}
		return defaultValue;
	}

	int32_t TabFile::RowNum()
	{
		return mRowNum;
	}

	int32_t TabFile::ColNum()
	{
		return mColNum;
	}


	LogFile* LogFile::getInstance()
	{
		static LogFile _inst;
		return &_inst;
	}

	LogFile::LogFile()
	{
		
	}

	LogFile::~LogFile()
	{
	}

	void LogFile::setLogMessageProvider(PLogMessage log)
	{
		m_LogDelegate = log;
	}

	void LogFile::LogMessage(const char* fmt, ...)
	{
		if(m_LogDelegate == NULL)
			return;

		char buffer[512] = {0};
		sprintf(buffer, "[LIBAI]");

		va_list args;
		va_start(args, fmt);
		vsprintf(buffer + 7, fmt, args);
		va_end(args);

		m_LogDelegate(buffer);
	}
}
