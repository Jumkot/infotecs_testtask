#include <iostream>

#include "logger.hpp"
#include "app.hpp"

void input_thread(MessageQueue &queue, Level default_level)
{
    std::cout << "Usage: <LOGGING LEVEL> <Logging message>\nor\n\t"
              << "<Logging message>\nor\nType <exit> to stop the app.\n";

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

        Level level;
        try
        {
            level = string_to_level(first_word);
            std::getline(line >> std::ws, text);
            if (text.empty())
                continue;
        }
        catch (const NotLevel &)
        {
            level = default_level;
        }
        catch (const UnknownLevel &error)
        {
            std::cerr << "Input error: " << error.what() << '\n';
            continue;
        }

        Message message(text, level);
        {
            std::lock_guard<std::mutex> lock(queue.mutex);
            queue.messages.push(message);
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

void write_thread(MessageQueue &queue, Logger &logger)
{
    while (true)
    {
        // позволяет на время захватить мьютекс и "уснуть" (wait())
        std::unique_lock<std::mutex> lock(queue.mutex);

        queue.condition.wait(lock,
                             [&queue]()
                             { return !queue.messages.empty() || queue.finished; });

        // завершаем, только когда сообщений уже не будет И очередь уже пуста
        if (queue.messages.empty() && queue.finished)
            break;

        Message message = queue.messages.front();
        queue.messages.pop();

        lock.unlock();
        // запись ведётся без блока мьютекса, чтобы input мог
        // пополнять очередь во время записи writer-ом в файл
        try
        {
            logger.log(message);
        }
        catch (const std::runtime_error &error)
        {
            std::cerr << "Logging error: " << error.what() << '\n';
        }
    }
}