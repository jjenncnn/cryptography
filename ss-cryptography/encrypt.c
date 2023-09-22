#include <stdio.h>
#include <gmp.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include "randstate.h"
#include "numtheory.h"
#include "ss.h"

#define OPTIONS "i:o:n:vh"

int main(int argc, char **argv) {
    int opt = 0;
    FILE *in = stdin;
    FILE *out = stdout;
    FILE *key;
    bool info = false;
    bool inf = false;
    bool outf = false;
    bool keyf = false;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            inf = true;
            in = fopen(optarg, "r");
            break;

        case 'o':
            outf = true;
            out = fopen(optarg, "w");
            break;

        case 'n':
            keyf = true;
            key = fopen(optarg, "r");
            break;

        case 'v': info = true; break;

        case 'h':
            printf("SYNOPSIS\n");
            printf("   Encrypts data using SS encryption.\n");
            printf("   Encrypted data is decrypted by the decrypt program.\n");
            printf("\nUSAGE\n   ./encrypt [ionvh]\n");
            printf("\nOPTIONS\n");
            printf("   -h              Display program help and usage.\n");
            printf("   -v              Display verbose program output.\n");
            printf("   -i infile       Input file of data to encrypt (default: stdin).\n");
            printf("   -o outfile      Output file for encrypted data (default: stdout).\n");
            printf("   -n pbfile       Public key file (default: ss.pub).\n");
            return 0;
        }
    }
    if (keyf == false) {
        key = fopen("ss.pub", "r");
    }
    mpz_t n;
    mpz_init(n);
    char *username = (char *) calloc(UCHAR_MAX, sizeof(char));
    ss_read_pub(n, username, key);
    if (info == true) {
        printf("user = %s\n", username);
        gmp_printf("n (%d bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
    }
    ss_encrypt_file(in, out, n);
    fclose(key);
    if (inf == true) {
        fclose(in);
    }
    if (outf == true) {
        fclose(out);
    }
    mpz_clear(n);
    free(username);
}
