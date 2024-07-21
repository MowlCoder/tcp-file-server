CLIENT_BIN_OUTPUT = ./bin/client
SERVER_BIN_OUTPUT = ./bin/server

CC = g++
CLIENT_FLAGS = -Wall -Wfatal-errors -std=c++20 -o $(CLIENT_BIN_OUTPUT)
CLIENT_SRC_FILES = src/client.cpp \
				src/network/*.cpp

SERVER_FLAGS = -Wall -Wfatal-errors -std=c++20 -o $(SERVER_BIN_OUTPUT)
SERVER_SRC_FILES = src/server.cpp \
				src/network/*.cpp

client:
	$(CC) $(CLIENT_FLAGS) $(CLIENT_SRC_FILES) && $(CLIENT_BIN_OUTPUT)

server:
	$(CC) $(SERVER_FLAGS) $(SERVER_SRC_FILES) && $(SERVER_BIN_OUTPUT)