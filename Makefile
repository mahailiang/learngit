all:
	gcc server.c databass.c history.c -o server -lpthread -lsqlite3
	gcc client.c -o client 
clean:
	rm server client



