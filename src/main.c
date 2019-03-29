#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include "../header-files/functions.h"
#include "../header-files/list.h"

int main(int argc,char **argv){
	struct_arguments *arguments;
	if(takeArguments(&arguments,argc,argv)==-1){
		return 1;
	}
//////////////////
	printf("ARGUMENTS\n");
	printf("id = %d...\n",arguments->id);
	printf("common_dir = %s...\n",arguments->common_dir);
	printf("input_dir = %s...\n",arguments->input_dir);
	printf("mirror_dir = %s...\n",arguments->mirror_dir);
	printf("buffer_size = %d...\n",arguments->buffer_size);
	printf("log_file = %s...\n",arguments->log_file);

	printf("getpid()=%d\n",getpid());
//////////////////

	if(checkAndCreateDirs(arguments)==-1){ // 1
		return 2;
	}

	if(createFileInCommon(arguments)==-1){ // 2
		return 3;
	}

	//leipei to bhma 3

	//lista me ta arxeia pou epikoinwnei
	list *l;
	initializeList(&l);

	//bazw to arxeio tou apo to common dir wste na to agnow
	char *temp;
	temp=malloc((countDigits(arguments->id)+strlen(".id")+1)*sizeof(char));
	memset(temp,0,countDigits(arguments->id)+strlen(".id")+1);
	sprintf(temp,"%d",arguments->id);
	strcat(temp,".id");

	insertList(l,temp);
	free(temp);
	printList(l);

	printf("\n\n\n\n\n");

	while(1){
		//anoigw ton common dir kai blepw ta arxeia
		//https://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program
		DIR *d;
		struct dirent *dir;
		d=opendir(arguments->common_dir);
		if(d){
			while((dir=readdir(d))!=NULL){
				if(strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0){
					printf("indir=%s...\n",dir->d_name);

					if(searchList(l,dir->d_name)==0){
						int pid;

						//gia to sender-child
						pid=fork();
						switch(pid){
							case -1:
								printf("\n\nFORK FAILED!!!\n\n\n");
								return 4;
							case 0://child
								//char *args[]={"./sender_child",NULL};
								//execvp(args[0],args);
								break;
							default://parent
								//array_of_processes[i]=pid;
								break;
						}

						//gia to receiver-child
					}
				}
			}
			closedir(d);
		}

		sleep(10);
	}

	destroyList(&l);
}











