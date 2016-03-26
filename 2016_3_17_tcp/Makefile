.PHONY:all
all:tcp_client tcp_server
tcp_client:tcp_client.c
	gcc -o $@ $^
tcp_server:tcp_server.c
	gcc -o $@ $^ -lpthread -D_FUN3_
.PHONY:clean
clean:
	rm -rf tcp_client tcp_server
