#include "Reader.hpp"



Reader::Reader(int port) : _socket(_ioContext, asio::ip::udp::endpoint(asio::ip::udp::v4(), port))
{
    
}

Reader::~Reader()
{
}

void Reader::Clock(std::vector<std::unique_ptr<Client>> &clients)
{
    asio::ip::udp::endpoint sender;
    bool pass = false;
    enum { max_length = 1024 };
    char data[max_length];

    while (true) {
        pass = false;
        std::error_code ec;
        std::size_t length = this->_socket.receive_from(asio::buffer(data, max_length), sender, 0, ec);

        for (auto i = clients.begin(); i != clients.end(); i++) {
            if ((*i)->GetEndpoint() == sender) {
                std::cout << "client already exist" << std::endl;
                pass = true;
            }
        }

        if (!pass)
            clients.push_back(std::make_unique<Client>(sender));
        if (ec) {
            std::cerr << "Error during receive: " << ec.message() << std::endl;
            continue;
        }

        this->_socket.send_to(asio::buffer(data, length), sender);
    }
}