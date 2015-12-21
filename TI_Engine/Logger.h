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

#pragma once
#include <cassert>
#include <fstream>
#include <memory>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "LibraryAPI.h"

namespace Indecisive
{
	typedef enum {
		LEVEL_VERBOSE,
		LEVEL_DEBUG,
		LEVEL_INFO,
		LEVEL_WARNING,
		LEVEL_ERROR,
		LEVEL_FATAL
	} LogLevel;

	extern std::ostream& operator<<(std::ostream &lhs, const LogLevel &rhs);

	class ILogger
	{
	private:
		ILogger(ILogger const&) = delete;
		void operator=(ILogger const&) = delete;
	protected:
		ILogger() {};
		void InternalWrite(std::ostream& stream, const LogLevel& level, const std::string& text)
		{
			stream << level << " " << text << std::endl;
		};
	public:
		virtual const std::string GetInfo() const = 0;
		virtual void Write(const LogLevel&, const std::string&) = 0;
	};

	typedef std::shared_ptr<ILogger> LoggerRef;

	class FileLogger : public ILogger
	{
	protected:
		bool _append;
		std::string _filepath;
		std::ofstream _stream;
		virtual bool CanWrite() const;
		std::string DefaultPath() const { return "Indecisive.log"; };
		virtual const std::string GetInfo() const override { return "Write log entries to a .log file";	};
		void Write(const LogLevel&, const std::string&) override;
	public:
		FileLogger(const std::string& = "", bool = true);
		virtual ~FileLogger();
		const std::string& GetPath() const { return _filepath; };
	};

	/// <summary> LogManager manages a stack of all active Loggers. </summary>
	// LogManager's default state contains a single FileLogger.  LogManager allows to add and remove Loggers via their pointer values.
	class LogManager {
	public:
		// Returns a pointer to the shared instance. To enable logging during shutdown, this instance is leaked at shutdown.
		LIBRARY_API static LogManager* Instance()	{ return _pInstance; }
		// Destroys the shared instance. Useful to remove false positives with leak detectors like valgrind.
		static void DestroyInstance()	{ delete _pInstance; }
		//! Adds a logger to the current stack of loggers.
		void AddLogger(const LoggerRef& logger);
		// Removes all loggers from the stack.
		void ClearLoggers();
		// Returns a vector of LoggerRef that contains all active loggers
		std::vector<LoggerRef> GetAllLoggers();
		// Returns a vector of Loggers of a specifc type.
		template<typename LoggerT>
		std::vector<std::shared_ptr<LoggerT>> GetLoggers();
		// Returns the mutex used for thread safe logging.
		std::mutex& GetMutex() const { return _mutex; }
		// Remove a logger from the current stack of loggers.
		void RemoveLogger(const LoggerRef& logger);
		// Resets the current Logger stack so only logger exists.
		void ResetLogger(const LoggerRef& logger);
		// LogManager to its default state - a single FileLogger.
		void ToDefault();
		// Writes log level and string to all active loggers.
		LIBRARY_API void Write(const LogLevel&, const std::string&);

		// Creates and returns a new logger of type LoggerT, adding it to the current Logger stack.
		template<typename LoggerT, typename... Args>
		std::shared_ptr<LoggerT> MakeLogger(Args&&... args);

	protected:
		LogManager() { ToDefault(); };
		std::vector<LoggerRef> _loggers;
		mutable std::mutex _mutex;
		static LogManager* _pInstance;
	};

	// Freestanding functions

	// The global manager for logging, used to manipulate the Logger stack. Provides thread safety amongst the Loggers.
	LogManager* LogManagerInstance() { return LogManager::Instance(); };

	// Creates and returns a new logger of type LoggerT, adding it to the current Logger stack.
	template<typename LoggerT, typename... Args>
	std::shared_ptr<LoggerT> MakeLogger(Args&&... args)
	{
		return LogManagerInstance()->MakeLogger<LoggerT>(std::forward<Args>(args)...);
	}

