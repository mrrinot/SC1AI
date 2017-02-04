#pragma once

#ifndef LOGGEROPTIONS_HH_
#define LOGGEROPTIONS_HH_

#include <string>
#include "LoggerCallbacks.hh"

enum class LogLevel
{
  Debug5 = 0,
  Debug4,
  Debug3,
  Debug2,
  Debug1,
  Debug,
  Info,
  Warning,
  Error
};

#define LOGGEROPTIONS_GENERATE_SERIALIZATION(level)\
  case LogLevel::level: return #level

inline std::string toString(LogLevel level)
{
  switch (level)
  {
    LOGGEROPTIONS_GENERATE_SERIALIZATION(Debug5);
    LOGGEROPTIONS_GENERATE_SERIALIZATION(Debug4);
    LOGGEROPTIONS_GENERATE_SERIALIZATION(Debug3);
    LOGGEROPTIONS_GENERATE_SERIALIZATION(Debug2);
    LOGGEROPTIONS_GENERATE_SERIALIZATION(Debug1);
    LOGGEROPTIONS_GENERATE_SERIALIZATION(Debug);
    LOGGEROPTIONS_GENERATE_SERIALIZATION(Info);
    LOGGEROPTIONS_GENERATE_SERIALIZATION(Warning);
    LOGGEROPTIONS_GENERATE_SERIALIZATION(Error);
  }
  return "UNKNOWN LogLevel";
}

#undef LOGGEROPTIONS_GENERATE_SERIALIZATION

#define LOGGEROPTIONS_GENERATE_DECLARATION(type, id, name) \
  type name; \
  type get##id() const; \
  LoggerOptions& set##id(type id);

struct LoggerOptions
{
  LoggerOptions();
    LOGGEROPTIONS_GENERATE_DECLARATION(LogLevel, LoggingLevel, _level)
    LOGGEROPTIONS_GENERATE_DECLARATION(bool, AutoLineBreak, _autoLineBreak)
    LOGGEROPTIONS_GENERATE_DECLARATION(bool, AutoSeparator, _autoSeparator)
    LOGGEROPTIONS_GENERATE_DECLARATION(std::string, Separator, _separator)
    LOGGEROPTIONS_GENERATE_DECLARATION(LoggerCallbacks::ProcessorType, Processor, _processor)
    LOGGEROPTIONS_GENERATE_DECLARATION(std::string, Format, _format)
    LOGGEROPTIONS_GENERATE_DECLARATION(std::string, DateFormat, _dateFormat)
};

#undef LOGGEROPTIONS_GENERATE_DECLARATION

#endif // LOGGEROPTIONS_HH_