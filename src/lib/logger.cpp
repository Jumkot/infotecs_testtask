#include <iomanip>
#include <ctime>
#include <stdexcept>

#include "logger.hpp"

const char *level_to_string(Level level)
{
    switch (level)
    {
    case INFO:
        return "INFO";
    case WARNING:
        return "WARNING";
    case ERROR:
        return "ERROR";
    }

    return "UNKNOWN";
}

Message::Message(const std::string &text, Level level)
    : level_(level), timestamp_(std::chrono::system_clock::now()), text_(text)
{
}

std::ostream &operator<<(std::ostream &stream, const Message &message)
{
    std::time_t time = std::chrono::system_clock::to_time_t(message.timestamp_);

    std::tm local_time;
    localtime_r(&time, &local_time);

    stream << '[' << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S") << "] ["
           << std::left << std::setw(7) << level_to_string(message.level_)
           << "]: " << message.text_;
    return stream;
}

Logger::Logger(const std::string &filename, Level default_level)
    : default_level_(default_level),
      logfile_(filename)
{
    if (!logfile_.is_open())
    {
        throw std::runtime_error("Failed to open log file: " + filename);
    }
}

void Logger::set_default_level(Level new_level)
{
    default_level_ = new_level;
}

bool Logger::is_important(Level level) const
{
    return (level >= default_level_);
}

// обработка сообщения без заданного уровня важности
void Logger::log(const std::string &message)
{
    log(message, default_level_);
}

void Logger::log(const std::string &message, Level level)
{
    if (!is_important(level))
    {
        return;
    }

    Message msg(message, level);

    logfile_ << msg << std::endl;

    if (!logfile_)
    {
        throw std::runtime_error("Failed to write to log file");
    }
}