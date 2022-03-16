#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "stack.h"
//Stack push and pop functions inspired by Professor Long(Lecture on Stacks and Queues)
struct Stack { //stack struct which has the size and capacity
    uint32_t size;
    uint32_t capacity;
    Node **items;
};

Stack *stack_create(uint32_t
        capacity) { //constructor for the stack which initializes the node pointer array, size, and capacity
    Stack *s = (Stack *) malloc(sizeof(Stack));
    s->items = (Node **) calloc(capacity + 1, sizeof(Node *));
    s->size = 0;
    s->capacity = capacity;
    return s;
}

void stack_delete(Stack **s) {
    free((*s)->items);
    (*s)->items = NULL;
    free(*s);
    *s = NULL; //destructor function which clears the node pointer array and stack
}

bool stack_empty(Stack *s) { //checks if the stack is empty
    return s->size == 0;
}

bool stack_full(Stack *s) { //checks if the stack is full
    return s->size == s->capacity;
}

uint32_t stack_size(Stack *s) { //returns the size of the stack
    return s->size;
}

bool stack_push(Stack *s, Node *n) { //pushes a node onto the stack
    if (stack_full(s) == true) { //checks if the stack is full before pushing
        return false;
    } else {
        s->items[s->size] = n; //sets the latest stack element to the new node
        s->size++; //increments size
    }
    return true;
}

bool stack_pop(Stack *s, Node **n) { //pops nodes from the stack
    if (stack_empty(s) == true) { //checks if the stack is empty
        return false;
    } else {
        *n = s->items[s->size - 1]; //sets the passed node to the latest element in the stack
        //node_delete(&s->items[s->size - 1]);
        s->size--; //decrements the size
    }
    return true;
}

void stack_print(Stack *s) { //print function for debugging
    for (uint64_t i = 0; i < stack_size(s); i++) {
        node_print(s->items[i]);
    }
    printf("size %u\n", s->size);
}
