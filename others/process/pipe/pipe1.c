#include<unistd.h>
#include<memory.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
int main()
{
	int pipe_fd[2];
	pid_t pid;
	char buf_r[100];
	char* p_wbuf;
	int r_num;
	memset(buf_r,0,sizeof(buf_r));
	if(pipe(pipe_fd)<0){
		printf("pipe create error\n");
		return -1;
	}
	if((pid=fork())==0){
		printf("\n");
	close(pipe_fd[1]);
	sleep(2);
	if((r_num=read(pipe_fd[0],buf_r,100))>0){
		printf("%d numbers read from be pipe is %s\n",r_num,buf_r);
	}
	close(pipe_fd[0]);
		exit(0);
	}else if(pid>0){
	close(pipe_fd[0]);
	if(write(pipe_fd[1],"Hello",5)!=-1)
		printf("parent write success!\n");
	if(write(pipe_fd[1]," Pipe",5)!=-1)
		printf("parent wirte2 succes!\n");
	close(pipe_fd[1]);
	sleep(3);
	waitpid(pid,NULL,0);
	exit(0);
	}
}
