#include "huffman.h"
#include "pq.h"
#include "stack.h"
#include "io.h"
#include "code.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <inttypes.h>
//Lines 13 to 46 derived from pseudocode given in the assignment 6 instructions
//Lines 48 to 67 derived from pseudocode given in the assignment 6 instructions
//Lines 73 to 88 derived from pseudocode given in the assignment 6 instructions
static Code c; //declaration of code

Node *build_tree(uint64_t hist
        [static ALPHABET]) { //build tree function that builds a priority queue and uses insertion sort to arrange the frequencies in ascending order
    PriorityQueue *q = pq_create(ALPHABET);
    Node *root; //declaration of priority queue and root node

    for (
        uint64_t i = 0; i < ALPHABET;
        i++) { //iterates through the histogram and checks if there's any elements with frequecies greater than 0. If there are, then it gets enqueued as a node
        if (hist[i] > 0) {
            Node *x = node_create(i, hist[i]);
            enqueue(q, x);
        }
    }
    //pq_print(q);
    while (
        pq_size(q)
        > 1) { //dequeues two children, makes a parent, enqueues the parent, and repeats until there's one node left, the root node.
        Node *parent;
        Node *left;
        Node *right;

        dequeue(q, &left);
        dequeue(q, &right);

        //puts("CHILDREN");
        //node_print(left);
        //node_print(right);
        parent = node_join(left, right);
        //puts("PARENT");
        //node_print(parent);
        enqueue(q, parent);

        //puts("QUEUE");
        //pq_print(q);
    }
    dequeue(q, &root); //dequeues the last node in the priority queue to the root node
    pq_delete(&q);
    return root;
}
void build_codes(Node *root,
    Code table[static ALPHABET]) { //constructs each symbols respective code using a code table
    //Code c = code_init();
    //c = code_init();
    if (c.top == 0) { // initializes the exstra code
        c = code_init();
    }
    uint8_t bit_throw = 0;
    if (root != NULL) { //checks if the root exists
        if (!root->left
            && !root->right) { //checks if the node is a leaf. If it is then a code is given
            table[root->symbol] = c;
        } else {
            code_push_bit(&c, 0);
            //code_print(&c);
            build_codes(root->left, table);
            //code_print(&c);
            code_pop_bit(&c, &bit_throw);

            code_push_bit(&c, 1);
            build_codes(root->right, table);
            code_pop_bit(&c,
                &bit_throw); //recusively calls build_codes until a leaf node is reached. pops and pushes 0s and 1s based on where the symbol is in the tree
        }
    }
}

void dump_tree(int outfile,
    Node *
        root) { //creates the tree dump by writing L when there's a leaf node and I when there's an interior node. The encoder will use this to reconstruct the tree
    uint8_t l = 'L';
    uint8_t i = 'I';

    if (root) {
        dump_tree(outfile, root->left); //recursively walks to the left and right nodes
        dump_tree(outfile, root->right);

        if (!root->left && !root->right) { //if a leaf node is found, then an L is written
            write_bytes(outfile, &l, 1);
            write_bytes(outfile, &root->symbol, 1);
        } else { //if an interior node is founc, then an I is written
            write_bytes(outfile, &i, 1);
        }
    }
}

Node *rebuild_tree(uint16_t nbytes,
    uint8_t tree[static nbytes]) { //rebuilds the tree using the stack mentioned before

    Stack *s = stack_create(nbytes);
    Node *root; //declaration of stack and root node
    for (uint16_t i = 0; i < nbytes + 1;
         i++) { //iterates through nbytes and checks which indices are a leaf or interior nodes
        if (tree[i] == 'L') {
            Node *x = node_create(
                tree[i + 1], 0); // pushes a new leaf node onto the stack if a leaf is found
            stack_push(s, x);
        }
        if (tree[i] == 'I') {
            Node *left, *right,
                *parent; //pops nodes in right and left order then creates parent nodes and pushes them onto the stack
            stack_pop(s, &right);
            stack_pop(s, &left);
            parent = node_join(left, right);
            stack_push(s, parent);
        }
    }
    stack_pop(s, &root); //pops the root node and returns it
    return root;
}

void delete_tree(Node **root) { //descructor function for the root node
    if (*root) {
        delete_tree(&(*root)->left);
        delete_tree(&(*root)->right);

        if (!(*root)->left && !(*root)->right) {
            node_delete(root);
        }
    }
}
