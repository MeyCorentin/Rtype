
#include "../include/UDPclient.hpp"

UDPClient::UDPClient(boost::asio::io_context& io_context, const std::string& host, unsigned short port)
    : io_context_(io_context), socket_(io_context, udp::endpoint(udp::v4(), 0)), server_endpoint_(boost::asio::ip::address::from_string(host), port)
{
    start_listening();
}

void UDPClient::send(const std::string& message)
{
    this->socket_.send_to(boost::asio::buffer(message), server_endpoint_);
}

void UDPClient::start_listening()
{
    read_data();
}

void UDPClient::read_data()
{
    this->socket_.async_receive_from(
        boost::asio::buffer(this->recv_buffer_), this->server_endpoint_,
        [this](boost::system::error_code ec, std::size_t bytes_recvd) {
            if (!ec && bytes_recvd > 0) {
                std::cout << "Received from server: " << std::string(this->recv_buffer_.begin(), this->recv_buffer_.begin() + bytes_recvd) << std::endl;
            }
            read_data();
        });
}

UDPClient::~UDPClient()
{
    this->socket_.close();
}
