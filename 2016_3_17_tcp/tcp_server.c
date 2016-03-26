#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<sys/wait.h>
#include<pthread.h>
#define _BACK_LOG_ 5
int startup(const char* ip,unsigned short port)
{
        int listen_sock=socket(AF_INET,SOCK_STREAM,0);
	if(listen_sock==-1)
	{
		printf("%d : %s\n",errno,strerror(errno));
		exit(1);
	}
	struct sockaddr_in local;
	local.sin_family=AF_INET;//IPV4
	local.sin_port=htons(port);
	inet_aton(ip,&local.sin_addr);
	if(bind(listen_sock,(struct sockaddr*)&local,sizeof(local))==-1)
	{	

		printf("%d : %s\n",errno,strerror(errno));
		exit(1);
	}
	if(listen(listen_sock,_BACK_LOG_)==-1){

		printf("%d : %s\n",errno,strerror(errno));
		exit(1);
	}
	return listen_sock;
	//close(listen_sock);
}
static void wait_child(int signo)
{
	pid_t exit_id;
	while((exit_id=waitpid(-1,NULL,WNOHANG))>0){
		printf("child exited,pid is: %d\n",exit_id);
	}
}
void *thread_run(void* arg)
{
	printf("create a new thread,thread_id is: %d\n",pthread_self());
	int new_client=(int)arg;
	char buf[1024];
	while(1){
		memset(buf,'\0',sizeof(buf)-1);
		ssize_t _size=read(new_client,buf,sizeof(buf)-1);
		if(_size<0){
			perror("read");
			break;
		}else if(_size==0){
			printf("client release!\n");
			break;
		}else{
			buf[_size]='\0';
			printf("client :-> %s",buf);
		}
	}
	close(new_client);
}
void usage(char* proc)
{
	printf("Usage:%s [IP] [PROC]\n",proc);
}
int main(int argc,char* argv[])
{
	if(argc!=3){
		usage(argv[0]);
		exit(1);
	}
	signal(SIGCHLD,wait_child);
	char* ip=argv[1];
	int port =atoi(argv[2]);
	struct sockaddr_in client_info;
	socklen_t client_len=0;
	int listen_sock=startup(ip,port);
	while(1){
		int new_client=accept(listen_sock,(struct sockaddr*)&client_info,&client_len);
		if(new_client<0){
			perror("accept");
			continue;
		}
		printf("get a new connect...\n");
#ifdef _FUN1_
		char buf[1024];
		memset(buf,'\0',sizeof(buf));
		while(1){
			ssize_t _size=read(new_client,buf,sizeof(buf)-1);
			if(_size<0){
				perror("read");
				break;
			}else if(_size==0){
				printf("client release\n");
				break;
			}else{
				buf[_size]='\0';
				printf("client :->%s\n",buf);
			}
		}
#endif
#ifdef _FUN2_
	printf("many procs,use fun2...\n");
	pid_t id=fork();
	if(id<0){
		perror("fork");
	}else if(id==0){
		close(listen_sock);
		char buf[1024];
		while(1){
			memset(buf,'\0',sizeof(buf)-1);
			ssize_t _size=read(new_client,buf,sizeof(buf)-1);
			if(_size<0){
				perror("read");
				break;
			}else if(_size==0){
				printf("client release\n");
			}else{
				buf[_size]='\0';
				printf("client :->%s",buf);
			}
		}
		close(new_client);
		exit(1);
	}
	close(bew_client);
#endif
#ifdef _FUN3_
	printf("thread,use fun3...\n");
	pthread_t t_id;
	pthread_create(&t_id,NULL,thread_run,(void*)new_client);
	pthread_detach(t_id);
#endif
	}
	return 0;
}
