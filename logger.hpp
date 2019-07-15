#ifndef LOGGER_HPP
#define LOGGER_HPP

#define LOG_ERROR(...) Logger::LogError(__FILE__, __PRETTY_FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_DEBUG(...) Logger::LogDebug(__PRETTY_FUNCTION__, __VA_ARGS__)
#define LOG_INFO(...) Logger::LogInfo(__VA_ARGS__)

#include <iostream>

class Logger
{
  public:
    template <typename... Args>
    static void LogError(const char* file_name, const char* function_name, int line, const Args&... args);

    template <typename... Args>
    static void LogDebug(const char* function_name, const Args&... args);
    
    template <typename... Args>
    static void LogInfo(const Args&... args);

  private:
    template <typename T, typename... Args> 
    static void LogRecursive_(const T& item, const Args&... args);

    template <typename T, typename... Args> 
    static void LogErrorRecursive_(const T& item, const Args&... args);

    static void LogRecursive_();
    static void LogErrorRecursive_();
};

template <typename... Args>
void Logger::LogError(const char* file_name, const char* function_name, int line, const Args&... args)
{
    std::cerr << "[ERROR] " << file_name << ":" << line << ":" << function_name << ": ";
    LogErrorRecursive_(args...);
}

template <typename... Args>
void Logger::LogDebug(const char* function_name, const Args&... args)
{
    std::cout << "[DEBUG] " << function_name << ": ";
    LogRecursive_(args...);
}

template <typename... Args>
void Logger::LogInfo(const Args&... args)
{
    std::cout << "[INFO]: ";
    LogRecursive_(args...);
}

template <typename T, typename... Args> 
void Logger::LogRecursive_(const T& item, const Args&... args)
{
    std::cout << item << " ";
    LogRecursive_(args...);
}

template <typename T, typename... Args> 
void Logger::LogErrorRecursive_(const T& item, const Args&... args)
{
    std::cerr << item << " ";
    LogErrorRecursive_(args...);
}

void Logger::LogRecursive_()
{
    std::cout << std::endl;
}

void Logger::LogErrorRecursive_()
{
    std::cerr << std::endl;
}

#endif // LOGGER_HPP
