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

///////////////////////////////////////
	FILE *f_log;
	f_log=fopen(log_file,"a");
/////////////////////////////////////////

	//ARXIKA PREPEI NA FTIAKSW STO MIRROR ENAN FAKELO ME TO OTHER_ID WSTE NA BALW EKEI MESA AYTA POU 8A MOU STEILEI
	char *path_to_mirror;
	path_to_mirror=malloc((strlen(mirror_dir)+strlen(other_id)+strlen("/")+1)*sizeof(char));
	memset(path_to_mirror,0,strlen(mirror_dir)+strlen(other_id)+strlen("/")+1);
	strcpy(path_to_mirror,mirror_dir);
	strcat(path_to_mirror,other_id);
	strcat(path_to_mirror,"/");

	printf("path_to_mirror=%s...\n",path_to_mirror);

	mkdir(path_to_mirror,0700);

	/*px
	char *str=malloc(100*sizeof(char));
	if(read(fd,str,10)<0){
		perror("ERROR IN READING 1");
				exit(10);
	}
	printf("\n\n\nMESSAGE IS %s...\n\n\n",str);
	*/
	//int useless_part=-1;//poio kommati apo to path_name den xreiazomai , dld krataw mono ton teleutaio fakelo apo to prwto pathname pou 8a mou er8ei kai to kollaw meta to path_to_mirror , to -1 shmainei oti einai h prwth fora pou to trexw opote den exei parei timh akoma , (apo to useless_part kai meta to kratame)
	for(;;){
		//char *length_name=malloc(3*sizeof(char));
		//memset(length_name,0,3);
		unsigned short length_name;
		if(read(fd,&length_name,2)<0){// 1.
			perror("ERROR IN READING 2");
					exit(10);
		}
		printf("\n\n\nlength_name IS %d...\n\n\n",length_name);

		if(length_name==0){
			printf("TELOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOS\n");
			break;
		}

		char *path_name=malloc((length_name+1)*sizeof(char));
		memset(path_name,0,length_name+1);
		if(read(fd,path_name,length_name)<0){// 2.
			perror("ERROR IN READING 3");
					exit(10);
		}
		printf("\n\n\npath_name IS %s...\n\n\n",path_name);

		//ftiaxnw to onoma tou arxeio gia to mirror
		/*if(useless_part==-1){
			int i;
			for(i=strlen(path_name);i>=0;i--){
				if(path_name[i]=='/'){
					break;
				}
			}
			useless_part=i+1;
			printf(" 1111111111111111111111111111111111111111111111 %s...\n",&(path_name[useless_part]));
		}*/

		if(path_name[strlen(path_name)-1]=='/'){//is a dir
			char *next_dir;
			next_dir=malloc((strlen(path_to_mirror)+strlen(path_name)+1)*sizeof(char));
			memset(next_dir,0,strlen(path_to_mirror)+strlen(path_name)+1);
			strcpy(next_dir,path_to_mirror);
			strcat(next_dir,path_name);

			mkdir(next_dir,0700);
///////////////////////////////////////
			fprintf(f_log,"receive %d\n",2+length_name);
////////////////////////////////////////
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

			printf("next_file = %s...\n",next_file);

			fp=fopen(next_file,"w");
			//fclose(fp);

			//read context of file
			//char *length_file=malloc(5*sizeof(char));
			//memset(length_file,0,5);
			unsigned int length_file;
			if(read(fd,&length_file,4)<0){// 3.
				perror("ERROR IN READING 4");
						exit(10);
			}
			printf("\n\n\nlength_file IS %d...\n\n\n",length_file);

			char *string;
			string=malloc((length_file+1)*sizeof(char));
			memset(string,0,length_file+1);

			//char *temp_string;
			//temp_string=malloc((b+1)*sizeof(char));

			int bytes_transfered=0,tr;
			while(bytes_transfered<length_file){
				//memset(temp_string,0,b+1);
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
				//printf("\t\t\ttr=%d",tr);
			}
			printf("\t\t\treceiver done with file\n");

			fprintf(fp, "%s",string);
			free(string);

			fclose(fp);
///////////////////////////////////////
			fprintf(f_log,"receive %s %d\n",next_file,2+length_name+4+length_file);
////////////////////////////////////////
		}

		//sleep(1); //sto git me to COMPLETED to exw anoikto
		//break;
	}
}

/*
int isDirectory(const char *path){
	//https://stackoverflow.com/questions/4553012/checking-if-a-file-is-a-directory-or-just-a-file
	struct stat statbuf;
	if (stat(path, &statbuf) != 0)
		return 0;
	return S_ISDIR(statbuf.st_mode);
}
*/











