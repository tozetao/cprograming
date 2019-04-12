#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"

struct node_type {
    char *val;
    node next;
    node last;
};

struct list_type {
    node head;
    node tail;
};

list list_create(void)
{
    list container = (list) malloc(sizeof(struct list_type));
    return container;
}

void list_destory(list container)
{
    node current;

    if (container) {
        for(current = container->head; current != NULL; current = current->next)
            free(current);

        free(container);
    }
}

int list_insert(list container, char *val)
{
    node new_node = (node) malloc(sizeof(struct node_type));

    if (new_node == NULL)
        return -1;

    new_node->val = val;

    if (container->head == NULL)
        container->head = container->tail = new_node;
    else {
        container->head->last = new_node;
        new_node->next = container->head;
        container->head = new_node;
    }
    return 0;
}

int list_append(list container, char *val)
{
    node new_node = (node) malloc(sizeof(struct node_type));
    if (new_node == NULL)
        return -1;

    new_node->val = val;

    if (container->tail == NULL)
        container->head = container->tail = new_node;
    else {
        container->tail->next = new_node;
        new_node->last = container->tail;
        container->tail = new_node;
    }

    return 0;
}

//移除尾部节点
char *list_remove(list container)
{
    char *str;
    node tmp_node;

    if (container == NULL || container->tail == NULL)
        return NULL;

    str = container->tail->val;
    tmp_node = container->tail;

    if (container->tail->last == NULL) {
        container->tail = NULL;
        container->head = NULL;
    } else {
        container->tail = container->tail->last;
        container->tail->next = NULL;
    }

    free(tmp_node);

    return str;
}

int list_remove_specified(list container, const char *val)
{
    if (container == NULL)
        return -1;

    node current = container->head;

    while(current != NULL) {
        if (strcmp(current->val, val) != 0) {
            current = current->next;
            continue;
        }

        if (current->last == NULL && current->next == NULL) {
            container->head = container->tail = NULL;
        }

        if (current->last) 
            current->last->next = current->next;
        else
            container->head = current->next;

        if (current->next)
            current->next->last = current->last;
        else
            container->tail = current->last;

        free(current);
        return 0;
    }

    return -1;
}

char *list_search(list container, const char *val)
{
    node current;

    for(current = container->head; current != NULL; current = current->next)
        if (strcmp(current->val, val) == 0)
            return current->val;

    return NULL;
}

int list_is_empty(list container)
{
    return (container->head == NULL) && (container->tail == NULL);
}

void list_display(list container)
{
    node current;
    for(current = container->head; current != NULL; current = current->next)
        printf("%s\n", current->val);
}

int main()
{
    list str_list = list_create();   
    
    list_insert(str_list, "a");
    list_insert(str_list, "b");
    list_insert(str_list, "c");

    //printf("%s %s %s\n", list_remove(str_list), list_remove(str_list), list_remove(str_list));
    printf("%d \n", list_remove_specified(str_list, "a"));
    printf("%d \n", list_remove_specified(str_list, "a"));
    printf("%d \n", list_remove_specified(str_list, "c"));

    printf("%d\n", list_is_empty(str_list));
    list_display(str_list);
    list_destory(str_list);
    return 0;
}
