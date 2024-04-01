#include <iostream>
#include "boost/asio.hpp"

#include <syncstream>
#include <fstream>

using boost::asio::ip::tcp;

void session(tcp::socket socket)
{
    try
    {
        std::osyncstream(std::cout) << "Client connected." << std::endl;

        std::string fileName;
        uint32_t fileSize;

        std::array <char, 131072> buf {};

        // Reading file name and file size of format: filename \n filesize \n\n
        boost::asio::streambuf requestBuf;
        boost::system::error_code ec;
        auto bytes = boost::asio::read_until(socket, requestBuf, "\n\n", ec);

        if (ec)
            throw boost::system::system_error(ec);

        std::istream requestStream(&requestBuf);
        requestStream >> fileName >> fileSize;
        requestStream.read(buf.data(), 2);

        // Creating file
        std::ofstream file(fileName, std::ios::binary);
        if (!file)
            throw std::runtime_error("Cannot create file: " + fileName);

        do {
            requestStream.read(buf.data(), buf.size());
            file.write(buf.data(), requestStream.gcount());
        } while (requestStream.gcount() > 0);


        // Reading remaining file content
        while (true)
        {
            bytes = socket.read_some(boost::asio::buffer(buf), ec);

            if (bytes > 0)
            {
                file.write(buf.data(), bytes);
            }

            if (ec == boost::asio::error::eof)
            {
                std::osyncstream(std::cout) << "Connection closed" << std::endl;
                return;
            }
            else if (ec)
                throw boost::system::system_error(ec);
        }
    }
    catch (const std::exception& ex)
    {
        std::osyncstream(std::cout) << "Worker exception: " << ex.what() << std::endl;
    }
}

int main() {
    try
    {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 7777));

        while (true)
        {
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            std::thread(session, std::move(socket)).detach();
        }
    }
    catch (const std::exception& ex)
    {
        std::osyncstream(std::cout) << "Exception: " << ex.what() << std::endl;
    }

    return 0;
}
