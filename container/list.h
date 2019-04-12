#ifndef LIST_H

#define LIST_H

typedef struct node_type *node;
typedef struct list_type *list;

list list_create(void);

void list_destory(list l);

int list_insert(list, char *);

int list_append(list, char *);

char *list_remove(list);

//移除指定字符串
int list_remove_specified(list, const char *);

char *list_search(list, const char*);

#endif
