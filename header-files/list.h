typedef struct list list;
typedef struct listnode listnode;

struct list{
	listnode *start;
	listnode *end;
};

struct listnode{
	char *str;
	int still_exist;//8a einai arxikopoihmeno se 0 kai an meta to telos tou loop sthn main den bre8ei to arxeio 8a parameinei 0 (den 8a ginei pote 1) kai 8a kserw oti prepei na diagrapsw
	listnode *next;
};

void initializeList(list **l);
void insertList(list *l,char *element);
int searchList(list *l,char *element);
void printList(list *l);
void destroyList(list **l);
void initializeZeroList(list *l);
char *searchAndDeleteZeroList(list *l);