	// Pieces together an entries information, returns a string.
	template<typename T>
	const std::string MakeEntry(const std::string& function, const std::string& filename, const size_t line, const T& stream)
	{
		std::stringstream ss;
		ss << function << " [L" << line << "] " << stream;
		return ss.str();
	}

	// Template method implementations

	template<typename LoggerT, typename... Args>
	std::shared_ptr<LoggerT> LogManager::MakeLogger(Args&&... args)
	{
		static_assert(std::is_base_of<ILogger, LoggerT>::value, "LoggerT must inherit from ILogger");

		std::shared_ptr<LoggerT> result = std::make_shared<LoggerT>(std::forward<Args>(args)...);
		AddLogger(result);
		return result;
	}

	template<typename LoggerT>
	std::vector<std::shared_ptr<LoggerT>> LogManager::GetLoggers()
	{
		std::vector<std::shared_ptr<LoggerT>> result;

		std::lock_guard<std::mutex> lock(LogManagerInstance()->GetMutex());
		for (const auto& logger : _loggers) {
			auto loggerCasted = std::dynamic_pointer_cast<LoggerT>(logger);
			if (loggerCasted) {
				result.push_back(loggerCasted);
			}
		}
		return result;
	}

} // namespace Indecisive

// Logging macros

#define INDECISIVE_LOG_STREAM(level, stream) ::Indecisive::LogManagerInstance()->Write(level, ::Indecisive::MakeEntry(__FUNCTION__, __FILE__, __LINE__, stream))

// TI_MIN_LOG_LEVEL is designed so that if you set it to 7 : nothing logs, 6 : only fatal, 5 : fatal + error, ..., 1 : everything

#if !defined(TI_MIN_LOG_LEVEL)
#	if !defined(NDEBUG)
#		define TI_MIN_LOG_LEVEL 0	// debug mode default is LEVEL_VERBOSE
#	else
#		define TI_MIN_LOG_LEVEL 2	// release mode default is LEVEL_INFO
#	endif
#endif

#if (TI_MIN_LOG_LEVEL <= 0)
#	define TI_LOG_V(stream)	INDECISIVE_LOG_STREAM(::Indecisive::LEVEL_VERBOSE, stream)
#else
#	define TI_LOG_V(stream) ((void)0)
#endif

#if (TI_MIN_LOG_LEVEL <= 1)
#	define TI_LOG_D(stream)	INDECISIVE_LOG_STREAM(::Indecisive::LEVEL_DEBUG, stream)
#else
#	define TI_LOG_D(stream) ((void)0)
#endif

#if (TI_MIN_LOG_LEVEL <= 2)
#	define TI_LOG_I(stream) INDECISIVE_LOG_STREAM(::Indecisive::LEVEL_INFO, stream)
#else
#	define TI_LOG_I(stream) ((void)0)
#endif

#if (TI_MIN_LOG_LEVEL <= 3)
#	define TI_LOG_W(stream) INDECISIVE_LOG_STREAM(::Indecisive::LEVEL_WARNING, stream)
#else					 
#	define TI_LOG_W(stream) ((void)0)
#endif

#if (TI_MIN_LOG_LEVEL <= 4)
#	define TI_LOG_E(stream) INDECISIVE_LOG_STREAM(::Indecisive::LEVEL_ERROR, stream)
#else					 
#	define TI_LOG_E(stream) ((void)0)
#endif

#if (TI_MIN_LOG_LEVEL <= 5)
#	define TI_LOG_F(stream) INDECISIVE_LOG_STREAM(::Indecisive::LEVEL_FATAL, stream)
#else					 
#	define TI_LOG_F(stream) ((void)0)
#endif

//! Debug macro to simplify logging an exception, which also prints the exception type
#define TI_LOG_EXCEPTION(stream, ex)	\
{										\
	std::stringstream ss;				\
	ss << stream << ", exception type: " << typeid(ex).name() << ", what: " << ex.what(); \
	TI_LOG_E(ss.str());	\
}