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
