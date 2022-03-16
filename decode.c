#include "io.h"
#include "huffman.h"
#include "code.h"
#include "stack.h"
#include "pq.h"
#include "node.h"
#include "defines.h"
#include "header.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

#define OPTIONS "i:o:vh"
#define PERMS   0666
//Line 86 derived from Benjamin Grant(TA): https://discord.com/channels/926211705766305842/926211706022162469/948381041993867304
//Line 115 inspired by Aidan Bacon(Student, CSE13s Winter 2022)
double space_saved;
void usage(char *exec) { //prints out usage options to stderr
    fprintf(stderr,
        "SYNOPSIS\n"
        "       A Huffman encoder.\n"
        "       Compresses a file using the Huffman coding algorithm.\n"
        "USAGE\n"
        "       %s [OPTOINS]\n"
        "OPTIONS\n"
        "-h             Program usage and help.\n"
        "-v             Print compression statistics.\n"
        "-i infile      Input file to compress.\n"
        "-o outfile     Output of compressed data.\n",
        exec);
}

int main(int argc, char **argv) {
    int opt = 0;
    struct stat infile;
    //char *outfile = NULL;
    int in = STDIN_FILENO;
    int out = STDOUT_FILENO; //declaration of file descriptors which are stdin and stdout by default
    //int count = 0;
    int comp_stats = 0;

    Header read; //header to read the header from the encoder
    //ssize_t j;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': {
            in = open(optarg, O_RDWR);
            fstat(in, &infile);
            break;
        }
        case 'o': {
            out = open(optarg, O_CREAT, O_RDWR);
            break;
        }
        case 'v': {
            comp_stats = 1;
            break;
        }
        case 'h': {
            usage(argv[0]);
            close(in);
            close(out);
            exit(0);
            break;
        }
        } //switch statement for command line options
    }
    if (in == -1) {
        printf("Error opening file\n");
        close(in);
        close(out);
        exit(0);
    }
    if (out == -1) {
        puts("Error creating file");
        close(in);
        close(out);
        exit(0);
    } //conditionals for checking if there was an error with the in and outfile
    read_bytes(in, ((uint8_t *) &read),
        sizeof(Header)); //reads in the header while typecasting the header to a uint8_t
    //printf("magic %x\n", read.magic);

    if (read.magic
        != MAGIC) { //checks the valididty of the encoder file by checking if it matches the magic number
        puts("Error, invalid file has been passed");
        close(in);
        close(out);
        exit(0);
    }
    uint8_t tree[read.tree_size]; //declaration of the tree array that will be used in roder to

    fchmod(out, read.permissions); //gets the file permissions specified by the encoder
    for (uint64_t s = 0; s < read.tree_size; s++) { //initializes the tree
        tree[s] = 0;
    }
    read_bytes(in, tree, read.tree_size); //reads in the tree dump using the tree array

    Node root = *rebuild_tree(read.tree_size,
        tree); //initializes the root node to rebuild_tree, which looks at the tree dump and creates a root node
    uint8_t bit;
    uint64_t count = 0;

    Node *walk = &root; //temp node made to walk the tree when decompressing the encoded file

    while (
        count
        != read.file_size) { //loop to check if the counters reached the header file size specified in encode
        //puts("test1");
        bool check = read_bit(in, &bit); //boolean to check if a valid bit was read

        while (true) {
            if (check
                == true) { //only proceeds with walking and writing if a valid bit was received
                if (walk->left == NULL
                    && walk->right == NULL) { //writes to outfile if a leaf node is reached
                    //puts("SYMBOL Here");
                    //printf("current symbol %c\n", walk->symbol);
                    write_bytes(out, &walk->symbol, sizeof(uint8_t));
                    //puts("symbol");
                    walk = &root;
                    count++;
                    break;
                }
                if (bit == 0) { //walks left if the read bit 0
                    walk = walk->left;
                    //puts("left");
                } else if (bit == 1) { //walks right if the read bit is 1
                    walk = walk->right;
                    //puts("right");
                }
            } else { //breaks if an invalid bit is read
                break;
            }
        }
    }
    close(in); //closes infile and outfile
    close(out);
}
