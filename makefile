all: 
	gcc -g forkServ.c -o forkServ
	gcc -g selectServ.c -o selectServ
	gcc -g client.c -o client
clean:
	rm -rf *.o forkServ selectServ client
