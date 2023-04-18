#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
find(const char *path,const char* target)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }
	if(st.type!=T_DIR){
		fprintf(2,"find: %s is not a directory\n",path);
		close(fd);
		exit(1);
	}
	if(strlen(path) + 1 + DIRSIZ +1 > sizeof(buf)) {
  	printf("find: path too long\n");
  }

  strcpy(buf, path);
  p = buf+strlen(buf);
  *p++ = '/';
  while(read(fd, &de, sizeof(de)) == sizeof(de)) {
    if(de.inum == 0) continue;
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    if(stat(buf,&st) < 0){
    	printf("find : cannot stat %s\n",buf);
      continue;
    }

    if(strcmp(p,target) == 0){
      printf("%s\n",buf);
    }
		else if(st.type == T_DIR && strcmp(p,".") !=0 && strcmp(p,"..") !=0){
      find(buf,target);//recursion
    }
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc < 2 || argc > 3){
    fprintf(2,"Usage: find <path> <target>\n");
    exit(1);
  }
	if(argc==2) find(".",argv[1]);
	else find(argv[1],argv[2]);
  exit(0);
}
