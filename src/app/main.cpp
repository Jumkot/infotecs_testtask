#include <iostream>
#include <thread>
#include <mutex>
#include <filesystem>

#include "logger.hpp"
#include "app.hpp"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <log_file> <default_level>\n";

        return 1;
    }

    const std::string filename = argv[1];
    const std::string level = argv[2];

    Level default_level;
    try
    {
        default_level = string_to_level(level);
    }
    catch (const UnknownLevel &error)
    {
        std::cerr << error.what() << '\n';
        return 1;
    }
    catch (const NotLevel &)
    {
        std::cerr << "Default level must be INFO, WARNING or ERROR\n";
        return 1;
    }

    std::ios::openmode mode = std::ios::app;
    if (std::filesystem::exists(filename))
    {
        std::cout << "Log file \"" << filename << "\" already exists. Overwrite it? [y/n]: ";
        while (true)
        {
            char answer;
            std::cin >> answer;
            if (answer == 'y' || answer == 'Y')
            {
                mode = std::ios::trunc;
                break;
            }
            else if (answer == 'n' || answer == 'N')
            {
                break;
            }
            else
            {
                std::cerr << "Please enter y or n.\n";
            }
        }
    }

    try
    {
        Logger logger(filename, default_level, mode);

        RequestQueue queue;

        std::thread input(input_thread, std::ref(queue), default_level);
        std::thread writer(write_thread, std::ref(queue), std::ref(logger));

        input.join();
        writer.join();
    }
    catch (const std::runtime_error &error)
    {
        std::cerr << "Application error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}