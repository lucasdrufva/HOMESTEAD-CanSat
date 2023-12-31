#pragma once

enum LogLevel {
  LOG_LEVEL_ALL = 1<<1,
  LOG_LEVEL_PRIMARY = 1<<2,
  LOG_LEVEL_SECONDARY = 1<<3,
  LOG_LEVEL_SCIENTIFIC = LOG_LEVEL_PRIMARY | LOG_LEVEL_SECONDARY,
  LOG_LEVEL_RETRIEVAL = 1<<4,
  LOG_LEVEL_ERROR = 1<<5
};

inline LogLevel operator|(LogLevel a, LogLevel b)
{
    return static_cast<LogLevel>(static_cast<int>(a) | static_cast<int>(b));
}