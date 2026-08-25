#include <iostream>

#include "logger.hpp"
#include "app.hpp"

Request::Request()
    : type_(Request::MESSAGE), message_("", INFO), level_(INFO)
{
}

void input_thread(RequestQueue &queue, Level default_level)
{
    std::cout << "Usage: <LOGGING LEVEL> <Logging message>\nor\n       "
              << "<Logging message>\nor     <exit> to stop the app\n"
              << "or     <LOGGING LEVEL> to set other level of importance.\n";

    std::string input;
    while (std::getline(std::cin, input))
    {
        if (input == "exit")
            break;
        if (input.empty())
            continue;

        std::istringstream line(input);
        std::string text = input;
        std::string first_word;
        line >> first_word;

        Request req;
        try
        {
            req.level_ = string_to_level(first_word);
            // пропуск пробела и проверка, что за ним (есть ли ещё слова)
            if (line >> std::ws && line.peek() != EOF)
            {
                std::getline(line, text);
            }
            else
            {
                // если после уровня ничего нет - запрос на смену уровня
                req.type_ = Request::SET_LEVEL;
                default_level = req.level_;
            }
        }
        catch (const NotLevel &)
        {
            req.level_ = default_level; // не указан уровень = по умолчанию
        }
        catch (const UnknownLevel &error)
        {
            // вместо уровня капсом введено что-то, что уровнем не является
            std::cerr << "Input error: " << error.what() << '\n';
            continue;
        }

        if (req.type_ == Request::MESSAGE)
        {
            req.message_ = Message(text, req.level_);
        }
        {
            std::lock_guard<std::mutex> lock(queue.mutex);
            queue.requests.push(req);
        }
        // будим writer после получения сообщения для записи
        queue.condition.notify_one();
    }
    {
        std::lock_guard<std::mutex> lock(queue.mutex);
        queue.finished = true;
    }
    // будим writer после конца ввода (exit)
    queue.condition.notify_one();
}

void write_thread(RequestQueue &queue, Logger &logger)
{
    while (true)
    {
        // позволяет на время захватить мьютекс и "уснуть" (wait())
        std::unique_lock<std::mutex> lock(queue.mutex);

        queue.condition.wait(lock,
                             [&queue]()
                             { return !queue.requests.empty() || queue.finished; });

        // завершаем, только когда сообщений уже не будет И очередь уже пуста
        if (queue.requests.empty() && queue.finished)
            break;

        Request request = queue.requests.front();
        queue.requests.pop();

        lock.unlock();
        // запись ведётся без блока мьютекса, чтобы input мог
        // пополнять очередь во время записи writer-ом в файл
        try
        {
            if (request.type_ == Request::MESSAGE)
            {
                logger.log(request.message_);
            }
            else if (request.type_ == Request::SET_LEVEL)
            {
                logger.set_default_level(request.level_);
                std::cout << "Logging level was changed to " << level_to_string(request.level_) << std::endl;
            }
        }
        catch (const std::runtime_error &error)
        {
            std::cerr << "Logging error: " << error.what() << '\n';
        }
    }
}