#include "io.h"
#include "huffman.h"
#include "code.h"
#include "stack.h"
#include "pq.h"
#include "node.h"
#include "defines.h"
#include "header.h"
#include "encode.h"

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
//line 122 derived from Professor Long: https://discord.com/channels/926211705766305842/926211706382868584/944400777508687872
//
double space_saved;
void usage(char *exec) { //prints out usage options to stderr
    fprintf(stderr,
        "SYNOPSIS\n"
        "	A Huffman encoder.\n"
        "	Compresses a file using the Huffman coding algorithm.\n"
        "USAGE\n"
        "	%s [OPTOINS]\n"
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
    int out = STDOUT_FILENO; //declaration of file descriptors that are stdin and stdout by default
    int count = 0;
    int comp_stats = 0; //variable for turninc comp_stats on and off

    uint64_t hist[ALPHABET];
    uint8_t buffer
        [BLOCK]; //histogram and buffer for getting frequencies of symbols and for reading data from the input file
    //uint8_t rebuff[BLOCK];
    for (int k = 0; k < ALPHABET; k++) { //initializes the histogram
        hist[k] = 0;
    }

    ssize_t j; //for keeping track of bytes actually read

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
        }
    } //switch statement for all the command line options

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
    } //conditionals to check if there was an error with the infile or outfile
    hist[0] = 1;
    hist[255] = 1; //sets both 0 and 255 to 1

    while ((j = read_bytes(in, buffer, BLOCK))
           > 0) { //reads symbols from the infile and increments the histogram accordingly
        for (int i = 0; i < j; i++) {
            hist[buffer[i]] += 1;
        }
    }
    Node *root = build_tree(hist); //calls build_tree in order to create the root node

    build_codes(root, table);
    for (int x = 0; x < ALPHABET; x++) { //takes the count for the number of symbols in the file
        if (hist[x] > 0) {
            count += 1;
        }
    }
    Header h;
    h.magic = MAGIC;
    fstat(in, &infile);
    h.permissions = infile.st_mode;
    fchmod(out, h.permissions);
    h.tree_size = (3 * count) - 1;
    h.file_size = infile.st_size;
    write_bytes(out, ((uint8_t *) &h),
        sizeof(h)); //takes all the header values to be written out and interpretted by the decoder
    dump_tree(out, root); //makes the tree dump

    lseek(in, 0, SEEK_SET); //resets the position in a file
    while ((j = read_bytes(in, buffer, BLOCK))
           > 0) { //reads the infile BLOCK bytes at a time, writing the codes to the code table
        for (int i = 0; i < j; i++) {
            write_code(out, &table[buffer[i]]);
        }
    }
    flush_codes(out);
    if (comp_stats == 1) { //prints stats if stats is turned on
        char per = '%';
        space_saved = 100 * (1 - (bytes_written / bytes_read));
        printf("\n\n\n");
        printf("Uncompressed file size: %lu bytes\n", bytes_read);
        printf("Compressed file size: %lu bytes\n", bytes_written);
        printf("Space saving: %f%c\n", space_saved / 100, per);
    }

    delete_tree(&root); //deletes tree and closes files
    close(in);
    close(out);
}
