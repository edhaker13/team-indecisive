/*
Copyright (c) 2014, The Cinder Project, All rights reserved.
This code is intended for use with the Cinder C++ library: http://libcinder.org
Redistribution and use in source and binary forms, with or without modification, are permitted provided that
the following conditions are met:
* Redistributions of source code must retain the above copyright notice, this list of conditions and
the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
the following disclaimer in the documentation and/or other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#include "Logger.h"
#include <algorithm>

namespace Indecisive
{
	LogManager* LogManager::_pInstance = new LogManager;

	std::ostream& operator<<(std::ostream& lhs, const LogLevel& rhs)
	{
		switch (rhs) {
		case LEVEL_VERBOSE:		lhs << "|verbose|";	break;
		case LEVEL_DEBUG:		lhs << "|debug  |";	break;
		case LEVEL_INFO:		lhs << "|info   |";	break;
		case LEVEL_WARNING:		lhs << "|warning|";	break;
		case LEVEL_ERROR:		lhs << "|error  |";	break;
		case LEVEL_FATAL:		lhs << "|fatal  |";	break;
		default: break;//CI_ASSERT_NOT_REACHABLE();
		}
		return lhs;
	}

	FileLogger::FileLogger(const std::string& filepath, bool append)
		: _filepath(filepath), _append(append)
	{
		if (_filepath.empty())
			_filepath = DefaultPath();
	}

	FileLogger::~FileLogger()
	{
		if (_stream.is_open())
			_stream.close();
	}

	bool FileLogger::CanWrite() const
	{
		assert(!_filepath.empty());
		auto pos = _filepath.find_last_of('.');
		if (pos == std::string::npos)
			return false;
		auto ext = _filepath.substr(pos + 1);
		return ext.compare("log") == 0;
	};

	void FileLogger::Write(const LogLevel& level, const std::string& text)
	{
		if (!_stream.is_open())
		{
			_append ? _stream.open(_filepath, std::ofstream::app) : _stream.open(_filepath);
		}
		InternalWrite(_stream, level, text);
	}

	void LogManager::AddLogger(const LoggerRef& logger)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_loggers.push_back(logger);
	}

	void LogManager::ClearLoggers()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_loggers.clear();
	}

	void LogManager::RemoveLogger(const LoggerRef& logger)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_loggers.erase(std::remove_if(_loggers.begin(), _loggers.end(),
			[logger](const LoggerRef& o) {
			return o == logger;
		}),
			_loggers.end());
	}

	void LogManager::ResetLogger(const LoggerRef& logger)
	{
		std::lock_guard<std::mutex> lock(LogManagerInstance()->GetMutex());
		_loggers.clear();
		_loggers.push_back(logger);
	}

	std::vector<LoggerRef> LogManager::GetAllLoggers()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		return _loggers;
	}

	void LogManager::ToDefault()
	{
		ClearLoggers();
		MakeLogger<FileLogger>();
	}

	void LogManager::Write(const LogLevel& level, const std::string& text)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		for (auto& logger : _loggers)
		{
			logger->Write(level, text); 
		}
	}
}