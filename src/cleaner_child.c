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

void cleanSubdirFromMirror(char *path);

int main(int argc,char **argv){
	printf("HELLO WORLD CLEANER\n");

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
	char *id=malloc((strlen(argv[1])+1)*sizeof(char));//SE KA8E MALLOC KAI ENA MEMCPY !!!!
	strcpy(id,argv[1]);

	char *common_dir=malloc((strlen(argv[2])+1)*sizeof(char));
	strcpy(common_dir,argv[2]);

	char *input_dir=malloc((strlen(argv[3])+1)*sizeof(char));
	strcpy(input_dir,argv[3]);

	char *mirror_dir=malloc((strlen(argv[4])+1)*sizeof(char));
	strcpy(mirror_dir,argv[4]);

	char *buffer_size=malloc((strlen(argv[5])+1)*sizeof(char));
	strcpy(buffer_size,argv[5]);
	int b=atoi(buffer_size);

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

	char *path;
	path=malloc((strlen(mirror_dir)+strlen(other_id)+strlen("/")+1)*sizeof(char));
	memset(path,0,strlen(mirror_dir)+strlen(other_id)+strlen("/")+1);
	strcpy(path,mirror_dir);
	strcat(path,other_id);
	strcat(path,"/");
	printf("\n\npath to rm is %s...\n\n\n",path);

	cleanSubdirFromMirror(path);
}


void cleanSubdirFromMirror(char *directory_or_file){
	if(isDirectory(directory_or_file)>0){//is dir
		DIR *d;
		struct dirent *dir;
		d=opendir(directory_or_file);
		if(d){
			while((dir=readdir(d))!=NULL){
				if(strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0){
					printf("[[[dir->d_name=%s]]]\n",dir->d_name);
					char *next_dir;
					next_dir=malloc((strlen(directory_or_file)+strlen(dir->d_name)+strlen("/")+1)*sizeof(char));
					memset(next_dir,0,strlen(directory_or_file)+strlen(dir->d_name)+strlen("/")+1);
					strcpy(next_dir,directory_or_file);
					//strcat(next_dir,"/");
					strcat(next_dir,dir->d_name);
					if(isDirectory(next_dir)>0){
						strcat(next_dir,"/");
					}
					//sendFiles(fd,next_dir,b,log_file,useless_part);
					cleanSubdirFromMirror(next_dir);
				}
			}
		}
		closedir(d);
		rmdir(directory_or_file);
	}
	else{
		remove(directory_or_file);
	}
}





