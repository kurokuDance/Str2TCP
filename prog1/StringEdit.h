#pragma once
#include <iostream>
#include <algorithm>
#include <queue>
#include <mutex>
#include <thread>
#include "boost/asio.hpp"
#include <condition_variable>

using boost::asio::ip::tcp;

#define SERVER_PORT 11111

class StringEditor
{
public:
    void run()
    {
        std::thread th1([&]() { inputThread(); });
        std::thread th2([&]() { outputThread(); });

        th1.join();
        th2.join();
    }

private:
    std::string str;
    std::queue<std::string> buffer;
    std::mutex mtx;
    std::condition_variable cv;

    // Обрабатывает пользовательский ввод и помещает данные в буфер
    void inputThread()
    {
        while (true)
        {
            std::getline(std::cin, str);
            if (isCorectString(str))
            {
                sortString();
                replaceString();
                std::lock_guard<std::mutex> lock(mtx);
                buffer.push(str);
                cv.notify_one();
            }
            else
                std::cout << "string not correct" << std::endl;
        }
    }

    // Отправляет обработанные данные через сокет в программу 2
    void outputThread()
    {
        while (true)
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [&]() { return !buffer.empty(); });

            if (!buffer.empty()) {
                outputData();
                lock.unlock();
                boost::asio::io_context io_context;
                tcp::socket socket(io_context);
                try
                {
                    socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), SERVER_PORT));
                    std::string sum_str = std::to_string(sumNumbersInString());
                    boost::asio::write(socket, boost::asio::buffer(sum_str + "\n"));
                }
                catch (const std::exception& e)
                {
                    std::cout << "Error connecting to the server" << std::endl;
                }
                while (!buffer.empty()) {
                    buffer.pop();
                }
            }
        }
    }


    // Проверяет, что строка состоит только из цифр и не превышает 64 символа
    bool isCorectString(std::string str)
    {
        if (str.size() > 64)
            return false;
        for (int i = 0; i < str.size(); i++)
            if (!isdigit(str[i]) || str[i] == ' ')
                return false;
        return true;
    }

    void sortString()
    {
        sort(str.begin(), str.end());
    }

    // Заменяет четные символы на "KB"
    void replaceString()
    {
        for (int i = 0; i < str.size(); i++)
            if ((int)str[i] % 2 == 0)
            {
                str.replace(i, 1, "KB");
                i++;
            }

    }

    void outputData()
    {
        std::cout << "received data: " << buffer.front() << std::endl;
    }

    // Вычисляет сумму чисел в буффере
    int sumNumbersInString() {
        int sum = 0, currentNumber = 0;
        std::string str = buffer.front();
        for (char c : str) {
            if (std::isdigit(c)) currentNumber = currentNumber * 10 + (c - '0');
            else { sum += currentNumber; currentNumber = 0; }
        }
        return sum + currentNumber;
    }
};
