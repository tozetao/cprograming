#ifndef STACKADT_H

#define STACKADT_H

/**
 * Stack类型，是一个指向stack_type结构的指针
 */
typedef struct stack_type *Stack;

Stack create(void);

void destory(Stack s);

void make_empty(Stack s);

short is_empty(Stack s);

short is_full(Stack s);

void push(Stack s, void *val, int size);

void pop(Stack s);

#endif
