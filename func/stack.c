#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

#define STACK_SIZE 100

struct stack_type {
    int contents[STACK_SIZE];
    int top;
};

static void terminate(const char *message)
{
    printf("%s\n", message);
    exit(EXIT_FAILURE);
}

Stack create(void)
{
    Stack s = (struct stack_type *) malloc(sizeof(struct stack_type));
    
    if (s == NULL)
        terminate("Error in create: Stack could not be created");

    s->top = 0;
    return s;
}

void destory(Stack s)
{
    free(s);
}

void make_empty(Stack s)
{
    s->top = 0;
}

short is_empty(Stack s)
{
    return s->top == 0;
}

short is_full(Stack s)
{
    return s->top == STACK_SIZE;
}

void push(Stack s, int i)
{
    if (is_full(s))
        terminate("Error in push: stack is full");

    s->contents[s->top++] = i;
}

int pop(Stack s)
{
    if (is_empty(s))
        return 0;

    return s->contents[--s->top];
}
