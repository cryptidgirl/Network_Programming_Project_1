CC = gcc
CFLAGS = -g -Wall -pthread
INCLUDE1=DieWithError.c CaesarCipher.c Server.c
INCLUDE2=DieWithError.c Client.c CaesarCipher.c
INCLUDE3=DieWithError.c Client.c

all: EchoClientTCP EchoServerTCP-Forked-Num CaesarCipher

Client: Client.c
	$(CC) $(CFLAGS) $(INCLUDE1) -o $@.o $<

Server: Server.c
	$(CC) $(CFLAGS) $(INCLUDE2) -o $@.o $<

Application: Application.c
	$(CC) $(CFLAGS) $(INCLUDE3) -o $@.o $<

clean:
	-rm -f *.o *~ *core* 
