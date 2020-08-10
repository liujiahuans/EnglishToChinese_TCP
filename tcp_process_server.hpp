#pragma once
#include <functional>
#include <signal.h>
#include "tcp_socket.hpp"

typedef std::function<void (const std::string& req, std::string* resp)> Handler;

//多进程版本的TCP服务器
class TcpProcessServer {
  public:
    TcpProcessServer(const std::string &ip, uint16_t port) :
      ip_(ip),
      port_(port) {
        //需要处理子进程
        signal(SIGCHLD, SIG_IGN);
      }

    void ProcessConnect(const TcpSocket &new_sock, const std::string &ip, uint16_t port, Handler handler) {
      int ret = fork();
      if (ret > 0) {
        //father
        //父进程不需要做额外的操作，直接返回即可
        //如果使用wait， 会导致父进程不能快速再次调用到accept， 仍然无法处理多个请求
        //这里父进程需要关闭new_sock
        new_sock.Close();
        return;
      }else if (ret == 0) {
        //child
        //处理具体的连接过程，每个连接一个子进程
        for ( ; ; ) {
          std::string req;
          bool ret = new_sock.Recv(&req);
          if (!ret) {
            //当前请求处理完了，可以推出子进程了，socket的关闭在析构函数中就完成了
            printf("[client %s : %d] disconnected!\n", ip.c_str(), port);
            exit(0);
          }
          std::string resp;
          handler(req, &resp);
          new_sock.Send(resp);
          printf("[vlient %s : %d] req: %s, resp : %s\n", ip.c_str(), port, req.c_str(), resp.c_str());
        }
      } else {
        perror("fork");
      }
    }

    bool Start(Handler handler) {
      //1、创建socket
      CHECK_RET(listen_sock_.Socket());
      //2、绑定端口号
      CHECK_RET(listen_sock_.Bind(ip_, port_));
      //3、进行监听
      CHECK_RET(listen_sock_.Listen(5));
      //4、进入事件循环
      for ( ; ; ) {
        //5、进行accept
        TcpSocket new_sock;
        std::string ip;
        uint16_t port = 0;
        if (!listen_sock_.Accept(&new_sock, &ip, &port)) {
          continue;
        }
        printf("[client %s : %d] connect!\n", &ip, port);
        ProcessConnect(new_sock, ip, port, handler);
      }
      return true;
    }
  private:
    std::string ip_;
    uint64_t port_;
    TcpSocket listen_sock_;
};
