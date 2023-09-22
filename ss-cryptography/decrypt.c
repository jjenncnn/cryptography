#include <stdio.h>
#include <gmp.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
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
    mpz_t pq;
    mpz_t d;
    mpz_init(pq);
    mpz_init(d);

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
            printf("   Decrypts data using SS decryption.\n");
            printf("   Encrypted data is encrypted by the encrypt program.\n");
            printf("\nUSAGE\n   ./decrypt [ionvh]\n");
            printf("\nOPTIONS\n");
            printf("   -h              Display program help and usage.\n");
            printf("   -v              Display verbose program output.\n");
            printf("   -i infile       Input file of data to decrypt (default: stdin).\n");
            printf("   -o outfile      Output file for decrypted data (default: stdout).\n");
            printf("   -n pvfile       Private key file (default: ss.priv).\n");
            return 0;
        }
    }

    if (keyf == false) {
        key = fopen("ss.priv", "r");
    }

    ss_read_priv(pq, d, key);
    if (info == true) {
        gmp_printf("pq (%d bits) = %Zd\n", mpz_sizeinbase(pq, 2), pq);
        gmp_printf("d (%d bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
    }
    ss_decrypt_file(in, out, d, pq);
    fclose(key);
    if (inf == true) {
        fclose(in);
    }
    if (outf == true) {
        fclose(out);
    }
    mpz_clear(pq);
    mpz_clear(d);
}
