CC = gcc
CFLAGS = -Wall -g

# Executables
client: client.o utils/socket-library.o utils/custom-utilities.o
	$(CC) $(CFLAGS) $^ -o $@

server: server.o utils/socket-library.o utils/custom-utilities.o
	$(CC) $(CFLAGS) $^ -o $@

# Object Files
client.o: client.c utils/socket-library.h
	$(CC) $(CFLAGS) -c $< -o $@

server.o: server.c utils/socket-library.h
	$(CC) $(CFLAGS) -c $< -o $@

utils/socket-library.o: utils/socket-library.c utils/socket-library.h utils/custom-utilities.h
	$(CC) $(CFLAGS) -c $< -o $@

utils/custom-utilities.o: utils/custom-utilities.c utils/custom-utilities.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -f client server client.o server.o utils/*.o
