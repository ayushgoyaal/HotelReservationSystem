all:
	make server
	make loadGen

server: server.cpp
	g++ -std=c++11 server.cpp -o server -lcpp_redis -ltacopie -lpthread -pthread

loadGen: loadGen.cpp
	g++ -pthread loadGen.cpp -o loadGen
