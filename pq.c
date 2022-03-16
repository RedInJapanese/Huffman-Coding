#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "pq.h"

//insertion sort methods and shifter function derived from Eugene Chou(TA) in Section/Office Hours
//Line 22 derived from Benjamin Grant(TA): https://discord.com/channels/926211705766305842/926211706382868585/947672641655935017
//Lines 93 to 108 derived from Akash Basu: Assignment 3: Sorting Algorithms: insert.c
void insertion_sort(
    PriorityQueue *q, Node *t); //declaration of insertion sort and shifter functions
void l_shift(PriorityQueue *q);

struct
    PriorityQueue { //priority queue struct which has a head, a tail, a size, a maximum capacity, and an array of node pointers
    Node *head;
    Node *tail;
    uint32_t size;
    uint32_t capacity;
    Node **nodes;
};

PriorityQueue *pq_create(uint32_t capacity) { //constructor for the priority queue
    PriorityQueue *q = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    q->nodes = (Node **) calloc(capacity + 1, sizeof(Node *)); //callocs the array of node pointers
    q->head = q->tail = NULL;
    q->capacity = capacity;
    q->size = 0; //initializes the other variables in the struct
    return q;
}

void pq_delete(PriorityQueue *
        *q) { // destructor for priority queue that frees both the node pointer array and the queue
    free((*q)->nodes);
    (*q)->nodes = NULL;
    free(*q);
    *q = NULL;
}

bool pq_empty(PriorityQueue *q) { //checks if a queue is empty
    return q->size == 0;
}

bool pq_full(PriorityQueue *q) { //checks if a queue is full
    return q->size == q->capacity;
}

uint32_t pq_size(PriorityQueue *q) { //returns the queue size
    return q->size;
}

bool enqueue(PriorityQueue *q, Node *n) {
    if (pq_full(q) == true) { //checks if the queue is full before adding a new node
        return false;
    }
    if (q->size
        >= 1) { //checks if the queue size is greater than one. If it is, then it calls the insertion sort function and increments size
        insertion_sort(q, n);
        q->size++;
    } else if (pq_empty(q)
               == true) { //checks if the queue is empty and that this is the first node being added
        q->nodes[q->size] = n;
        q->head = q->nodes[0];
        q->tail = q->nodes[q->size];
        q->size += 1;
        //return true;
    }
    return true;
}

bool dequeue(PriorityQueue *q, Node **n) {
    if (pq_empty(q) == true) { //checks if the queue is empty before dequeueing
        return false;
    }
    if (q->size == 1) { //checks if this is the only node in the queue
        *n = q->head;
        //node_delete(&q->nodes[0]);
        q->head = NULL;
        q->tail = NULL;
        q->size--;
    }
    if (q->size > 1) { //checks if the queue size is greater than 1
        *n = q->head; //sets the head of the queue to the passed node, since queues are first in first out
        //node_delete(&q->nodes[0]);
        //puts("delete works");
        l_shift(q); //calls the shifter function
    }

    return true;
}
void pq_print(PriorityQueue *q) { //printing function for debugging
    for (uint64_t i = 0; i < pq_size(q); i++) {
        node_print(q->nodes[i]);
    }
    printf("\nsize: %u\n", q->size);
}

void insertion_sort(PriorityQueue *q, Node *t) {
    uint64_t j = q->size;
    q->nodes[j] = t;
    Node *temp = q->nodes[j]; //creation of temporary variables to simulate the insertion sort
    while (
        j > 0
        && temp->frequency
               < q->nodes[j - 1]
                     ->frequency) { //checks if the queue is fully sorted and if the current nodes frequency is smaller than the last node
        /*Node temp;
        temp.symbol = q->nodes[j]->symbol;
        temp.frequency = q->nodes[j]->frequency;*/

        q->nodes[j] = q->nodes[j - 1]; //switches the nodes and decrements if the condition is met
        q->nodes[j - 1] = temp;
        j--;
    }
    q->head = q->nodes[0];
    q->tail = q->nodes[q->size]; //readjust the head and tail
}

void l_shift(PriorityQueue *q) {
    uint64_t y = 1;
    //puts("works");
    for (uint64_t k = 0; k < pq_size(q);
         k++) { //shifts every node pointer down by 1 every time a node gets dequeued
        //node_print(q->nodes[k]);
        q->nodes[k] = q->nodes[y]; //switches nodes
        y++;
        //q->nodes[k]->frequency = q->nodes[k+1]->frequency;
    }
    q->size--;
    q->head = q->nodes[0];
    q->tail = q->nodes[q->size - 1]; //readjust size, head, and tail
}
