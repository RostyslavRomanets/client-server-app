//
// Created by Rostik on 3/29/2024.
//

#include <iostream>
#include <exception>
#include <string>
#include <fstream>

#include "boost/asio.hpp"

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <filepath>" << std::endl;
        return 1;
    }

    std::string filePath = argv[1];

    try
    {
        boost::asio::io_context io_context;
        tcp::socket socket(io_context);

        boost::asio::streambuf requestBuf;

        std::array<char, 131072> buf {};

        boost::system::error_code ec;
        socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 7777), ec);

        if (ec)
            throw boost::system::system_error(ec);

        std::ifstream file(filePath, std::ios::binary);
        if (!file)
            throw std::runtime_error("Cannot open file: " + filePath);

        file.seekg(0, std::ifstream::end);
        auto fileSize = file.tellg();
        file.seekg(0, std::ifstream::beg);

        std::ostream requestStream(&requestBuf);
        auto fileName = filePath.substr(filePath.find_last_of("/\\") + 1);
        requestStream << fileName << "\n" << fileSize << "\n\n";

        boost::asio::write(socket, requestBuf, ec);
        if (ec)
            throw boost::system::system_error(ec);

        while (!file.eof())
        {
            file.read(buf.data(), buf.size());
            boost::asio::write(socket, boost::asio::buffer(buf), ec);
            if (ec)
                throw boost::system::system_error(ec);
        }
    }
    catch (const std::exception& ex)
    {
        std::cout << "Exception: " << ex.what() << std::endl;
    }
}