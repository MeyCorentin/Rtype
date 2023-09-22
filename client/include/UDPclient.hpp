#ifndef UDPCLIENT_HPP
#define UDPCLIENT_HPP

#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>

using boost::asio::ip::udp;

class UDPClient {
public:
    UDPClient(boost::asio::io_context& io_context, const std::string& host, unsigned short port);
    ~UDPClient();

    void send(const std::string& message);
    void start_listening();

private:
    void read_data();

    boost::asio::io_context& io_context_;
    udp::socket socket_;
    udp::endpoint server_endpoint_;
    boost::array<char, 1024> recv_buffer_;
};

#endif
