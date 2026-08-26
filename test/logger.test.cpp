#include <iostream>
#include <cstdio>

#include "logger.hpp"

#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_RESET "\033[0m"

#define CHECK(condition)  \
    do                    \
    {                     \
        if (!(condition)) \
            return false; \
    } while (false)

#define RUN_TEST(number, test)                       \
    do                                               \
    {                                                \
        bool result = test();                        \
        show_test_result(number, result, #test + 5); \
        passed = passed && result;                   \
        if (!result)                                 \
            ++fail;                                  \
    } while (false)

const std::string TEST_LOG = "logger_test.log";
const int TEST_N = 5;

// управляет файлом вывода теста
class TestFile
{
public:
    explicit TestFile(const std::string &path) : path_(path)
    {
        std::remove(path_.c_str());
    }

    ~TestFile()
    {
        std::remove(path_.c_str());
    }

private:
    std::string path_;
};

// форматированный вывод результатов теста
void show_test_result(int number, bool passed, const char *name)
{
    if (passed)
    {
        std::cout << COLOR_GREEN << "[" << number << "/" << TEST_N
                  << "] PASSED: " << name << COLOR_RESET << "\n";
    }
    else
    {
        std::cout << COLOR_RED << "[" << number << "/" << TEST_N
                  << "] FAILED: " << name << COLOR_RESET << "\n";
    }
}

// фильтрация в соответствии с заданным по умолчанию уровнем важности
bool test_level_filtering()
{
    TestFile test_file(TEST_LOG);
    Logger logger(TEST_LOG, WARNING, std::ios::trunc);

    logger.log("This message must not be logged", INFO);
    logger.log("Warning message", WARNING);
    logger.log("Error message", ERROR);

    std::ifstream file(TEST_LOG);

    CHECK(file.is_open());

    std::string line;

    CHECK(std::getline(file, line));
    CHECK(line.find("[WARNING]") != std::string::npos);
    CHECK(line.find("Warning message") != std::string::npos);

    CHECK(std::getline(file, line));
    CHECK(line.find("[ERROR  ]") != std::string::npos);
    CHECK(line.find("Error message") != std::string::npos);

    CHECK(!std::getline(file, line));

    return true;
}

// запись без указания уровня важности
bool test_default_level()
{
    TestFile test_file(TEST_LOG);
    Logger logger(TEST_LOG, WARNING, std::ios::trunc);

    logger.log("Default level message");

    std::ifstream file(TEST_LOG);

    CHECK(file.is_open());

    std::string line;

    CHECK(std::getline(file, line));
    CHECK(line.find("[WARNING]") != std::string::npos);
    CHECK(line.find("Default level message") != std::string::npos);

    CHECK(!std::getline(file, line));

    return true;
}

// изменение уровня важности по умолчанию
bool test_set_default_level()
{
    TestFile test_file(TEST_LOG);
    Logger logger(TEST_LOG, ERROR, std::ios::trunc);

    logger.log("This must not be logged", WARNING);

    logger.set_default_level(INFO);

    logger.log("This must be logged", INFO);

    std::ifstream file(TEST_LOG);

    CHECK(file.is_open());

    std::string line;

    CHECK(std::getline(file, line));
    CHECK(line.find("[INFO   ]") != std::string::npos);
    CHECK(line.find("This must be logged") != std::string::npos);

    CHECK(!std::getline(file, line));

    return true;
}

// ошибка открытия файла
bool test_invalid_file()
{
    bool exception_thrown = false;

    try
    {
        Logger logger("/nonexistent_directory/logger.log", INFO, std::ios::trunc);
    }
    catch (const std::runtime_error &)
    {
        exception_thrown = true;
    }

    return exception_thrown;
}

// ошибка записи в файл
bool test_write_error()
{
    bool exception_thrown = false;

    try
    {
        Logger logger("/dev/full", INFO, std::ios::trunc);
        logger.log("This write must fail");
    }
    catch (const std::runtime_error &)
    {
        exception_thrown = true;
    }

    return exception_thrown;
}

void test_all()
{
    bool passed = true;
    int fail = 0;

    RUN_TEST(1, test_level_filtering);
    RUN_TEST(2, test_default_level);
    RUN_TEST(3, test_set_default_level);
    RUN_TEST(4, test_invalid_file);
    RUN_TEST(5, test_write_error);

    std::cout << '\n';
    if (passed)
    {
        std::cout << COLOR_GREEN << "All tests passed!" << COLOR_RESET << "\n";
    }
    else
    {
        std::cout << COLOR_RED << fail << "/" << TEST_N << " tests failed" << COLOR_RESET << "\n";
    }
};

int main()
{
    test_all();

    return 0;
}