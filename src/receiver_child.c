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


	//ftiaxnw to pipename dld to path + to arxeio id*_to_id*.fifo
	char *pipename=malloc((strlen(common_dir)+strlen(id)+strlen("_to_")+strlen(other_id)+strlen(".fifo")+1)*sizeof(char));
	memset(pipename,0,strlen(common_dir)+strlen(id)+strlen("_to_")+strlen(other_id)+strlen(".fifo")+1);
	strcpy(pipename,common_dir);
	strcat(pipename,other_id);
	strcat(pipename,"_to_");
	strcat(pipename,id);
	strcat(pipename,".fifo");

	//https://stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c
	if(access(pipename,F_OK)!=-1){// file exists
		//printf("%s already exists\n",pipename);
	}
	else{// file doesn't exist
		if(mkfifo(pipename,0666)==-1){
			if(errno!=EEXIST){
				perror("receiver:mkfifo_recv");
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


	FILE *f_log;
	f_log=fopen(log_file,"a");

	//arxika ftiaxnw sto mirror enan fakelo me to other_id wste na balw ekei mesa auta pou 8a mou steilei
	char *path_to_mirror;
	path_to_mirror=malloc((strlen(mirror_dir)+strlen(other_id)+strlen("/")+1)*sizeof(char));
	memset(path_to_mirror,0,strlen(mirror_dir)+strlen(other_id)+strlen("/")+1);
	strcpy(path_to_mirror,mirror_dir);
	strcat(path_to_mirror,other_id);
	strcat(path_to_mirror,"/");

	mkdir(path_to_mirror,0700);

	for(;;){
		unsigned short length_name;
		if(read(fd,&length_name,2)<0){// 1.
			perror("ERROR IN READING 2");
					exit(10);
		}

		if(length_name==0){//otan o sender steilei 00 tote stamataw na diabazw
			printf("TELOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOS\n");
			break;
		}

		char *path_name=malloc((length_name+1)*sizeof(char));
		memset(path_name,0,length_name+1);
		if(read(fd,path_name,length_name)<0){// 2.
			perror("ERROR IN READING 3");
					exit(10);
		}

		if(path_name[strlen(path_name)-1]=='/'){//is a dir
			char *next_dir;
			next_dir=malloc((strlen(path_to_mirror)+strlen(path_name)+1)*sizeof(char));
			memset(next_dir,0,strlen(path_to_mirror)+strlen(path_name)+1);
			strcpy(next_dir,path_to_mirror);
			strcat(next_dir,path_name);

			//ftiaxnw ton fakelo
			mkdir(next_dir,0700);

			free(next_dir);
			//bazw sto logfile ta bytes pou diabase gia thn metafora tou fakelou
			fprintf(f_log,"receive %d\n",2+length_name);
		}
		else{//is a file
			//https://www.tutorialspoint.com/c_standard_library/c_function_fopen.htm
			//create file
			char *next_file;
			FILE *fp;
			next_file=malloc((strlen(path_to_mirror)+strlen(path_name)+1)*sizeof(char));
			memset(next_file,0,strlen(path_to_mirror)+strlen(path_name)+1);
			strcpy(next_file,path_to_mirror);
			strcat(next_file,path_name);

			//ftiaxnw to arxeio
			fp=fopen(next_file,"w");

			unsigned int length_file;
			if(read(fd,&length_file,4)<0){// 3.
				perror("ERROR IN READING 4");
						exit(10);
			}

			char *string;
			string=malloc((length_file+1)*sizeof(char));
			memset(string,0,length_file+1);


			int bytes_transfered=0,tr;
			while(bytes_transfered<length_file){
				if(b<length_file-bytes_transfered){//exei panw apo b akoma
					if((tr=read(fd,&(string[bytes_transfered]),b))==-1){// 4.
						perror("ERROR IN READING 5");
								exit(10);
					}
					bytes_transfered+=tr;
				}
				else{
					if((tr=read(fd,&(string[bytes_transfered]),length_file-bytes_transfered))==-1){// 3.
						perror("ERROR IN READING 6");
								exit(10);
					}
					bytes_transfered+=tr;
				}
			}

			//gemizw to arxeio
			fprintf(fp, "%s",string);
			free(string);

			fclose(fp);
			//bazw sto logfile ta bytes pou diabase gia thn metafora tou arxeiou
			fprintf(f_log,"receive %s %d\n",next_file,2+length_name+4+length_file);

			free(next_file);
		}

		free(path_name);
	}
	//close pipe and remove file
	close(fd);
	remove(pipename);


	free(id);
	free(common_dir);
	free(input_dir);
	free(mirror_dir);
	free(buffer_size);
	free(log_file);
	free(other_id);
	free(pipename);
	free(path_to_mirror);

	exit(2);
}






