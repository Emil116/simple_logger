#ifndef LOGGER_HPP
#define LOGGER_HPP

#define LOG_CONSOLE_SUCCESS() Logger::LogConsoleSuccess(__PRETTY_FUNCTION__)
#define LOG_CONSOLE_ERROR(...) Logger::LogConsoleError(__FILE__, __PRETTY_FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_CONSOLE_DEBUG(...) Logger::LogConsoleDebug(__PRETTY_FUNCTION__, __VA_ARGS__)
#define LOG_CONSOLE_INFO(...) Logger::LogConsoleInfo(__VA_ARGS__)

#define LOG_FILE_SUCCESS() Logger::LogFileSuccess(__PRETTY_FUNCTION__)
#define LOG_FILE_ERROR(...) Logger::LogFileError(__FILE__, __PRETTY_FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_FILE_DEBUG(...) Logger::LogFileDebug(__PRETTY_FUNCTION__, __VA_ARGS__)
#define LOG_FILE_INFO(...) Logger::LogFileInfo(__VA_ARGS__)

#include <iostream>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <ctime>

class Logger
{
  public:
    template <typename... Args>
    inline static void LogConsoleError(const char* file_name, const char* function_name, int line, const Args&... args);

    template <typename... Args>
    inline static void LogConsoleDebug(const char* function_name, const Args&... args);
    
    template <typename... Args>
    inline static void LogConsoleInfo(const Args&... args);

    template <typename... Args>
    inline static void LogFileError(const char* file_name, const char* function_name, int line, const Args&... args);

    template <typename... Args>
    inline static void LogFileDebug(const char* function_name, const Args&... args);

    template <typename... Args>
    inline static void LogFileInfo(const Args&... args);

    inline static void LogConsoleSuccess(const char* function_name);
    inline static void LogFileSuccess(const char* function_name);

  private:
    template <typename T, typename... Args> 
    inline static void LogConsoleRecursive_(std::ostream& out_stream, const T& item, const Args&... args);

    template <typename T, typename... Args> 
    inline static void LogFileRecursive_(std::ofstream& out_fstream, const T& item, const Args&... args);

    inline static void LogConsoleRecursive_(std::ostream& stream);
    inline static void LogFileRecursive_(std::ofstream& out_fstream);
    inline static std::_Put_time<char> GetCurrentTime();

    inline static std::ofstream file;
};

template <typename... Args>
inline void Logger::LogConsoleError(const char* file_name, const char* function_name, int line, const Args&... args)
{
    std::cerr << "[ERROR] " << file_name << ":" << line << ":" << function_name << ": ";
    LogConsoleRecursive_(std::cerr, args...);
}

template <typename... Args>
inline void Logger::LogConsoleDebug(const char* function_name, const Args&... args)
{
    std::cout << function_name << ": ";
    LogConsoleRecursive_(std::cout, args...);
}

template <typename... Args>
inline void Logger::LogConsoleInfo(const Args&... args)
{
    std::cout << "[INFO]: ";
    LogConsoleRecursive_(std::cout, args...);
}

template <typename T, typename... Args> 
inline void Logger::LogConsoleRecursive_(std::ostream& out_stream, const T& item, const Args&... args)
{
    out_stream << item << " ";
    LogConsoleRecursive_(out_stream, args...);
}

inline void Logger::LogConsoleRecursive_(std::ostream& out_stream)
{
    out_stream << std::endl;
}

template <typename... Args>
inline void Logger::LogFileError(const char* file_name, const char* function_name, int line, const Args&... args)
{
    static std::ofstream file("log.txt", std::ios_base::binary|std::ios_base::app);

    if (!file.is_open())
    {
        LOG_CONSOLE_ERROR("Can not create log.txt file");
        return;
    }

    file << "[" << GetCurrentTime() << "] [ERROR] " << file_name << ":" << line << ":" << function_name << ": ";
    LogFileRecursive_(file, args...);
}

template <typename... Args>
inline void Logger::LogFileDebug(const char* function_name, const Args&... args)
{
    static std::ofstream file("log.txt", std::ios_base::binary|std::ios_base::app);

    if (!file.is_open())
    {
        LOG_CONSOLE_ERROR("Can not create log.txt file");
        return;
    }

    file << "[" << GetCurrentTime() << "] " << function_name << ": ";
    LogFileRecursive_(file, args...);
}

template <typename... Args>
inline void Logger::LogFileInfo(const Args&... args)
{
    static std::ofstream file("log.txt", std::ios_base::binary|std::ios_base::app);

    if (!file.is_open())
    {
        LOG_CONSOLE_ERROR("Can not create log.txt file");
        return;
    }

    file << "[" << GetCurrentTime() << "] [INFO]: ";
    LogFileRecursive_(file, args...);
}

template <typename T, typename... Args> 
inline void Logger::LogFileRecursive_(std::ofstream& out_fstream, const T& item, const Args&... args)
{
    out_fstream << item << " ";
    LogFileRecursive_(out_fstream, args...);
}

inline void Logger::LogFileRecursive_(std::ofstream& out_fstream)
{
    out_fstream << std::endl;
}

inline void Logger::LogConsoleSuccess(const char* function_name)
{
    std::cout << "[SUCCESS]: " << function_name << std::endl;
}

inline void Logger::LogFileSuccess(const char* function_name)
{
    static std::ofstream file("log.txt", std::ios_base::binary|std::ios_base::app);

    if (!file.is_open())
    {
        LOG_CONSOLE_ERROR("Can not create log.txt file");
        return;
    }

    file << "[" << GetCurrentTime() << "] [SUCCESS]: " << function_name << std::endl;
}

inline std::_Put_time<char> Logger::GetCurrentTime()
{   
    const auto current_time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    
    return std::put_time(gmtime(&current_time_t), "%Y-%m-%d %H:%M:%S");
}

#endif // LOGGER_HPP
