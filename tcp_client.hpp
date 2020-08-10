/*这是通用TCP客户端*/

#pragma once
#include "tcp_socket.hpp"

class TcpClient {
  public:
    TcpClient(const std::string &ip, uint16_t port) :
      ip_(ip),
      port_(port) {
        sock_.Socket();//要先创建好socket
      }

    ~TcpClient() {
      sock_.Close();
    }

    bool Connect() {
      return sock_.Connect(ip_, port_);
    }

    bool Recv(std::string* buf) {
      return sock_.Recv(buf);
    }

    bool Send(const std::string &buf) {
      return sock_.Send(buf);
    }
  private:
    std::string ip_;
    uint16_t port_;
    TcpSocket sock_;
};
