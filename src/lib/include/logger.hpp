#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <string>
#include <chrono>
#include <ostream>

enum Level
{
    INFO = 0,
    WARNING,
    ERROR

};
const char *level_to_string(Level level);

// класс одного логирующего сообщения
class Message
{
public:
    Message(const std::string &text, Level level);

    friend std::ostream &operator<<(std::ostream &os, const Message &message);

    Level get_level() const;

private:
    Level level_;
    std::chrono::system_clock::time_point timestamp_;
    std::string text_;
};

// класс журнала
class Logger
{
public:
    Logger(const std::string &filename, Level default_level);

    // изменить уровень важности сообщения по умолчанию
    void set_default_level(Level new_level);
    // пролучить уровень важности сообщения по умолчанию
    Level get_default_level() const;

    // внесение записи в журнал (с уровнем важности или без)
    void log(const std::string &message);
    void log(const std::string &message, Level level);
    void log(const Message &message);

private:
    Level default_level_;
    std::ofstream logfile_;

    // проверка уровня важности входящего сообщения
    bool is_important(Level level) const;
};

#endif