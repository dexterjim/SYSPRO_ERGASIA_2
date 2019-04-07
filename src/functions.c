#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../header-files/functions.h"
#include "../header-files/list.h"

int takeArguments(struct_arguments **arguments,int argc,char **argv){
	int flag_n=0,flag_c=0,flag_i=0,flag_m=0,flag_b=0,flag_l=0;
	if(argc!=13){
		printf("\n\nWRONG NUMBER OF ARGUMENTS !!!\n\n\n");
		return -1;
	}
	(*arguments)=malloc(sizeof(struct_arguments));
	for(int i=1;i<argc;i++){
		if(strcmp(argv[i],"-n")==0){
			(*arguments)->id=atoi(argv[i+1]);
			flag_n++;
		}
		if(strcmp(argv[i],"-c")==0){
			(*arguments)->common_dir=malloc((strlen(argv[i+1])+1)*sizeof(char));
			strcpy((*arguments)->common_dir,argv[i+1]);
			flag_c++;
		}
		if(strcmp(argv[i],"-i")==0){
			(*arguments)->input_dir=malloc((strlen(argv[i+1])+1)*sizeof(char));
			strcpy((*arguments)->input_dir,argv[i+1]);
			flag_i++;
		}
		if(strcmp(argv[i],"-m")==0){
			(*arguments)->mirror_dir=malloc((strlen(argv[i+1])+1)*sizeof(char));
			strcpy((*arguments)->mirror_dir,argv[i+1]);
			flag_m++;
		}
		if(strcmp(argv[i],"-b")==0){
			(*arguments)->buffer_size=atoi(argv[i+1]);
			flag_b++;
		}
		if(strcmp(argv[i],"-l")==0){
			(*arguments)->log_file=malloc((strlen(argv[i+1])+1)*sizeof(char));
			strcpy((*arguments)->log_file,argv[i+1]);
			flag_l++;
		}
	}
	if(flag_n!=1 || flag_c!=1 || flag_i!=1 || flag_m!=1 || flag_b!=1 || flag_l!=1){
		return -1;
	}
	return 0;
}

int checkAndCreateDirs(struct_arguments *arguments){
	//https://stackoverflow.com/questions/7430248/creating-a-new-directory-in-c
	struct stat st = {0};

	//elegxw an uparxei to input_dir , an den uparxei emfanizw mnm la8ous
	if(stat(arguments->input_dir,&st)==-1){
		printf("\n\ninput_dir Directory does not exist !!!\n\n");
		return -1;
	}

	//elegxw an uparxei to mirror_dir , an uparxei emfanizw mnm la8ous
	if(stat(arguments->mirror_dir,&st)==0){
		printf("\n\nmirror_dir Directory does exist !!!\n\n");
		return -1;
	}
	else{//to dhmiourgw
		mkdir(arguments->mirror_dir,0700);
	}

	//dhmiourgw to common_dir
	if(stat(arguments->common_dir,&st)==-1){
		mkdir(arguments->common_dir,0700);//GENIKA ME TA mkdir PREPEI NA FTIAXNW TO PATH , EDW 8A TA EXW ETOIMA
	}

	return 0;
}

int createFileInCommon(struct_arguments *arguments){//ta dirs prepei na dinontai me / sto telos
	char *idfile;

	//ftiaxnw to idfile dld to path + to arxeio *.id
	idfile=malloc((strlen(arguments->common_dir)+countDigits(arguments->id)+strlen(".id")+1)*sizeof(char));
	memset(idfile,0,strlen(arguments->common_dir)+countDigits(arguments->id)+strlen(".id")+1);
	strcpy(idfile,arguments->common_dir);
	char *temp;
	temp=malloc((countDigits(arguments->id)+1)*sizeof(char));
	memset(temp,0,countDigits(arguments->id)+1);
	sprintf(temp,"%d",arguments->id);
	strcat(idfile,temp);
	free(temp);
	strcat(idfile,".id");

	printf("path file=%s...\n",idfile);

	//////////
	//https://stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c
	if(access(idfile,F_OK)!=-1){
		printf("\n\nThere is another client with the same id !!!\n\n\n");
		return -1;
	}
	//////////

	//anoigw to arxeio
	FILE *f;
	f=fopen(idfile,"w");

	//grafw to id mesa se auto
	fprintf(f,"%d",getpid());

	//kleinw to arxeio
	fclose(f);//GENIKA NA ELEGXW AN KLEINW SWSTA OLA TA ARXEIA

	free(idfile);

	return 0;
}

int createLogFile(struct_arguments *arguments){
	//anoigw to arxeio
	FILE *f;
	f=fopen(arguments->log_file,"w");

	//grafw to id mesa se auto
	fprintf(f,"id %d\n",arguments->id);

	//kleinw to arxeio
	fclose(f);//GENIKA NA ELEGXW AN KLEINW SWSTA OLA TA ARXEIA
	return 0;
}

int countDigits(int x){
	int c=0;
	while(x>0){
		c++;
		x=x/10;
	}
	return c;
}


int isDirectory(const char *path){
	//https://stackoverflow.com/questions/4553012/checking-if-a-file-is-a-directory-or-just-a-file
	struct stat statbuf;
	if (stat(path, &statbuf) != 0)
		return 0;
	return S_ISDIR(statbuf.st_mode);
}

void cleanDirOrFile(char *directory_or_file){
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
					cleanDirOrFile(next_dir);
					free(next_dir);
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
