#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "code.h"
//Lines 27-33 inspired by Professor Long: https://git.ucsc.edu/cse13s/code-comments/-/blob/master/bv8.h
//Lines 36-42 inspired by Professor Long: https://git.ucsc.edu/cse13s/code-comments/-/blob/master/bv8.h
//Lines 45-47 inspired by Professor Long: https://git.ucsc.edu/cse13s/code-comments/-/blob/master/bv8.h
//Lines 50-58 inspired by Professor Long: https://git.ucsc.edu/cse13s/code-comments/-/blob/master/bv8.h
Code code_init(
    void) { //constructor that sets the top of the code stack to 0 and zeros out all the indices in the bits array
    Code c;
    c.top = 0;
    for (uint32_t i = 0; i < MAX_CODE_SIZE; i++) {
        c.bits[i] = 0;
    }
    return c;
}

uint32_t code_size(Code *c) { //returns the size of the code
    return c->top;
}

bool code_empty(Code *c) { //checks if a code is empty
    return c->top == 0;
}

bool code_full(Code *c) { //checks if the code is full or not
    return c->top == ALPHABET;
}

bool code_set_bit(Code *c, uint32_t i) { //sets a bit at a specific position using bitwise OR
    if (!(i < ALPHABET && i >= 0)) { //checks if the position is out of bounds or not
        return false;
    }
    c->bits[i / 8] |= (0x1 << i % 8);
    return true;
}

bool code_clr_bit(
    Code *c, uint32_t i) { //sets a bit at a specific position to 0 using bitwise AND and inverse
    if (!(i < ALPHABET && i >= 0)) { //checks if the position is out of bounds or not
        return false;
    }
    c->bits[i / 8] &= ~(0x1 << i % 8);
    return true;
}

bool code_get_bit(
    Code *c, uint32_t bit) { // gets the current bit at a specific position using bitwise AND
    return (c->bits[bit / 8] >> bit % 8) & 0x1;
}

bool code_push_bit(Code *c, uint8_t bit) { //pushes bits onto the stack of bits using bitwise OR 0x1
    if (code_full(c) == true) { //checks if the code is full or not before pushing
        return false;
    } else {
        c->bits[(c->top) / 8] |= (bit << (c->top) % 8);
        c->top++;
    }
    return true;
}

bool code_pop_bit(
    Code *c, uint8_t *bit) { //pops bits at a specific position from the stack using code_get_bit
    if (code_empty(c) == true) { //checks if the code is empty before popping
        return false;
    } else {
        *bit = code_get_bit(c, c->top - 1); //pops, deletes, and decrements top
        code_clr_bit(c, c->top - 1);
        c->top--;
    }
    return true;
}

void code_print(Code *c) { //print function for debugging
    for (uint32_t x = 0; x < c->top; x++) {
        //printf("test %d", x);
        if (code_get_bit(c, x)) {
            printf("1");
        } else {
            printf("0");
        }
    }
    printf("\n");
}
