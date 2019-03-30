#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include "../header-files/functions.h"
#include "../header-files/list.h"

int main(int argc,char **argv){
	printf("HELLO WORLD RECEIVER\n");

	struct_arguments *arguments;
	

	//////////////////
	printf("arg - id = %s...\n",argv[1]);
	printf("arg - common_dir = %s...\n",argv[2]);
	printf("arg - input_dir = %s...\n",argv[3]);
	printf("arg - mirror_dir = %s...\n",argv[4]);
	printf("arg - buffer_size = %s...\n",argv[5]);
	printf("arg - log_file = %s...\n",argv[6]);

	printf("arg - other_id = %s...\n",argv[7]);

	printf("getpid()=%d\n",getpid());
	//////////////////

	//////////////////
	char *id=malloc((strlen(argv[1])+1)*sizeof(char));
	strcpy(id,argv[1]);

	char *common_dir=malloc((strlen(argv[2])+1)*sizeof(char));
	strcpy(common_dir,argv[2]);

	char *input_dir=malloc((strlen(argv[3])+1)*sizeof(char));
	strcpy(input_dir,argv[3]);

	char *mirror_dir=malloc((strlen(argv[4])+1)*sizeof(char));
	strcpy(mirror_dir,argv[4]);

	char *buffer_size=malloc((strlen(argv[5])+1)*sizeof(char));
	strcpy(buffer_size,argv[5]);

	char *log_file=malloc((strlen(argv[6])+1)*sizeof(char));
	strcpy(log_file,argv[6]);

	char *other_id=malloc((strlen(argv[7])+1)*sizeof(char));
	strcpy(other_id,argv[7]);
	//////////////////

	//////////////////
	printf("id = %s...\n",id);
	printf("common_dir = %s...\n",common_dir);
	printf("input_dir = %s...\n",input_dir);
	printf("mirror_dir = %s...\n",mirror_dir);
	printf("buffer_size = %s...\n",buffer_size);
	printf("log_file = %s...\n",log_file);

	printf("other_id = %s...\n",other_id);
	//////////////////

	//ftiaxnw to pipename dld to path + to arxeio id*_to_id*.fifo
	char *pipename=malloc((strlen(common_dir)+strlen(id)+strlen("_to_")+strlen(other_id)+strlen(".fifo")+1)*sizeof(char));//einai strlen opote den mas endiaferei h seira
	memset(pipename,0,strlen(common_dir)+strlen(id)+strlen("_to_")+strlen(other_id)+strlen(".fifo")+1);
	strcpy(pipename,common_dir);
	strcat(pipename,other_id);
	strcat(pipename,"_to_");
	strcat(pipename,id);
	strcat(pipename,".fifo");

	printf("pipename file=%s...\n",pipename);

	//https://stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c
	if(access(pipename,F_OK)!=-1){// file exists
		printf("%s already exists\n",pipename);
	}
	else{// file doesn't exist
		if(mkfifo(pipename,0666)==-1){
			if(errno!=EEXIST){
				perror("receiver:mkfifo_recv");//8elei allagh?
				exit(6);
			}
		}
	}

	//open
	int fd;
	if((fd=open(pipename,O_RDONLY))<0){
		perror("fifo open problem1");
				exit(3);
	}

	
	char *str=malloc(100*sizeof(char));
	if(read(fd,str,10)<0){
		perror("ERROR IN WRITING");
				exit(10);
	}
	printf("\n\n\nMESSAGE IS %s...\n\n\n",str);
	

	
}














