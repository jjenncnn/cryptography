#include <stdio.h>
#include <gmp.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "randstate.h"
#include "numtheory.h"
#include "ss.h"

#define OPTIONS "b:i:n:d:s:vh"

int main(int argc, char **argv) {
    int opt = 0;
    uint64_t bits = 256;
    uint64_t iters = 50;
    FILE *pub;
    FILE *priv;
    uint64_t seed = time(NULL);
    bool out = false;
    bool puf = false;
    bool prf = false;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'b': bits = (uint64_t) strtoul(optarg, NULL, 10); break;

        case 'i': iters = (uint64_t) strtoul(optarg, NULL, 10); break;

        case 'n':
            puf = true;
            pub = fopen(optarg, "w");
            break;

        case 'd':
            prf = true;
            priv = fopen(optarg, "w");
            break;

        case 's': seed = (uint64_t) strtoul(optarg, NULL, 10); break;

        case 'v': out = true; break;

        case 'h':
            printf("SYNOPSIS\n");
            printf("Generates an SS public/private key pair.\n");
            printf("\nUSAGE\n   ./keygen [bindsvh]\n");
            printf("\nOPTIONS\n");
            printf("   -h              Display program help and usage.\n");
            printf("   -v              Display verbose program output.\n");
            printf("   -b bits         Minimum bits needed for public key n (default: 256).\n");
            printf(
                "   -i iterations   Miller-Rabin iterations for testing primes (default: 50).\n");
            printf("   -n pbfile       Public key file (default: ss.pub).\n");
            printf("   -d pvfile       Private key file (default: ss.priv).\n");
            printf("   -s seed         Random seed for testing.\n");
            return 0;
        }
    }
    if (puf == false) {
        pub = fopen("ss.pub", "w");
    }
    if (prf == false) {
        priv = fopen("ss.priv", "w");
    }
    fchmod(fileno(priv), S_IRUSR | S_IWUSR);
    randstate_init(seed);
    mpz_t pubkey, privkey, p, q, n, d, pq;
    mpz_inits(pubkey, privkey, p, q, n, d, pq, NULL);
    ss_make_pub(p, q, n, bits, iters);
    ss_make_priv(d, pq, p, q);
    char *user;
    user = getenv("USER");
    ss_write_pub(n, user, pub);
    ss_write_priv(pq, d, priv);
    if (out == true) {
        printf("user = %s\n", user);
        gmp_printf("p (%d bits) = %Zd\n", mpz_sizeinbase(p, 2), p);
        gmp_printf("q (%d bits) = %Zd\n", mpz_sizeinbase(q, 2), q);
        gmp_printf("n (%d bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("d (%d bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
        gmp_printf("pq (%d bits) = %Zd\n", mpz_sizeinbase(pq, 2), pq);
    }
    fclose(pub);
    fclose(priv);
    randstate_clear();
    mpz_clears(pubkey, privkey, p, q, n, d, pq, NULL);
}
