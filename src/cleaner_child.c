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



	char *path;
	path=malloc((strlen(mirror_dir)+strlen(other_id)+strlen("/")+1)*sizeof(char));//sto path bazw ton monopati ws to mirror tou client kai to id tou allou client poy einai kai to onoma tou arxeiou mesa sto mirror
	memset(path,0,strlen(mirror_dir)+strlen(other_id)+strlen("/")+1);
	strcpy(path,mirror_dir);
	strcat(path,other_id);
	strcat(path,"/");

	cleanSubdirFromMirror(path);

	free(path);

	free(id);
	free(common_dir);
	free(input_dir);
	free(mirror_dir);
	free(buffer_size);
	free(log_file);
	free(other_id);
}


void cleanSubdirFromMirror(char *directory_or_file){
	if(isDirectory(directory_or_file)>0){//is dir
		DIR *d;
		struct dirent *dir;
		d=opendir(directory_or_file);//anoigw ton fakelo kai kalw thn synarthsh gia kaue stoixeio pou exei mesa
		if(d){
			while((dir=readdir(d))!=NULL){
				if(strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0){
					char *next_dir;//kollaw sto ws twra monopati to onoma tou stoixeiou pou brhka
					next_dir=malloc((strlen(directory_or_file)+strlen(dir->d_name)+strlen("/")+1)*sizeof(char));
					memset(next_dir,0,strlen(directory_or_file)+strlen(dir->d_name)+strlen("/")+1);
					strcpy(next_dir,directory_or_file);
					strcat(next_dir,dir->d_name);
					if(isDirectory(next_dir)>0){//an einai fakelo bazw kai to '/' sto telos
						strcat(next_dir,"/");
					}
					cleanSubdirFromMirror(next_dir);
					free(next_dir);
				}
			}
		}
		closedir(d);
		//delete directory
		rmdir(directory_or_file);
	}
	else{//is file
		remove(directory_or_file);
	}
}





