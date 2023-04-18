#include "Kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"


int main(int argc,char* argv[]){
	if(argc>=MAXARG){
		fprintf(2,"xargs: too many arguments\n");
		exit(1);
	}
	if(argc<2){
		fprintf(2,"Usage: %s <arg1> <arg2> ...\n",argv[0]);
		exit(1);
	}
	char *ags[MAXARG];
	for(int i=1;i<argc;i++) ags[i-1]=argv[i];
	int ub=argc-1;
	char buf[105];
	int eol=0;
	int i=0;
	while(read(0,&buf[i],sizeof(char))){
		if(buf[i]=='\n')  eol=1;
		if(buf[i]==' '||eol){
			buf[i]=0;
			ags[ub]=buf;
			ub++;
			i=0;
		}
		i++;
		if(eol){
			eol-=1;ags[ub]=0;ub=argc-1;
			int pid=fork();
			if(pid<0){
				fprintf(2,"fork error\n");
				exit(1);
			}else if(!pid){
				exec(ags[0],ags);
			}else{
				wait(0);
			}
		}
	}
	exit(0);
}

			

