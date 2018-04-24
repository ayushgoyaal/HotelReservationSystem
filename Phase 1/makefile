all:
	make server
	make client

server: server.cpp
	g++ -pthread -o server -I/usr/local/include -I/usr/local/include/cppconn -Wl,-Bdynamic server.cpp -lmysqlcppconn

client: client.cpp
	g++ client.cpp -o client

clean:
	rm server client
