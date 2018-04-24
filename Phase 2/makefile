all:
	make server
	make loadGen

server: server.cpp
	g++ -pthread -o server -I/usr/local/include -I/usr/local/include/cppconn -Wl,-Bdynamic server.cpp -lmysqlcppconn

loadGen: loadGen.cpp
	g++ -pthread loadGen.cpp -o loadGen
