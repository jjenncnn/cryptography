# SS Cryptography

This is a cryptography program consisting of three programs: keygen, encrypt, and decrypt. keygen creates the public and private SS keys using the Schmidt-Samoa (SS) encryption algorithm. encrypt will encrypt the given message into hexstrings, and decrypt will decrypt the encrypted message back to plaintext.

## Files Included:
- randstate.c: Implements the random state interface for the SS library and number theory functions.

- randstate.h: Header file for randstate.c.

- numtheory.c: Implements the interface for number theory functions.

- numtheory.h: Header file for numtheory.c.

- ss.c: Implements the SS library.

- ss.h: header file for ss.c.

- keygen.c: Implements the keygen program.

- encrypt.c: Implements the encrypt program.

- decrypt.c: Implements the decrypt program.

- Makefile: Used for compiling the program.

- README.md: This file.

## Build:
To build all executables:
 - $ make (or) $ make all
 
To build keygen:
 - $ make keygen
 
To build encrypt:
 - $ make encrypt
 
To build decrypt:
 - $ make decrypt
 
 To run programs:
 - $ ./(program_name) -options

## Debugging:
Run 'make debug' to compile the program with debug flags. Add 'valgrind' to the front of the file execution command to run the file with valgrind to check for leaks and errors.

## Cleaning:
To clean the directory after building, type 'make clean' into the command line.
