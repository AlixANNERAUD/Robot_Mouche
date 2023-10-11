#ifndef LOG_HPP_INCLUDED
#define LOG_HPP_INCLUDED

#include <stdio.h>

#ifndef LOG_LEVEL
#define LOG_LEVEL 1
#endif

#if LOG_LEVEL >= 6
#warning "Log level is set to an invalid value, logging is disabled."
#define LOG_LEVEL 0
#endif

#if LOG_LEVEL > 0

#define LOG_RAW(Format, ...) printf(Format, ##__VA_ARGS__);
#define LOG_RAW_LINE(Format, ...) LOG_RAW(Format "\r\n", ##__VA_ARGS__)
#define LOG_FORMAT(Module_Name, Type, Format) "| %s | (%s:%u) | %s() | %s | " Format "\r\n", Module_Name, __FILE__, __LINE__, __FUNCTION__, Type
#define LOG_ALL(Module_Name, Type, Format, ...) LOG_RAW(LOG_FORMAT(Module_Name, Type, Format), ##__VA_ARGS__)
#else
#define LOG_RAW(Tag, Format, ...)
#define LOG_RAW_LINE(Format, ...)
#define LOG_ALL()
#endif

#if LOG_LEVEL >= 1
#define LOG_ERROR(Module_Name, Format, ...) LOG_ALL(Module_Name, "Error", Format, ##__VA_ARGS__)
#else
#define LOG_ERROR(Module_Name, Format, ...)
#endif

#if LOG_LEVEL >= 2
#define LOG_WARNING(Module_Name, Format, ...) LOG_ALL(Module_Name, "Warning", Format, ##__VA_ARGS__)
#else
#define LOG_WARNING(Module_Name, Format, ...)
#endif

#if LOG_LEVEL >= 3
#define LOG_INFORMATION(Module_Name, Format, ...) LOG_ALL(Module_Name, "Information", Format, ##__VA_ARGS__)
#else
#define LOG_INFORMATION(Module_Name, Format, ...)
#endif

#if LOG_LEVEL >= 4
#define LOG_DEBUG(Module_Name, Format, ...) LOG_ALL(Module_Name, "Debug", Format, ##__VA_ARGS__)
#define LOG_TRACE() LOG_ALL("Trace", "Debug", " - ");
#define LOG_DUMP(Variable) LOG_ALL("Dump", "Debug", #Variable)
#else
#define LOG_DEBUG(Module_Name, Format, ...)
#define LOG_TRACE()
#define LOG_DUMP(Variable)
#endif

#if LOG_LEVEL >= 5
#define LOG_VERBOSE(Module_Name, Format, ...) LOG_ALL(Module_Name, "Verbose", Format, ##__VA_ARGS__)
#else
#define LOG_VERBOSE(Module_Name, Format, ...)
#endif

#endif