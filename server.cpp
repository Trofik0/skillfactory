#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "database.hpp"

using boost::asio::ip::tcp;

class ChatServer {
public:
    ChatServer(short port);
    void run();

private:
    void start_accept();
    void handle_client(tcp::socket socket);

    boost::asio::io_service io_service_;
    tcp::acceptor acceptor_;

    Database database_{"chat.db"};
};

ChatServer::ChatServer(short port) : acceptor_(io_service_, tcp::endpoint(tcp::v4(), port)) {
    start_accept();
}

void ChatServer::run() {
    io_service_.run();
}

void ChatServer::start_accept() {
    tcp::socket socket(io_service_);
    acceptor_.async_accept(socket,
        [this, socket = std::move(socket)](const boost::system::error_code& error) mutable {
            if (!error) {
                std::thread(&ChatServer::handle_client, this, std::move(socket)).detach();
            }
            start_accept();
        });
}

void ChatServer::handle_client(tcp::socket socket) {
    try {
        char data[1024];
        std::string username;

        // Регистрация или вход в систему
        boost::asio::read(socket, boost::asio::buffer(data, 1024));
        std::string command(data);
        if (command.substr(0, 9) == "REGISTER:") {
            std::string credentials = command.substr(9);
            size_t delimiter = credentials.find(':');
            std::string new_username = credentials.substr(0, delimiter);
            std::string new_password = credentials.substr(delimiter + 1);
            if (database_.register_user(new_username, new_password)) {
                boost::asio::write(socket, boost::asio::buffer("User registered successfully!"));
            } else {
                boost::asio::write(socket, boost::asio::buffer("Error registering user."));
            }
        } else if (command.substr(0, 5) == "LOGIN:") {
            std::string credentials = command.substr(5);
            size_t delimiter = credentials.find(':');
            username = credentials.substr(0, delimiter);
            std::string password = credentials.substr(delimiter + 1);
            if (!database_.authenticate_user(username, password)) {
                boost::asio::write(socket, boost::asio::buffer("Login failed: Invalid username or password."));
                return; // Неверные учетные данные
            }
        }

        std::cout << username << " has joined the chat." << std::endl;
        boost::asio::write(socket, boost::asio::buffer("Welcome " + username + "! You can start chatting now."));

        while (true) {
            size_t length = boost::asio::read(socket, boost::asio::buffer(data, 1024));
            std::string message(data, length);
            std::cout << username << ": " << message << std::endl;
            database_.save_message(username, message);
        }
    } catch (std::exception& e) {
        std::cerr << "Exception in client handler: " << e.what() << std::endl;
    }
}

int main() {
    try {
        ChatServer server(12345);
        std::cout << "Chat server started on port 12345." << std::endl;
        server.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
