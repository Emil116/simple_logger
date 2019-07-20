#ifndef LOGGER_HPP
#define LOGGER_HPP

#define LOG_INFO(...) Logger::LogInfo(__VA_ARGS__)
#define LOG_DEBUG(...) Logger::LogDebug(__PRETTY_FUNCTION__, __VA_ARGS__)
#define LOG_SUCCESS() Logger::LogSuccess(__PRETTY_FUNCTION__)
#define LOG_ERROR(...) Logger::LogError(__FILE__, __PRETTY_FUNCTION__, __LINE__, __VA_ARGS__)

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>

class Logger
{
  private:
    enum StreamType
    {
        CONSOLE,
        FILE
    };

  public:
    Logger() = delete;

    inline static void SetStream(StreamType stream_type);

    template <typename... Args> inline static void LogInfo(const Args&... args);

    template <typename... Args>
    inline static void LogDebug(const char* function_name, const Args&... args);

    template <typename... Args>
    inline static void LogError(const char* file_name, const char* function_name, int line,
                                const Args&... args);

    inline static void LogSuccess(const char* function_name);

    constexpr static StreamType console = StreamType::CONSOLE;
    constexpr static StreamType file = StreamType::FILE;

  private:
    template <typename Stream, typename T, typename... Args>
    inline static void LogRecursive(Stream& stream, const T& item, const Args&... args);

    template <typename Stream> inline static void LogRecursive(Stream& stream);
    inline static std::_Put_time<char> CurrentTime();

    inline static StreamType stream_type_{Logger::console};
    constexpr static char* log_file_name{"log.txt"};
    
    constexpr static char* error_type{"[ERROR]:"};
    constexpr static char* debug_type{"[DEBUG]:"};
    constexpr static char* success_type{"[SUCCESS]:"};

    constexpr static char* error_type_color{"\033[31m[ERROR]:\033[0m"};
    constexpr static char* debug_type_color{"\033[33m[DEBUG]:\033[0m"};
    constexpr static char* success_type_color{"\033[32m[SUCCESS]:\033[0m"};
};

inline void Logger::SetStream(Logger::StreamType stream_type) { stream_type_ = stream_type; }

template <typename... Args> inline void Logger::LogInfo(const Args&... args)
{
    switch (stream_type_)
    {
        case Logger::console:
            LogRecursive(std::cout, args...);
            break;

        case Logger::file:
        {
            std::ofstream log_file(log_file_name, std::ios_base::binary | std::ios_base::app);
            if (log_file.is_open())
            {
                LogRecursive(log_file, args...);
            }
        }
            break;

        default:
            break;
    }
}

template <typename... Args> inline void Logger::LogDebug(const char* function_name, const Args&... args)
{
    switch (stream_type_)
    {
        case Logger::console:
            LogRecursive(std::cout, debug_type_color, function_name, ":", args...);
            break;

        case Logger::file:
        {
            std::ofstream log_file(log_file_name, std::ios_base::binary | std::ios_base::app);
            if (log_file.is_open())
            {
                LogRecursive(log_file, CurrentTime(), debug_type, function_name, ":", args...);
            }
        }
            break;

        default:
            break;
    }
}

template <typename... Args>
inline void Logger::LogError(const char* file_name, const char* function_name, int line, const Args&... args)
{
    switch (stream_type_)
    {
        case Logger::console:
            LogRecursive(std::cerr, error_type_color, file_name, ":", line, ":", function_name, ":", args...);
            break;

        case Logger::file:
        {
            std::ofstream log_file(log_file_name, std::ios_base::binary | std::ios_base::app);
            if (log_file.is_open())
            {
                LogRecursive(log_file, CurrentTime(), error_type, line, ":", function_name, ":", args...);
            }
        }
            break;

        default:
            break;
    }
}

inline void Logger::LogSuccess(const char* function_name)
{
    switch (stream_type_)
    {
        case Logger::console:
            LogRecursive(std::cout, success_type_color, function_name);
            break;

        case Logger::file:
        {
            std::ofstream log_file(log_file_name, std::ios_base::binary | std::ios_base::app);
            if (log_file.is_open())
            {
                LogRecursive(log_file, CurrentTime(), success_type, function_name);
            }
        }
            break;

        default:
            break;
    }
}

template <typename Stream, typename T, typename... Args>
inline void Logger::LogRecursive(Stream& stream, const T& item, const Args&... args)
{
    stream << item << " ";
    LogRecursive(stream, args...);
}

template <typename Stream> inline void Logger::LogRecursive(Stream& stream)
{
    stream << std::endl;
}

inline std::_Put_time<char> Logger::CurrentTime()
{
    const auto current_time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    return std::put_time(gmtime(&current_time_t), "[%Y-%m-%d %H:%M:%S]");
}

#endif // LOGGER_HPP
