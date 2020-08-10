.PHONY:all
 all:dict_server dict_client

dict_client:dict_client.cpp
	g++ -std=c++11 -lpthread $^ -o $@
dict_server:dict_server.cpp
	g++ -std=c++11 -lpthread $^ -o $@

.PHONY:clean
clean:
	rm -f dict_server dict_client
