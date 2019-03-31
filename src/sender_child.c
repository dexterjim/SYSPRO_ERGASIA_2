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

void sendFiles(int fd,char *directory_or_file,int b);

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
	printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA id = %s...\n",id);
	//open
	int fd;
	if((fd=open(pipename,O_WRONLY))<0){
		printf("QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ\n");
		perror("fifo open problem1");
				exit(3);
	}

	/*px
	if(write(fd,argv[1],10)==-1){
		perror("ERROR IN WRITING 1");
				exit(10);
	}
	*/

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
	printf("SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS id = %s...\n",id);
	sendFiles(fd,input_dir,b);
	//////////////////
	char temp_length_name[2];
	temp_length_name[0]='0';
	temp_length_name[1]='0';

	if(write(fd,temp_length_name,2)==-1){//1.
		perror("ERROR IN WRITING 2");
				exit(10);
	}
	//////////////////
}

//TO INPUTDIR NA EINAI XWRIS '/' STO TELOS

void sendFiles(int fd,char *directory_or_file,int b){//pairnw to fd apo to pipe kai ton fakelo pou 8elw na metaferw
	unsigned short length_name;
	length_name=strlen(directory_or_file);
	printf("sendFiles %s %d\n",directory_or_file,length_name);
	char temp_length_name[2];
	sprintf(temp_length_name,"%d",length_name);

	if(write(fd,temp_length_name,2)==-1){//1.
		perror("ERROR IN WRITING 3");
				exit(10);
	}

	if(write(fd,directory_or_file,length_name)==-1){//2.
		perror("ERROR IN WRITING 4");
				exit(10);
	}

	//elegxw an einai arxeio h dir
	if(isDirectory(directory_or_file)>0){//an einai dir , tom anoigw kai kalw thn synarthsh gia oti exei mesa
		printf("sendFiles->dir\n");
////////////////////////////////////////////
		//https://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program
		DIR *d;
		struct dirent *dir;
		d=opendir(directory_or_file);
		if(d){
			while((dir=readdir(d))!=NULL){
				if(strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0){
					printf("[[[dir->d_name=%s]]]\n",dir->d_name);
					char *next_dir;
					next_dir=malloc((strlen(directory_or_file)+strlen("/")+strlen(dir->d_name)+1)*sizeof(char));
					memset(next_dir,0,strlen(directory_or_file)+strlen("/")+strlen(dir->d_name)+1);
					strcpy(next_dir,directory_or_file);
					strcat(next_dir,"/");
					strcat(next_dir,dir->d_name);
					sendFiles(fd,next_dir,b);
				}
			}
		}
////////////////////////////////////////////
	}
	else{//alliws ektelw ta bhmata 3,4
		printf("sendFiles->file\n");

		//https://stackoverflow.com/questions/14002954/c-programming-how-to-read-the-whole-file-contents-into-a-buffer
		FILE *f=fopen(directory_or_file,"rb");
		fseek(f,0,SEEK_END);
		//long fsize=ftell(f);
		int fsize=ftell(f);
		fseek(f,0,SEEK_SET);

		char *string=malloc((fsize+1)*sizeof(char));
		fread(string,fsize,1,f);
		fclose(f);

		string[fsize]=0;

		/////////////////////////////////////////////////////
		char temp_length_file[4];
		memset(temp_length_file,0,4);
		sprintf(temp_length_file,"%d",fsize);

		printf("fsize=%d temp_length_file=%s...\n",fsize,temp_length_file);

		if(write(fd,temp_length_file,4)==-1){//3.
			perror("ERROR IN WRITING 5");
					exit(10);
		}


		//4.
		int bytes_transfered=0,tr;
		while(bytes_transfered<fsize){
			if(b<fsize-bytes_transfered){//exei panw apo b akoma
				if((tr=write(fd,&(string[bytes_transfered]),b))==-1){//4.
					perror("ERROR IN WRITING 6");
							exit(10);
				}
				bytes_transfered+=tr;
			}
			else{
				if((tr=write(fd,&(string[bytes_transfered]),fsize-bytes_transfered))==-1){//4.
					perror("ERROR IN WRITING 7");
							exit(10);
				}
				bytes_transfered+=tr;
			}
			printf("\t\t\ttr=%d",tr);
		}
		printf("\t\t\tsender done with file\n");
	}

	/*
	//https://stackoverflow.com/questions/14002954/c-programming-how-to-read-the-whole-file-contents-into-a-buffer
	FILE *f=fopen(directory_or_file,"rb");
	fseek(f,0,SEEK_END);
	long fsize=ftell(f);
	fseek(f,0,SEEK_SET);

	char *string=malloc((fsize+1)*sizeof(char));
	fread(string,fsize,1,f);
	fclose(f);

	string[fsize]=0;
	*/
}

//ama einai as exw ena sendDir kai ena sendFile

/*
int isDirectory(const char *path){
	//https://stackoverflow.com/questions/4553012/checking-if-a-file-is-a-directory-or-just-a-file
	struct stat statbuf;
	if (stat(path, &statbuf) != 0)
		return 0;
	return S_ISDIR(statbuf.st_mode);
}
*/


