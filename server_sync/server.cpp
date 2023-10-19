/**
 * @file server.cpp
 * @author wangdong (wangdong1328@163.com)
 * @brief 同步通信服务器端
 * @version 0.1
 * @date 2023-10-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <iostream>
#include <boost/asio.hpp>
#include <memory>
#include <thread>
#include <set>
using namespace std;

using boost::asio::ip::tcp;

// 缓冲区大小
const int BUFFER_SIZE = 1024;

// 线程容器
std::set<std::shared_ptr<std::thread>> g_setThreads;

// 会话类
void Session(std::shared_ptr<tcp::socket> sock)
{
    try
    {
        char chBuffer[BUFFER_SIZE] = {0};

        for (;;)
        {
            memset(chBuffer, 0, sizeof(chBuffer));

            boost::system::error_code errcode;
            int iRecvRet = sock->read_some(boost::asio::buffer(chBuffer, BUFFER_SIZE), errcode);

            if (errcode == boost::asio::error::eof)
            {
                std::cout << "connection closed by peer" << endl;
                break;
            }
            else if (errcode)
            {
                throw boost::system::system_error(errcode);
            }

            cout << "Recv form " << sock->remote_endpoint().address().to_string() << " ,msg is: " << chBuffer << std::endl;

            // 回传信息值
            boost::asio::write(*sock, boost::asio::buffer(chBuffer, iRecvRet));
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void Server(boost::asio::io_context &ioc, unsigned short usPort)
{
    // 创建服务器acceptor 来接收客户
    tcp::acceptor acceptor(ioc, tcp::endpoint(tcp::v4(), usPort));

    for (;;)
    {
        std::shared_ptr<tcp::socket> sock(new tcp::socket(ioc));
        // 接收socket
        acceptor.accept(*sock);

        // 创建线程运行socket
        auto td = std::make_shared<std::thread>(Session, sock);
        g_setThreads.insert(td);
    }
}

int main(int argc, char *argv[])
{
    try
    {
        // 1. 创建io上下文
        boost::asio::io_context ioc;

        Server(ioc, 10086);

        for (auto &td : g_setThreads)
        {
            td->join();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}