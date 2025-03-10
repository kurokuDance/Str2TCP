#pragma once
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
#define SERVER_PORT 11111

class TCPServer {
public:
    TCPServer() : io_context_(), acceptor_(io_context_, tcp::endpoint(tcp::v4(), SERVER_PORT)) {}

    void startListening() {
        while (true) {
            tcp::socket socket(io_context_);
            acceptor_.accept(socket);
            handleClient(socket);
        }
    }

private:
    boost::asio::io_context io_context_;
    tcp::acceptor acceptor_;

    void handleClient(tcp::socket& socket) {
        boost::asio::streambuf buffer;
        boost::asio::read_until(socket, buffer, '\n');

        std::istream input_stream(&buffer);
        std::string data;
        std::getline(input_stream, data);

        processReceivedData(data);
    }

    void processReceivedData(const std::string& data) {
        if (data.size() > 2 && std::stoi(data) % 32 == 0)
            std::cout << "Received data: " << data << " (Valid)" << std::endl;
        else
            std::cout << "Received data: " << data << " (Invalid)" << std::endl;
    }
};
