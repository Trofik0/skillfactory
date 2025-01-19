#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <thread>

using boost::asio::ip::tcp;

void read_messages(tcp::socket& socket) {
    char data[1024];
    while (true) {
        size_t length = boost::asio::read(socket, boost::asio::buffer(data, 1024));
        std::cout << std::string(data, length) << std::endl; // Выводим сообщения от других пользователей.
    }
}

int main() {
    try {
        boost::asio::io_service io_service;
        tcp::socket socket(io_service);
        socket.connect(tcp::endpoint(tcp::v4(), 12345));

        std::string username;
        std::string password;
        std::cout << "Enter your username: ";
        std::getline(std::cin, username);
        std::cout << "Enter your password: ";
        std::getline(std::cin, password);

        // Регистрация или вход
        std::string command;
        while (true) {
            std::cout << "Type 'register' to create an account or 'login' to sign in: ";
            std::getline(std::cin, command);
            if (command == "register") {
                boost::asio::write(socket, boost::asio::buffer("REGISTER:" + username + ":" + password));
                break;
            } else if (command == "login") {
                boost::asio::write(socket, boost::asio::buffer("LOGIN:" + username + ":" + password));
                break;
            }
        }

        // Ожидание сообщения от сервера о статусе входа или регистрации
        char welcome_msg[1024];
        boost::asio::read(socket, boost::asio::buffer(welcome_msg, 1024));
        std::cout << std::string(welcome_msg) << std::endl;

        std::thread reader_thread(read_messages, std::ref(socket));

        // Основной цикл обмена сообщениями
        std::string message;
        while (true) {
            std::getline(std::cin, message);
            if (message == "exit") {
                break; 
            }
            boost::asio::write(socket, boost::asio::buffer(message));
        }

        reader_thread.detach();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
