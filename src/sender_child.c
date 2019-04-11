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

void sendFiles(int fd,char *directory_or_file,int b,char *log_file,int useless_part);

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
	strcat(pipename,id);
	strcat(pipename,"_to_");
	strcat(pipename,other_id);
	strcat(pipename,".fifo");

	//https://stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c
	if(access(pipename,F_OK)!=-1){// file exists
		//printf("%s already exists\n",pipename);
	}
	else{// file doesn't exist 
		if(mkfifo(pipename,0666)==-1){//opote to ftiaxnw
			if(errno!=EEXIST){
				perror("receiver:mkfifo_recv");
				exit(6);
			}
		}
	}
	//open
	int fd;
	if((fd=open(pipename,O_WRONLY))<0){
		perror("fifo open problem1");
				exit(7);
	}

	//to useless_part einai apo thn arxh ws to proteleutaio '/' dld kratame mono ton teleutaio fakelo tou input_dir
	//o skopos einai na dinw ston receiver apo ekei kai katw gt mono auto ton endiaferei
	int useless_part;
	int i;
	for(i=strlen(input_dir)-2;i>=0;i--){//-2 gt den 8elw to teleutaio '/'
		if(input_dir[i]=='/'){
			break;
		}
	}
	useless_part=i+1;

	sendFiles(fd,input_dir,b,log_file,useless_part);

	unsigned short int length_name=0;

	//otan teleiwsw me thn metafora stelnw mesw sou pipe 00
	if(write(fd,&length_name,2)==-1){//1.
		perror("ERROR IN WRITING 2");
				exit(10);
	}

	//close pipe
	close(fd);


	free(id);
	free(common_dir);
	free(input_dir);
	free(mirror_dir);
	free(buffer_size);
	free(log_file);
	free(other_id);
	free(pipename);

	exit(1);
}

void sendFiles(int fd,char *directory_or_file,int b,char *log_file,int useless_part){//pairnw to fd apo to pipe kai ton fakelo pou 8elw na metaferw(directory_or_file)
	unsigned short length_name;
	length_name=strlen(&(directory_or_file[useless_part]));

	if(write(fd,&length_name,2)==-1){//1.
		perror("ERROR IN WRITING 3");
				exit(10);
	}

	if(write(fd,&(directory_or_file[useless_part]),length_name)==-1){//2.
		perror("ERROR IN WRITING 4");
				exit(10);
	}

	//anoigw to log arxeio
	FILE *f_log;
	f_log=fopen(log_file,"a");


	//elegxw an einai arxeio h dir
	if(isDirectory(directory_or_file)>0){//an einai dir , tom anoigw kai kalw thn synarthsh gia oti exei mesa
		//bazw sto logfile ta bytes pou edwsa sto pipe gia na metaferw ton fakelo
		fprintf(f_log,"send %d\n",2+length_name);
		fclose(f_log);

		//https://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program
		DIR *d;
		struct dirent *dir;
		d=opendir(directory_or_file);//anoigw ton fakelo , ftiaxnw to monopati gia oti exei mesa kai kalw pali thn sendFiles
		if(d){
			while((dir=readdir(d))!=NULL){
				if(strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0){
					char *next_dir;
					//ousistika kollaw sto prohgoumeno dir ton neo stoixeio
					next_dir=malloc((strlen(directory_or_file)+strlen(dir->d_name)+strlen("/")+1)*sizeof(char));
					memset(next_dir,0,strlen(directory_or_file)+strlen(dir->d_name)+strlen("/")+1);
					strcpy(next_dir,directory_or_file);
					strcat(next_dir,dir->d_name);
					if(isDirectory(next_dir)>0){//kai an einai dir tote bazw / sto telos
						strcat(next_dir,"/");
					}
					sendFiles(fd,next_dir,b,log_file,useless_part);
					free(next_dir);
				}
			}
		}
		closedir(d);
	}
	else{//alliws ektelw ta bhmata 3,4

		//https://stackoverflow.com/questions/14002954/c-programming-how-to-read-the-whole-file-contents-into-a-buffer
		FILE *f=fopen(directory_or_file,"rb");
		fseek(f,0,SEEK_END);
		unsigned int fsize=ftell(f);
		fseek(f,0,SEEK_SET);

		char *string=malloc((fsize+1)*sizeof(char));

		fread(string,fsize,1,f);
		fclose(f);

		string[fsize]=0;

		if(write(fd,&fsize,4)==-1){//3.
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
			else{//exei ligotera apo b opote metaferw osa mou exoun meinei
				if((tr=write(fd,&(string[bytes_transfered]),fsize-bytes_transfered))==-1){//4.
					perror("ERROR IN WRITING 7");
							exit(10);
				}
				bytes_transfered+=tr;
			}
		}
		free(string);

		//bazw sto logfile to onoma kai to bytes pou xreiasthkan gia na metafer8ei to arxeio
		fprintf(f_log,"send %s %d\n",directory_or_file,2+length_name+4+fsize);
		fclose(f_log);
	}
}


