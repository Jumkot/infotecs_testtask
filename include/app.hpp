#ifndef APP_HPP
#define APP_HPP

#include <thread>
#include <mutex>
#include <queue>
#include <sstream>
#include <condition_variable>

#include "logger.hpp"

struct MessageQueue
{
    std::queue<Message> messages;      // очередь сообщений
    std::mutex mutex;                  // мьютекс, защищающий очередь
    std::condition_variable condition; // ожидание сообщений в очереди writer-потоком
    bool finished = false;             // флаг окончания приёма сообщений
};

void input_thread(MessageQueue &queue, Level default_level);
void write_thread(MessageQueue &queue, Logger &logger);

#endif