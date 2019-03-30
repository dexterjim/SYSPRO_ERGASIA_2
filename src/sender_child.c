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

void sendFiles();

int main(int argc,char **argv){
	printf("HELLO WORLD SENDER\n");

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
	char *pipename=malloc((strlen(common_dir)+strlen(id)+strlen("_to_")+strlen(other_id)+strlen(".fifo")+1)*sizeof(char));
	memset(pipename,0,strlen(common_dir)+strlen(id)+strlen("_to_")+strlen(other_id)+strlen(".fifo")+1);
	strcpy(pipename,common_dir);
	strcat(pipename,id);
	strcat(pipename,"_to_");
	strcat(pipename,other_id);
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
	if((fd=open(pipename,O_WRONLY))<0){
		perror("fifo open problem1");
				exit(3);
	}


	if(write(fd,argv[1],10)==-1){
		perror("ERROR IN WRITING");
				exit(10);
	}

	/*8A TO BALW SE MIA ANADROMIKH FUNCTION
	//https://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program
	DIR *d;
	struct dirent *dir;
	d=opendir(input_dir);
	if(d){
		//arxika stelnw ton fakelo input_dir kai mesa sto while ta subdirs tou
		
		while((dir=readdir(d))!=NULL){
			if(strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0){
				
			}
		}
	}
	*/
	//sendFiles(fd,input_dir);
}
/*
void sendFiles(int fd,char *dir){//pairnw to fd apo to pipe kai ton fakelo pou 8elw na metaferw
	unsigned short length;
	length=strlen(dir);
	printf("sendFiles %s %d\n",dir,length);
	char temp_length[2];
	sprintf(temp_length,"%d",length);

	if(write(fd,temp_length,2)==-1){
		perror("ERROR IN WRITING");
				exit(10);
	}
}

//ama einai as exw ena sendDir kai ena sendFile

*/











