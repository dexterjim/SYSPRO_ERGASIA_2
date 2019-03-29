typedef struct list list;
typedef struct listnode listnode;

struct list{
	listnode *start;
	listnode *end;
};

struct listnode{
	char *str;
	listnode *next;
};

void initializeList(list **l);
void insertList(list *l,char *element);
int searchList(list *l,char *element);
void printList(list *l);
void destroyList(list **l);
