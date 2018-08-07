.PHONY:all
all:	tcp_server tcp_client

tcp_server:tcp_server.c
	gcc -o $@ $^ -lpthread
tcp_client:tcp_client.c
	gcc -o $@ $^

clean:
	rm -f tcp_server tcp_client
.PHONY:clean
