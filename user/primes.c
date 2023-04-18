#include "Kernel/types.h"
#include "Kernel/stat.h"
#include "user/user.h"

/*
 * fd为0时将标准输入重定向到p[0]
 * fd为1时将标准输出重定向到p[1]
 */
void redirect(int fd,int p[2]){
	close(fd);
	dup(p[fd]);
	close(p[0]);
	close(p[1]);
}

/*
 * 删掉当前素数的倍数
 */
void filter_out(int n){
	int x;
	while(read(0,&x,sizeof x)){
		if(x%n){
			write(1,&x,sizeof x);
		}
	}
}

/*
 * 获得当前的素数
 * 创建子进程，筛掉本层素数的倍数
 * 筛完返回父进程，获取新一轮的素数
 */

void filter_down(){
	int n;
	int p[2];
	while(read(0,&n,sizeof n)){
		fprintf(1,"prime %d\n",n);
		if(pipe(p)<0){
			fprintf(2,"pipe error\n");
			exit(1);
		}
		int pid=fork();
		if(pid<0){
			fprintf(2,"fork error\n");
			exit(1);
		}
		else if(pid==0){
			redirect(1,p);
			filter_out(n);
		}else{
			redirect(0,p);
			continue;
		}
	}
}

/*
 * 创建子进程，把待处理数据全部写到重定向后的标准输出
 * 返回父进程，进行筛数流程
 */
int main(){
	int p[2];
	if(pipe(p)<0){
		fprintf(2,"pipe error\n");
		exit(1);
	}
	int pid=fork();
	if(pid<0){
		fprintf(2,"fork error\n");
		exit(1);
	}
	else if(pid==0){
		redirect(1,p);
		for(int i=2;i<=35;i++)
			write(1,&i,sizeof i);
	}
	else{
		redirect(0,p);
		filter_down();
	}
	exit(0);
}
