#ifndef LOGGER_HPP
#define LOGGER_HPP

#define LOG_CONSOLE_ERROR(...) Logger::LogConsoleError(__FILE__, __PRETTY_FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_CONSOLE_DEBUG(...) Logger::LogConsoleDebug(__PRETTY_FUNCTION__, __VA_ARGS__)
#define LOG_CONSOLE_INFO(...) Logger::LogConsoleInfo(__VA_ARGS__)

#define LOG_FILE_ERROR(...) Logger::LogFileError(__FILE__, __PRETTY_FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_FILE_DEBUG(...) Logger::LogFileDebug(__PRETTY_FUNCTION__, __VA_ARGS__)
#define LOG_FILE_INFO(...) Logger::LogFileInfo(__VA_ARGS__)

#include <iostream>
#include <fstream>

class Logger
{
  public:
    template <typename... Args>
    static void LogConsoleError(const char* file_name, const char* function_name, int line, const Args&... args);

    template <typename... Args>
    static void LogConsoleDebug(const char* function_name, const Args&... args);
    
    template <typename... Args>
    static void LogConsoleInfo(const Args&... args);

    template <typename... Args>
    static void LogFileError(const char* file_name, const char* function_name, int line, const Args&... args);

    template <typename... Args>
    static void LogFileDebug(const char* function_name, const Args&... args);

    template <typename... Args>
    static void LogFileInfo(const Args&... args);

  private:
    template <typename T, typename... Args> 
    static void LogConsoleRecursive_(std::ostream& out_stream, const T& item, const Args&... args);

    template <typename T, typename... Args> 
    static void LogFileRecursive_(std::ofstream& out_fstream, const T& item, const Args&... args);

    static void LogConsoleRecursive_(std::ostream& stream);
    static void LogFileRecursive_(std::ofstream& out_fstream);

    static std::ofstream file;
};

std::ofstream Logger::file("log.txt", std::ios_base::binary|std::ios_base::app);

template <typename... Args>
void Logger::LogConsoleError(const char* file_name, const char* function_name, int line, const Args&... args)
{
    std::cerr << "[ERROR] " << file_name << ":" << line << ":" << function_name << ": ";
    LogConsoleRecursive_(std::cerr, args...);
}

template <typename... Args>
void Logger::LogConsoleDebug(const char* function_name, const Args&... args)
{
    std::cout << "[DEBUG] " << function_name << ": ";
    LogConsoleRecursive_(std::cout, args...);
}

template <typename... Args>
void Logger::LogConsoleInfo(const Args&... args)
{
    std::cout << "[INFO]: ";
    LogConsoleRecursive_(std::cout, args...);
}

template <typename T, typename... Args> 
void Logger::LogConsoleRecursive_(std::ostream& out_stream, const T& item, const Args&... args)
{
    out_stream << item << " ";
    LogConsoleRecursive_(out_stream, args...);
}

void Logger::LogConsoleRecursive_(std::ostream& out_stream)
{
    out_stream << std::endl;
}

template <typename... Args>
void Logger::LogFileError(const char* file_name, const char* function_name, int line, const Args&... args)
{
    if (!file.is_open())
    {
        LOG_CONSOLE_ERROR("Can not create log.txt file");
        return;
    }

    file << "[ERROR] " << file_name << ":" << line << ":" << function_name << ": ";
    LogFileRecursive_(file, args...);
}

template <typename... Args>
void Logger::LogFileDebug(const char* function_name, const Args&... args)
{
    if (!file.is_open())
    {
        LOG_CONSOLE_ERROR("Can not create log.txt file");
        return;
    }

    file << "[DEBUG] " << function_name << ": ";
    LogFileRecursive_(file, args...);
}

template <typename... Args>
void Logger::LogFileInfo(const Args&... args)
{
    if (!file.is_open())
    {
        LOG_CONSOLE_ERROR("Can not create log.txt file");
        return;
    }

    file << "[INFO]: ";
    LogFileRecursive_(file, args...);
}

template <typename T, typename... Args> 
void Logger::LogFileRecursive_(std::ofstream& out_fstream, const T& item, const Args&... args)
{
    out_fstream << item << " ";
    LogFileRecursive_(out_fstream, args...);
}

void Logger::LogFileRecursive_(std::ofstream& out_fstream)
{
    out_fstream << std::endl;
}

#endif // LOGGER_HPP
