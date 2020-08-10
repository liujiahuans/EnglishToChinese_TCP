/*这是英译汉客户端*/

#include <iostream>
#include "tcp_client.hpp"

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Usage ./dict_client [ip] [port]\n");
    return 1;
  }
  TcpClient client(argv[1], atoi(argv[2]));
  bool ret = client.Connect();
  if (!ret) {
    return 1;
  }

  while (1) {
    std::cout << "Please Enter The Word: ";
    std::string word;
    std::cin >> word;
    if(!std::cin) {
      break;
    }
    client.Send(word);
    std::string result;
    client.Recv(&result);
    std::cout << ' ' << result << std::endl;
  }
  return 0;
}
