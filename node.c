
#include "node.h"
#include <stdlib.h>
#include <stdio.h>

Node *node_create(uint8_t symbol, uint64_t frequency) {
    Node *x = (Node *) malloc(
        sizeof(Node)); //creation of the node, which has a left child and a right child
    x->symbol = symbol;
    x->frequency = frequency;
    x->left = NULL;
    x->right
        = NULL; //initializes the given symboles to the passed arguments and the left and right children to NULL(for now)
    return x;
}

void node_delete(Node **n) { //desctructor for the node
    free(*n);
    *n = NULL;
}

Node *node_join(Node *left,
    Node *
        right) { //creates a node parent who's sybol is '$' and frequency is the sum of the left and right children
    Node *parent = node_create('$', left->frequency + right->frequency);
    parent->left = left;
    parent->right = right; //makes the parrent point to the left and right child
    return parent;
}

void node_print(Node *n) { //print function for debugging the nodes
    if (n->left == NULL || n->right == NULL) {
        printf("\nparent: %c frequency: %lu\n", n->symbol, n->frequency);
        return;
    }
    printf("\nparent: %c%c%c frequency: %lu\n", n->symbol, n->left->symbol, n->right->symbol,
        n->frequency);
    printf("left child: %c frequency: %lu\n", n->left->symbol, n->left->frequency);
    printf("right child: %c frequency: %lu\n", n->right->symbol, n->right->frequency);
}
