#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <signal.h>
#include <sys/wait.h>
#include "../header-files/functions.h"
#include "../header-files/list.h"

#define PERIOD 10 //ka8e posa seconds 8a elegxei o client ton common

//signals
void IntSignal(int signum);
void QuitSignal(int signum);
void ChildFinishedSignal(int signum);

int flag_to_quit;//otan o xrhsths dwsei ctrl^C h ctrl^\ tote auto to flag 8a ginei 1 wste na kserei o client na termathsei

int main(int argc,char **argv){
	flag_to_quit=0;

	struct_arguments *arguments;
	if(takeArguments(&arguments,argc,argv)==-1){
		return 1;
	}

	//printf("short=%ld int=%ld\n",sizeof(short),sizeof(int));

	if(checkAndCreateDirs(arguments)==-1){ // 1
		return 2;
	}

	if(createFileInCommon(arguments)==-1){ // 2
		return 3;
	}

	if(createLogFile(arguments)==-1){
		return 4;
	}

	//kanw set ta signals
	signal(SIGINT,IntSignal);
	signal(SIGQUIT,QuitSignal);
	signal(SIGCHLD,ChildFinishedSignal);


	//lista me ta arxeia pou epikoinwnei o client
	list *l;
	initializeList(&l);

	char *temp_id;
	temp_id=malloc((countDigits(arguments->id)+1)*sizeof(char));
	memset(temp_id,0,countDigits(arguments->id)+1);
	sprintf(temp_id,"%d",arguments->id);

	char *temp_buffer_size;
	temp_buffer_size=malloc((countDigits(arguments->id)+1)*sizeof(char));
	memset(temp_buffer_size,0,countDigits(arguments->id)+1);
	sprintf(temp_buffer_size,"%d",arguments->buffer_size);

	while(1){

		//elefxw to flag_to_quit gia na kanw delete to mirror kai to id apo ton common kai na bgw apo to loop
		if(flag_to_quit==1){
			break;
		}

		//printf("Checking for new or missing clients!\n");

		//mhdenizw ola ta still_exist
		initializeZeroList(l);

		//anoigw ton common dir kai blepw ta arxeia
		//https://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program
		DIR *d;
		struct dirent *dir;
		d=opendir(arguments->common_dir);
		if(d){
			while((dir=readdir(d))!=NULL){
				if(strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0){

					if(strstr(dir->d_name,".id")==NULL){//an den exei .id , tote einai pipe kai den prepei na to laboume upopshn
						continue;
					}

					if(searchList(l,dir->d_name)==0){
						printf("Found new client : %s\n",dir->d_name);
						insertList(l,dir->d_name);
						//printList(l);

						int pid;

						//gia to sender-child
						pid=fork();
						switch(pid){
							case -1:
								printf("\n\nFORK FAILED!!!\n\n\n");
								return 4;
							case 0://child
								;
								int other_id=0;
								int i=0;
								while(dir->d_name[i]!='.'){
									other_id=other_id*10+dir->d_name[i]-'0';
									i++;
								}
								char *temp_other_id;
								temp_other_id=malloc((countDigits(other_id)+1)*sizeof(char));
								memset(temp_other_id,0,countDigits(other_id)+1);
								sprintf(temp_other_id,"%d",other_id);
								execl("./sender_child","./sender_child",temp_id,arguments->common_dir,arguments->input_dir,arguments->mirror_dir,temp_buffer_size,arguments->log_file,temp_other_id,NULL);
								break;
							default://parent
								break;
						}

						//gia to receiver-child
						pid=fork();
						switch(pid){
							case -1:
								printf("\n\nFORK FAILED!!!\n\n\n");
								return 4;
							case 0://child
								;
								int other_id=0;
								int i=0;
								while(dir->d_name[i]!='.'){
									other_id=other_id*10+dir->d_name[i]-'0';
									i++;
								}
								char *temp_other_id;
								temp_other_id=malloc((countDigits(other_id)+1)*sizeof(char));
								memset(temp_other_id,0,countDigits(other_id)+1);
								sprintf(temp_other_id,"%d",other_id);
								execl("./receiver_child","./receiver_child",temp_id,arguments->common_dir,arguments->input_dir,arguments->mirror_dir,temp_buffer_size,arguments->log_file,temp_other_id,NULL);
								break;
							default://parent
								break;
						}
					}
				}
			}

			//ektelw to 5 gia ola ta still_exist==0
			char *missing_client;
			while((missing_client=searchAndDeleteZeroList(l))!=NULL){
				printf("Found missing_client : %s\n",missing_client);

				//gia to receiver-child
				int pid=fork();
				switch(pid){
					case -1:
						printf("\n\nFORK FAILED!!!\n\n\n");
						return 4;
					case 0://child
						;
						int other_id=0;
						int i=0;
						while(missing_client[i]!='.'){
							other_id=other_id*10+missing_client[i]-'0';
							i++;
						}
						char *temp_other_id;
						temp_other_id=malloc((countDigits(other_id)+1)*sizeof(char));
						memset(temp_other_id,0,countDigits(other_id)+1);
						sprintf(temp_other_id,"%d",other_id);
						execl("./cleaner_child","./cleaner_child",temp_id,arguments->common_dir,arguments->input_dir,arguments->mirror_dir,temp_buffer_size,arguments->log_file,temp_other_id,NULL);
						break;
					default://parent
						break;
				}
				free(missing_client);
			}
			closedir(d);
		}

		sleep(PERIOD);
	}
	//6
	//printf("DESTROY EVERYTHING !!!\n");
	//destroy mirror
	cleanDirOrFile(arguments->mirror_dir);
	//ftiaxnw to path gia to id ston common
	char *myIdFile;
	myIdFile=malloc((strlen(arguments->common_dir)+strlen(temp_id)+strlen(".id")+1)*sizeof(char));
	memset(myIdFile,0,strlen(arguments->common_dir)+strlen(temp_id)+strlen(".id")+1);
	strcpy(myIdFile,arguments->common_dir);
	strcat(myIdFile,temp_id);
	strcat(myIdFile,".id");
	//destroy arxeio
	cleanDirOrFile(myIdFile);
	/////6-end

	FILE *f_log;//GIA TO SCRIPT
	f_log=fopen(arguments->log_file,"a");
	fflush(f_log);
	fprintf(f_log,"client left id=%d\n",arguments->id);
	fclose(f_log);


	free(temp_id);
	free(temp_buffer_size);
	free(myIdFile);

	free(arguments->common_dir);
	free(arguments->input_dir);
	free(arguments->mirror_dir);
	free(arguments->log_file);
	free(arguments);

	destroyList(&l);
}


//signals
void IntSignal(int signum){//gia to A6
	signal(SIGINT,IntSignal);//mporei na mhn xreiazetai
	flag_to_quit=1;
}

void QuitSignal(int signum){//gia to A6
	signal(SIGQUIT,QuitSignal);//mporei na mhn xreiazetai
	flag_to_quit=1;
}

//https://www.linuxquestions.org/questions/programming-9/how-a-father-process-know-which-child-process-send-the-signal-sigchld-266290/
//https://www.geeksforgeeks.org/wait-system-call-c/
void ChildFinishedSignal(int signum){
	int stat;
	wait(&stat);

	if(WIFEXITED(stat)){
		if(WEXITSTATUS(stat)==2){//to 2 einai to epituxes exit apo ton receiver kai to 1 apo ton sender kai 3 gia ton cleaner
			printf("FILES TRANSFERRED SUCCESSFULLY !!!\n");
		}
	}
	else{
		printf("Child didn't terminate normally!\n");
	}

	signal(SIGCHLD,ChildFinishedSignal);
}





