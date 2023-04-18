#include "Kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc,char* argv[]){
	int p1[2],p2[2];
	if(pipe(p1)==-1||pipe(p2)==-1){
		fprintf(2,"pipe error\n");
		exit(1);
	}
	int pid=fork();
	if(pid<0){
		fprintf(2,"fork error\n");
		exit(1);
	}
	else if(pid==0){
		close(p1[1]);
		close(p2[0]);
		char buf;
		read(p1[0],&buf,1);
		printf("%d: received ping\n",getpid());
		close(p1[0]);
		write(p2[1],&buf,1);
		close(p2[1]);
		exit(0);
	}
	else{
		char byte='x';
		close(p1[0]);
		close(p2[1]);
		write(p1[1],&byte,1);
		close(p1[1]);
		wait(0);
		char buf;
		read(p2[0],&buf,1);
		close(p2[0]);
		printf("%d: received pong\n",getpid());
		exit(0);
	}
}
