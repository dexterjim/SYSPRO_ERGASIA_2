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
		strcpy(l->end->str,element);
		l->end->next=NULL;

		l->start=l->end;
	}
	else{
		l->end->next=malloc(sizeof(listnode));
		l->end->next->str=malloc((strlen(element)+1)*sizeof(char));
		strcpy(l->end->next->str,element);
		l->end->next->next=NULL;

		l->end=l->end->next;
	}
}

int searchList(list *l,char *element){
	listnode *temp;
	temp=l->start;
	while(temp!=NULL){
		if(strcmp(temp->str,element)==0){
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
		printf("%s...\n",temp->str);
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
