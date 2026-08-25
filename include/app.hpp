#ifndef APP_HPP
#define APP_HPP

#include <thread>
#include <mutex>
#include <queue>
#include <sstream>
#include <condition_variable>

#include "logger.hpp"

// запрос - либо лог-сообщение,
// либо запрос на смену дефолт уровня
class Request
{
public:
    Request();

    enum Type
    {
        MESSAGE,
        SET_LEVEL
    };

    Type type_;
    Message message_;
    Level level_;
};

struct RequestQueue
{
    std::queue<Request> requests;      // очередь запросов
    std::mutex mutex;                  // мьютекс, защищающий очередь
    std::condition_variable condition; // ожидание запросов в очереди writer-потоком
    bool finished = false;             // флаг окончания приёма
};

void input_thread(RequestQueue &queue, Level default_level);
void write_thread(RequestQueue &queue, Logger &logger);

#endif