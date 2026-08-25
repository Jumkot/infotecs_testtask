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
// переводы Уровень <=> строка
const char *level_to_string(Level level);
Level string_to_level(const std::string &level);
// определение, является ли слово верхним регистром
bool is_upper(const std::string &word);

class NotLevel : public std::invalid_argument
{
public:
    explicit NotLevel(const std::string &value)
        : std::invalid_argument("Value is not a logging level: " + value)
    {
    }
};

class UnknownLevel : public std::invalid_argument
{
public:
    explicit UnknownLevel(const std::string &value)
        : std::invalid_argument("Unknown logging level: " + value)
    {
    }
};

// класс одного логирующего сообщения
class Message
{
public:
    Message(const std::string &text, Level level);

    // перегрузка оператора вывода для Message
    friend std::ostream &operator<<(std::ostream &os, const Message &message);
    // получить уровень важности сообщения
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