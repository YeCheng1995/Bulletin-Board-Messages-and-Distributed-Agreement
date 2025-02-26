COBJS=tinyxml2.o main.o network.o xml_parser.o data_handler.o
CC=g++
CFLAGS=-c -Wall -g

all:BulletinBoard client

BulletinBoard:$(COBJS)
	g++ $(COBJS) -o BulletinBoard -lpthread

client:client.o
	g++ client.o -o client -g

tinyxml2.o:./src/tinyxml2.cpp
	g++ ./src/tinyxml2.cpp -c -o tinyxml2.o -g

main.o:main.cpp
	g++ main.cpp -c -o main.o -g
network.o:./src/network.cpp
	g++ ./src/network.cpp -c -o network.o -lpthread -g
xml_parser.o:./src/xml_parser.cpp
	g++ ./src/xml_parser.cpp -c -o xml_parser.o -lpthread -g
data_handler.o:./src/data_handler.cpp
	g++ ./src/data_handler.cpp -c -o data_handler.o -lpthread -g

client.o:./example/client.cpp
	g++ ./example/client.cpp -c -o client.o

clean:
	rm -rf *.o