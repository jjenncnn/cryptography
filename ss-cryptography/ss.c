#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include "randstate.h"
#include "numtheory.h"

void ss_make_pub(mpz_t p, mpz_t q, mpz_t n, uint64_t nbits, uint64_t iters) {
    uint64_t pbits = 0;
    uint64_t qbits = 0;
    uint64_t max = (2 * nbits) / 5;
    uint64_t min = nbits / 5;
    mpz_t m1, m2, r1, r2;
    mpz_inits(m1, m2, r1, r2, NULL);
    pbits = random() % (max - min) + min;
    qbits = nbits - (2 * pbits);
    make_prime(p, pbits, iters);
    make_prime(q, qbits, iters);
    mpz_sub_ui(m1, q, 1);
    mpz_sub_ui(m2, p, 1);
    mpz_mod(r1, p, m1);
    mpz_mod(r2, q, m2);
    while (mpz_cmp_ui(r1, 0) == 0 && mpz_cmp_ui(r2, 0) == 0) {
        pbits = random() % (max - min) + min;
        qbits = nbits - (2 * pbits);
        make_prime(p, pbits, iters);
        make_prime(q, qbits, iters);
        mpz_sub_ui(m1, q, 1);
        mpz_sub_ui(m2, p, 1);
        mpz_mod(r1, p, m1);
        mpz_mod(r2, q, m2);
    }
    mpz_mul(n, p, p);
    mpz_mul(n, n, q);
    mpz_clears(m1, m2, r1, r2, NULL);
}

void ss_make_priv(mpz_t d, mpz_t pq, const mpz_t p, const mpz_t q) {
    mpz_t n, lampq, num, div, a, b;
    mpz_inits(n, lampq, num, div, a, b, NULL);
    mpz_mul(n, p, p);
    mpz_mul(n, n, q);
    mpz_sub_ui(a, p, 1);
    mpz_sub_ui(b, q, 1);
    mpz_mul(num, a, b);
    gcd(div, a, b);
    mpz_fdiv_q(lampq, num, div);
    mod_inverse(d, n, lampq);
    mpz_mul(pq, p, q);
    mpz_clears(n, lampq, num, div, a, b, NULL);
}

void ss_write_pub(const mpz_t n, const char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n", n);
    gmp_fprintf(pbfile, "%s\n", username);
}

void ss_write_priv(const mpz_t pq, const mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n", pq);
    gmp_fprintf(pvfile, "%Zx\n", d);
}

void ss_read_pub(mpz_t n, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n", n);
    gmp_fscanf(pbfile, "%s\n", username);
}

void ss_read_priv(mpz_t pq, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n", pq);
    gmp_fscanf(pvfile, "%Zx\n", d);
}

void ss_encrypt(mpz_t c, const mpz_t m, const mpz_t n) {
    pow_mod(c, m, n, n);
}

void ss_encrypt_file(FILE *infile, FILE *outfile, const mpz_t n) {
    mpz_t m, c;
    mpz_inits(m, c, NULL);
    size_t j;
    uint64_t k = ((mpz_sizeinbase(n, 2) / 2) - 1) / 8;
    uint8_t *arr = (uint8_t *) calloc(k, sizeof(uint8_t));
    arr[0] = 0xFF;
    while ((j = fread(&arr[1], sizeof(uint8_t), k - 1, infile)) > 0) {
        mpz_import(m, j + 1, 1, sizeof(uint8_t), 1, 0, arr);
        ss_encrypt(c, m, n);
        gmp_fprintf(outfile, "%Zx \n", c);
    }
    free(arr);
    mpz_clears(m, c, NULL);
}

void ss_decrypt(mpz_t m, const mpz_t c, const mpz_t d, const mpz_t pq) {
    pow_mod(m, c, d, pq);
}

void ss_decrypt_file(FILE *infile, FILE *outfile, const mpz_t d, const mpz_t pq) {
    mpz_t m, c;
    mpz_inits(m, c, NULL);
    size_t j;
    uint64_t k = (mpz_sizeinbase(pq, 2) - 1) / 8;
    uint8_t *arr = (uint8_t *) calloc(k, sizeof(uint8_t));
    while (gmp_fscanf(infile, "%Zx \n", c) == 1) {
        ss_decrypt(m, c, d, pq);
        mpz_export(arr, &j, 1, sizeof(uint8_t), 1, 0, m);
        fwrite(&arr[1], sizeof(uint8_t), j - 1, outfile);
    }
    mpz_clears(m, c, NULL);
    free(arr);
}
