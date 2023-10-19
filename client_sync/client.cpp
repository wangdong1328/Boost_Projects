/**
 * @file server.cpp
 * @author wangdong (wangdong1328@163.com)
 * @brief 同步通信客户端
 * @version 0.1
 * @date 2023-10-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <iostream>
#include <boost/asio.hpp>
#include <string>
using namespace std;

using boost::asio::ip::tcp;

// 缓冲区大小
const int BUFFER_SIZE = 1024;

int main(int argc, char *argv[])
{
    try
    {
        // 1. 创建io上下文
        boost::asio::io_context ioc;

        // 2. 创建socket
        tcp::socket sock(ioc);

        // 3. 构建终端结点
        tcp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 10086);

        // 4. 连接服务器
        boost::system::error_code errcode = boost::asio::error::host_not_found;
        sock.connect(endpoint, errcode);

        // 连接错误后，错误码不为0
        if (errcode)
        {
            std::cout << "connect failed, error code is " << errcode.value() << ", message is"
                      << errcode.message() << std::endl;
            return -1;
        }

        // 连接成功

        // 循环接收发送消息，输入 bye 退出循环。
        char chBuffer[BUFFER_SIZE] = {0};

        while (true)
        {
            // 清空缓冲区
            memset(chBuffer, 0, sizeof(chBuffer));

            cout << "Please input message: ";
            cin.getline(chBuffer, BUFFER_SIZE);

            if (0 == string("bye").compare(chBuffer))
            {
                break;
            }

            // 发送消息
            int iSendRet = boost::asio::write(sock, boost::asio::buffer(chBuffer, strlen(chBuffer)));

            // 清空缓冲区
            memset(chBuffer, 0, sizeof(chBuffer));
            // 读取消息
            int iReadRet = boost::asio::read(sock, boost::asio::buffer(chBuffer, iSendRet));
            cout << "Recv Msg: " << chBuffer << endl;
        }

        sock.close();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}