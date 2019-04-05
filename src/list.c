#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header-files/list.h"

void initializeList(list **l){
	(*l)=malloc(sizeof(list));
	(*l)->start=NULL;
	(*l)->end=NULL;
}

void insertList(list *l,char *element){
	if(l->end==NULL){
		l->end=malloc(sizeof(listnode));
		l->end->str=malloc((strlen(element)+1)*sizeof(char));
		memset(l->end->str,0,strlen(element)+1);
		strcpy(l->end->str,element);
		l->end->still_exist=1;//
		l->end->next=NULL;

		l->start=l->end;
	}
	else{
		l->end->next=malloc(sizeof(listnode));
		l->end->next->str=malloc((strlen(element)+1)*sizeof(char));
		memset(l->end->next->str,0,strlen(element)+1);
		strcpy(l->end->next->str,element);
		l->end->next->still_exist=1;//
		l->end->next->next=NULL;

		l->end=l->end->next;
	}
}

int searchList(list *l,char *element){
	listnode *temp;
	temp=l->start;
	while(temp!=NULL){
		if(strcmp(temp->str,element)==0){
			temp->still_exist=1;//
			return 1;//found it
		}
		temp=temp->next;
	}
	return 0;//not found
}

void printList(list *l){
	listnode *temp;
	temp=l->start;
	printf("\n\n");
	printf("printList\n");
	while(temp!=NULL){
		printf("%d %s...\n",temp->still_exist,temp->str);
		temp=temp->next;
	}
	printf("\n\n");
}

void destroyList(list **l){
	listnode *temp,*prev;
	temp=(*l)->start;
	while(temp!=NULL){
		prev=temp;
		temp=temp->next;
		free(prev);
	}
	free((*l));
}

void initializeZeroList(list *l){
	listnode *temp;
	temp=l->start;
	while(temp!=NULL){
		temp->still_exist=0;
		temp=temp->next;
	}
}

char *searchAndDeleteZeroList(list *l){
	listnode *temp,*prev;
	temp=l->start;
	prev=NULL;
	while(temp!=NULL){
		if(temp->still_exist==0){
			char *id;
			printf("\n\n\n\ntemp->str=%s\n\n\n\n",temp->str);
			id=malloc((strlen(temp->str)+1)*sizeof(char));
			memset(id,0,strlen(temp->str)+1);
			strcpy(id,temp->str);

			if(l->start==temp && l->end==temp){//exei mono 1 stoixeio
				l->start=NULL;
				l->end=NULL;
				//free(temp);
			}
			else if(l->start==temp){//to prwto stoixeio
				l->start=temp->next;
				//free(temp);
			}
			else if(l->end==temp){//to teletaio stoixeio
				prev->next=NULL;
				l->end=prev;
				//free(temp);
			}
			else{//stoixeio kapou sth mesh
				prev->next=temp->next;
			}
			free(temp);

			return id;
		}
		prev=temp;
		temp=temp->next;
	}
	return NULL;
}






