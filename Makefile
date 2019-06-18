CC = gcc

CFLAGS = -g -Wall

SERVER = p4server
CLIENT = p4client

default: all

all: $(SERVER) $(CLIENT)

$(SERVER): $(SERVER).c
	$(CC) $(CFLAGS) -o $(SERVER) $(SERVER).c
$(CLIENT): $(CLIENT).c
	$(CC) $(CFLAGS) -o $(CLIENT) $(CLIENT).c

clean:
	$(RM) $(SERVER)
	$(RM) $(CLIENT)