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

//signals
void IntSignal(int signum);
void QuitSignal(int signum);
void ChildFinishedSignal(int signum);

int flag_to_quit;

int main(int argc,char **argv){
	flag_to_quit=0;

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
	printf("short=%ld int=%ld\n",sizeof(short),sizeof(int));
//////////////////

	if(checkAndCreateDirs(arguments)==-1){ // 1
		return 2;
	}

	if(createFileInCommon(arguments)==-1){ // 2
		return 3;
	}

	if(createLogFile(arguments)==-1){
		return 4;
	}

/////////////////////
	signal(SIGINT,IntSignal);
	signal(SIGQUIT,QuitSignal);
	signal(SIGCHLD,ChildFinishedSignal);//////////////////////
/////////////////////

	//leipei to bhma 3

	//lista me ta arxeia pou epikoinwnei
	list *l;
	initializeList(&l);

	//bazw to arxeio tou apo to common dir wste na to agnow , MPOREI NA MHN XREIAZETAI WSTE NA KANW SUNDESH ME TON EAUTO MOU WSTE NA GINEI H METAFORA !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	/*char *temp;
	temp=malloc((countDigits(arguments->id)+strlen(".id")+1)*sizeof(char));
	memset(temp,0,countDigits(arguments->id)+strlen(".id")+1);
	sprintf(temp,"%d",arguments->id);
	strcat(temp,".id");

	insertList(l,temp);*///AYTO EINAI GIA AN 8ELW NA METAFERW H OXI TA DIKA MOU ARXEIA KAI ANOIKTO KAI KLEISTO DOULEYEI MIA XARA
	printList(l);

	printf("\n\n\n\n\n");

	//char *sender_args[]={"./sender_child",NULL};
	//char *receiver_args[]={"./receiver_child",NULL};

	char *temp_id;
	temp_id=malloc((countDigits(arguments->id)+1)*sizeof(char));
	//memset(temp_id,0,countDigits(arguments->id)+strlen(".id")+1); //////////////////
	memset(temp_id,0,countDigits(arguments->id)+1);
	sprintf(temp_id,"%d",arguments->id);

	char *temp_buffer_size;
	temp_buffer_size=malloc((countDigits(arguments->id)+1)*sizeof(char));
	//memset(temp_buffer_size,0,countDigits(arguments->id)+strlen(".id")+1);//////////
	memset(temp_buffer_size,0,countDigits(arguments->id)+1);
	sprintf(temp_buffer_size,"%d",arguments->buffer_size);

	//char *sender_args[]={"./sender_child",temp_id,arguments->common_dir,arguments->input_dir,arguments->mirror_dir,temp_buffer_size,arguments->log_file,NULL,NULL};
	//char *receiver_args[]={"./receiver_child",temp_id,arguments->common_dir,arguments->input_dir,arguments->mirror_dir,temp_buffer_size,arguments->log_file,NULL,NULL};

	while(1){
////////////////////////////
//elefxw to flag_to_quit gia na kanw delete to mirror kai to id apo ton common kai na bgw apo to loop
		if(flag_to_quit==1){
			break;
		}
////////////////////////////
////////////////////////////
//mhdenizw ola ta still_exist
		initializeZeroList(l);///////
////////////////////////////
		//anoigw ton common dir kai blepw ta arxeia
		//https://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program
		DIR *d;
		struct dirent *dir;
		d=opendir(arguments->common_dir);
		if(d){
			while((dir=readdir(d))!=NULL){
				if(strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0){
					printf("indir=%s...\n",dir->d_name);

					if(strstr(dir->d_name,".id")==NULL){//an den exei .id , tote einai pipe kai den prepei na to laboume upopshn
						printf("AAA\n");
						continue;
					}

					if(searchList(l,dir->d_name)==0){
						insertList(l,dir->d_name);
						printList(l);

						int pid;

						//gia to sender-child
						pid=fork();
						switch(pid){
							case -1:
								printf("\n\nFORK FAILED!!!\n\n\n");
								return 4;
							case 0://child
								printf("child1\n");
								//////////////////////////
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
								//////////////////////////
								//execv(sender_args[0],sender_args);
								execl("./sender_child","./sender_child",temp_id,arguments->common_dir,arguments->input_dir,arguments->mirror_dir,temp_buffer_size,arguments->log_file,temp_other_id,NULL);
								printf("child2\n");
								break;
							default://parent
								//array_of_processes[i]=pid;
								printf("parent\n");
								break;
						}

						//gia to receiver-child
						pid=fork();
						switch(pid){
							case -1:
								printf("\n\nFORK FAILED!!!\n\n\n");
								return 4;
							case 0://child
								printf("child1\n");
								//////////////////////////
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
								//////////////////////////
								//execv(receiver_args[0],receiver_args);
								execl("./receiver_child","./receiver_child",temp_id,arguments->common_dir,arguments->input_dir,arguments->mirror_dir,temp_buffer_size,arguments->log_file,temp_other_id,NULL);
								printf("child2\n");
								break;
							default://parent
								//array_of_processes[i]=pid;
								printf("parent\n");
								break;
						}
					}
				}
			}
////////////////////////////////////////////////////////////////
//ektelw to 5 gia ola ta still_exist==0
			char *missing_client;//exei leak mallon
			while((missing_client=searchAndDeleteZeroList(l))!=NULL){///////
				printf("\n\n\nmissing_client=%s...\n\n\n",missing_client);

				//gia to receiver-child
				int pid=fork();
				switch(pid){
					case -1:
						printf("\n\nFORK FAILED!!!\n\n\n");
						return 4;
					case 0://child
						printf("child1\n");
						//////////////////////////
						int other_id=0;
						int i=0;
						while(missing_client[i]!='.'){
							other_id=other_id*10+missing_client[i]-'0';
							i++;
						}
						char *temp_other_id;//mallon auto einai to mono leak mou
						temp_other_id=malloc((countDigits(other_id)+1)*sizeof(char));
						memset(temp_other_id,0,countDigits(other_id)+1);
						sprintf(temp_other_id,"%d",other_id);
						//////////////////////////
						//execv(receiver_args[0],receiver_args);
						execl("./cleaner_child","./cleaner_child",temp_id,arguments->common_dir,arguments->input_dir,arguments->mirror_dir,temp_buffer_size,arguments->log_file,temp_other_id,NULL);
						printf("child2\n");
						break;
					default://parent
						//array_of_processes[i]=pid;
						printf("parent\n");
						break;
				}
			}
			printList(l);
			printf("\n\nALL missing_clients DELETED\n\n");
////////////////////////////////////////////////////////////////
			closedir(d);
		}

		sleep(10);//to eixa 10
///////////////////////////////////////
/*	FILE *f_log;//AYTA PREPEI NA FYGEI TO EXW BALEI APLA GIA NA TESTARW TO SCRIPT
	f_log=fopen(arguments->log_file,"a");
	fprintf(f_log,"client left id=%d\n",arguments->id);
	fclose(f_log);
*/
/////////////////////////////////////////
	}
	//6
	printf("DESTROY EVERYTHING !!!\n");
	//destroy mirror
	cleanDirOrFile(arguments->mirror_dir);
	//ftiaxnw to path gia to id ston common
	char *myIdFile;
	myIdFile=malloc((strlen(arguments->common_dir)+strlen(temp_id)+strlen(".id")+1)*sizeof(char));
	memset(myIdFile,0,strlen(arguments->common_dir)+strlen(temp_id)+strlen(".id")+1);
	strcpy(myIdFile,arguments->common_dir);
	strcat(myIdFile,temp_id);
	strcat(myIdFile,".id");
	printf("temp_id=%s... myIdFile=%s...\n",temp_id,myIdFile);
	//destroy arxeio
	cleanDirOrFile(myIdFile);
	/////6-end
///////////////////////////////////////
	FILE *f_log;//GIA TO SCRIPT
	f_log=fopen(arguments->log_file,"a");
	fprintf(f_log,"client left id=%d\n",arguments->id);
	fclose(f_log);
/////////////////////////////////////////

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
	printf("I AM SIGINT !\n");
	flag_to_quit=1;
}

void QuitSignal(int signum){//gia to A6
	signal(SIGQUIT,QuitSignal);//mporei na mhn xreiazetai
	printf("I AM SIGQUIT !\n");
	flag_to_quit=1;
}

//https://www.linuxquestions.org/questions/programming-9/how-a-father-process-know-which-child-process-send-the-signal-sigchld-266290/
//https://www.geeksforgeeks.org/wait-system-call-c/
void ChildFinishedSignal(int signum){
	//signal(SIGCHLD,ChildFinishedSignal);
	int stat;
	pid_t pid;
	pid=wait(&stat);
	printf("Pid %d exited.\n",pid);

	if(WIFEXITED(stat)){
		printf("Exit status: %d\n",WEXITSTATUS(stat));
		if(WEXITSTATUS(stat)==2){//to 2 einai to epituxes exit apo ton receiver kai to 1 apo ton sender
			printf("FILES TRANSFERRED SUCCESSFULLY !!!\n");
		}
	}
	else{
		printf("we will see !!!\n");
	}

	signal(SIGCHLD,ChildFinishedSignal);
}




