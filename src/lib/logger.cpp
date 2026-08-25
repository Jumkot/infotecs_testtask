#include <iomanip>
#include <ctime>
#include <algorithm>
#include <cctype>

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

Level string_to_level(const std::string &level)
{
    if (!is_upper(level))
    {
        throw NotLevel(level);
    }
    if (level == "INFO")
        return INFO;

    if (level == "WARNING")
        return WARNING;

    if (level == "ERROR")
        return ERROR;

    throw UnknownLevel(level);
}

bool is_upper(const std::string &word)
{
    return std::all_of(
        word.begin(),
        word.end(),
        [](unsigned char c)
        {
            return !std::isalpha(c) || std::isupper(c);
        });
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

Level Message::get_level() const
{
    return level_;
}

Logger::Logger(const std::string &filename, Level default_level, std::ios::openmode mode)
    : default_level_(default_level),
      logfile_(filename, mode)
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

Level Logger::get_default_level() const
{
    return default_level_;
}

bool Logger::is_important(Level level) const
{
    return (level >= default_level_);
}

// обработка сообщения без заданного уровня важности
void Logger::log(const std::string &message)
{
    log(Message(message, default_level_));
}

void Logger::log(const std::string &message, Level level)
{
    log(Message(message, level));
}

void Logger::log(const Message &message)
{
    if (!is_important(message.get_level()))
    {
        return;
    }

    logfile_ << message << std::endl;

    if (!logfile_)
    {
        throw std::runtime_error("Failed to write to log file");
    }
}