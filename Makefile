ifeq ($(OS), Windows_NT)
	CLIENT_BIN_OUTPUT = .\bin\client.exe
	SERVER_BIN_OUTPUT = .\bin\server.exe
	SERVER_LD_FLAGS = -lws2_32 -static-libstdc++
	CLIENT_LD_FLAGS = -lws2_32 -static-libstdc++
else
	CLIENT_BIN_OUTPUT = ./bin/client
	SERVER_BIN_OUTPUT = ./bin/server
	SERVER_LD_FLAGS =
	CLIENT_LD_FLAGS =
endif

CC = g++
CLIENT_FLAGS = -Wall -Wfatal-errors -std=c++20 -o $(CLIENT_BIN_OUTPUT)
CLIENT_SRC_FILES = src/client.cpp \
				src/network/*.cpp

SERVER_FLAGS = -Wall -Wfatal-errors -std=c++20 -o $(SERVER_BIN_OUTPUT)
SERVER_SRC_FILES = src/server.cpp \
				src/network/*.cpp

client-build:
	$(CC) $(CLIENT_FLAGS) $(CLIENT_SRC_FILES) $(CLIENT_LD_FLAGS)

client-run:
	$(CLIENT_BIN_OUTPUT)

client: client-build client-run

server-build:
	$(CC) $(SERVER_FLAGS) $(SERVER_SRC_FILES) $(SERVER_LD_FLAGS)

server-run:
	$(SERVER_BIN_OUTPUT)

server: server-build server-run