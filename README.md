# Assignment 6 - Huffman Coding
These programs implement David Huffman's lossless compression algorithm using Shanon's entropy. It is implemented using sevaral differnt modules that are used to build a huffman tree, encode said tree, dump the tree, rebuild the tree, and decompress the file.

NOTE: The decode binary does not sucessfully decode the tree dump from the encoder at the moment. 

# Building

Run the following to build the 'encode' and 'decode' executables
```
make
```

or 

```
make all
```

Run the following to delete executables and objects

```
make clean
```

# Running encode
```
./encode
```

# Running decode
```
./decode
```

