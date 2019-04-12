struct Node {
    int value;
    struct Node *next;
};

struct Node *add_node(struct Node *list, int n) 
{
    struct Node *new_node;

    new_node = (struct Node *) malloc(sizeof(struct Node));

    if (new_node == NULL) {
        printf("Error: malloc failed in add_node\n");
        exit(-1);
    }

    new_node->value = n;
    new_node->next = list;
    
    return new_node;
}

struct Node *search_list(struct Node *list, int n)
{
    struct Node *p;

    for(p = list; p->next != NULL; p = p->next)
        if(p->value == n)
            return p;

    return NULL;
}

//删除一个节点
int remove_node(struct Node *list, int n)
{
    struct Node *prev, *cur; 

    for(cur = list, prev = NULL; cur != NULL; prev = cur, cur = cur->next) {
        if (cur->value == n) {
            if (prev == NULL) {
                list = list->next;
                free(cur);
                return 1;
            }

            if (cur->next == NULL) 
                prev->next = NULL;
            else
                prev->next = cur->next;

            free(cur);
            return 1;
        }
    }

    return 0;
}

void display_list(struct Node *list)
{
    if (list == NULL)
        printf("the list is empty");

    while(list != NULL) {
        printf("%d\n", list->value);
        list = list->next;
    }
}
