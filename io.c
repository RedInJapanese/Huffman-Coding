#include "io.h"
#include "defines.h"
#include "code.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
//Lines 27 to 29 inspired by Simon Kwong(Student, CSE13S Winter 2022)
//Lines 44 to 46 inspired by Simon Kwong(Stident, CSE13s Winter 2022)
//Lines 56 to 71 inspired by Eugene Chou(TA), Section/Office Hours
//Lines 74 to 93 inspired by Eugene Chou(TA), Section/Office Hours
uint64_t bytes_read = 0;
uint64_t bytes_written = 0;
uint64_t bytes_coded = 0;

static uint8_t buf[BLOCK];
static uint8_t read_buff[BLOCK];
static uint32_t index = 0;
static uint32_t read_index = 0;
static ssize_t k = 0;
static ssize_t test = 0;
static uint8_t end
    = 0; //declaration of several buffers, byte storage variables, and three other variables to keep track of the bytes written and read for compression statistics

int read_bytes(int infile, uint8_t *buf,
    int nbytes) { //used in order to read bytes in huffman.c and encode/decode.c
    ssize_t j = 0;
    ssize_t x
        = 0; //ssize_ts used in order to increment bytes_read and give the offset when calling read()

    while (
        (j = read(infile, buf + x, nbytes - x))
        > 0) { //calls read with nbytes using an offset to make sure it doesn't start from the beginning repeatedly
        x += j; //increments x by j bytes, as read() does not consistently read nbytes
    }
    if (x == nbytes) {
        bytes_read += nbytes;
        return nbytes;
    } else {
        bytes_read += x;
        return x;
    } //conditionals to check if x reaches nbytes or not. returns values accordingly
}

int write_bytes(int outfile, uint8_t *buf,
    int nbytes) { //used for writing bytes in huffman.c and encode/decode.c
    ssize_t j = 0;
    ssize_t x
        = 0; //ssize_ts used in order ot increment bytes_written and give th eoffset when calling write()

    while (
        (j = write(outfile, buf + x, nbytes - x))
        > 0) { // calls write with nbytes using an offset ot make sure it doesn't start from the beginning repeatedly
        x += j; // increments x by j bytes, as write() does not consistently read nbytes
    }
    if (x == nbytes) {
        bytes_written += nbytes;
        return nbytes;
    } else {
        bytes_written += x;
        return x; //conditionals to check if x reaches nbytes or not. returns values accordingly
    }
}

bool read_bit(int infile,
    uint8_t *
        bit) { //function for reading the infile in decode.c bit by bit in order to interpret the codes
    //printf("reed buff%s\n", read_buff);
    if (read_index == 0) { //fills buffer if empty
        test = read_bytes(infile, read_buff, BLOCK);
        end = 8 * (test) + 1; //set the end equal to the bytes actually read, converted to bits
    }
    *bit = (read_buff[read_index / 8] >> read_index % 8)
           & 0x1; //gets the bit from the buffer using bitwise AND
    read_index++; //increments index
    if (read_index == end) { //conditionals to check if a valid bit was read
        //read_index = 0;
        return false;
    } else {
        //read_index = 0;
        return true;
    }
}

void write_code(int outfile,
    Code *c) { //function for taking in codes as parameters and copying them to a BLOCK sized buffer
    for (uint32_t i = 0; i < c->top; i++) { //iterates through the code size
        //bool current = code_get_bit(c, i);
        if (code_get_bit(c, i) == true) { //checks if the current bit is a 1 or 0
            buf[index / 8] |= (0x1 << index % 8);
        } else {
            buf[index / 8] &= ~(0x1 << index % 8); //copies bit accordingly
        }
        //bool check = (buf[index / 8] >> index% 8) & 0x1;
        //printf("test %d\n", buf[index/8]);
        //printf("current bit %d\n", code_get_bit(c, i));
        index++; //incrememnts index
    }
    if (index == BLOCK) {
        k = write_bytes(outfile, buf, BLOCK); //flushes the buffer if the BLOCK size is reached
        bytes_coded += k;
        index = 0;
    }
    //j = write_bytes(outfile, buf, BLOCK);
}

void flush_codes(
    int outfile) { //appends 0 to the remaining bits in the buffer in case the actual bits aren't evenly dividec BLOC times
    if (k < BLOCK) {
        for (uint32_t i = 0; i < BLOCK - bytes_coded; i++) {
            buf[index / 8] &= ~(0x1 << index % 8);
        }
        write_bytes(outfile, buf, BLOCK);
    }
}
