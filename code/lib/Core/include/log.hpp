#ifndef LOG_HPP_INCLUDED
#define LOG_HPP_INCLUDED

#include <stdio.h>

#ifndef LOG_LEVEL
#define LOG_LEVEL 1
#endif

#define LOG_BLACK "\033[30m"
#define LOG_RED "\033[31m"
#define LOG_GREEN "\033[32m"
#define LOG_YELLOW "\033[33m"
#define LOG_BLUE "\033[34m"
#define LOG_MAGENTA "\033[35m"
#define LOG_CYAN "\033[36m"
#define LOG_WHITE "\033[37m"


#define LOG_RESET "\033[0m"


#if LOG_LEVEL >= 6
#warning "Log level is set to an invalid value, logging is disabled."
#define LOG_LEVEL 0
#endif

#if LOG_LEVEL > 0

#define LOG_RAW(Format, ...) printf(Format, ##__VA_ARGS__); fflush(stdout);
#define LOG_RAW_LINE(Format, ...) LOG_RAW(Format "\r\n", ##__VA_ARGS__)
#define LOG_FORMAT(Module_Name, Type, Format) "| %s | %s:%u:%s() | %s | " Format "\r\n", Module_Name, __FILE__, __LINE__, __FUNCTION__, Type
#define LOG_ALL(Module_Name, Color, Type, Format, ...) LOG_RAW(Color LOG_FORMAT(Module_Name, Type, Format LOG_RESET), ##__VA_ARGS__)
#else
#define LOG_RAW(Tag, Format, ...)
#define LOG_RAW_LINE(Format, ...)
#define LOG_ALL()
#endif

#if LOG_LEVEL >= 1
#define LOG_ERROR(Module_Name, Format, ...) LOG_ALL(Module_Name, LOG_RED, "Error", Format, ##__VA_ARGS__)
#else
#define LOG_ERROR(Module_Name, Format, ...)
#endif

#if LOG_LEVEL >= 2
#define LOG_WARNING(Module_Name, Format, ...) LOG_ALL(Module_Name, LOG_YELLOW, "Warning", Format, ##__VA_ARGS__)
#else
#define LOG_WARNING(Module_Name, Format, ...)
#endif

#if LOG_LEVEL >= 3
#define LOG_INFORMATION(Module_Name, Format, ...) LOG_ALL(Module_Name, LOG_BLUE, "Information", Format, ##__VA_ARGS__)
#else
#define LOG_INFORMATION(Module_Name, Format, ...)
#endif

#if LOG_LEVEL >= 4
#define LOG_DEBUG(Module_Name, Format, ...) LOG_ALL(Module_Name, LOG_WHITE, "Debug", Format, ##__VA_ARGS__)
#define LOG_TRACE() LOG_ALL("Trace", "Debug", " - ");
#define LOG_DUMP(Variable) LOG_ALL("Dump", "Debug", #Variable)
#else
#define LOG_DEBUG(Module_Name, Format, ...)
#define LOG_TRACE()
#define LOG_DUMP(Variable)
#endif

#if LOG_LEVEL >= 5
#define LOG_VERBOSE(Module_Name, Format, ...) LOG_ALL(Module_Name, LOG_WHITE, "Verbose", Format, ##__VA_ARGS__)
#else
#define LOG_VERBOSE(Module_Name, Format, ...)
#endif

#endif