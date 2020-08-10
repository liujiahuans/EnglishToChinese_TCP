#include <unordered_map>
//#include "tcp_server.hpp"
//#include "tcp_process_server.hpp"
#include "tcp_thread_server.hpp"

std::unordered_map<std::string, std::string> g_dict;

void Translate(const std::string &req, std::string *resp) {
  auto it = g_dict.find(req);
  if (it == g_dict.end()) {
    *resp = "Not Found~";
    return;
  }
  *resp = it->second;
  return;
}

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Usage ./dict_server [ip] [port]\n");
    return 1;
  }
  //1、初始化词典
  g_dict.insert(std::make_pair("abandon", "放弃，抛弃"));
  g_dict.insert(std::make_pair("believe", "相信"));
  g_dict.insert(std::make_pair("cut", "切"));
  //2、启动服务器
  //TcpServer server(argv[1], atoi(argv[2]));
  //TcpProcessServer process_server(argv[1], atoi(argv[2]));
  TcpThreadServer thread_server(argv[1], atoi(argv[2]));

  //server.Start(Translate);
  //process_server.Start(Translate);
  thread_server.Start(Translate);
  return 0;
}
