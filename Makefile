

all: server client
	
client:
	gcc client.c -o client
	

server:
	gcc server.c -o server