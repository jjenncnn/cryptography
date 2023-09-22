#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <gmp.h>
#include "numtheory.h"
#include "randstate.h"

void gcd(mpz_t g, const mpz_t a, const mpz_t b) {
    mpz_t t, bb, aa;
    mpz_inits(t, bb, aa, NULL);
    mpz_set(bb, b);
    mpz_set(aa, a);
    while (mpz_cmp_ui(bb, 0) != 0) {
        mpz_set(t, bb);
        mpz_mod(bb, aa, bb);
        mpz_set(aa, t);
    }
    mpz_set(g, aa);
    mpz_clears(t, bb, aa, NULL);
}

void mod_inverse(mpz_t o, const mpz_t a, const mpz_t n) {
    mpz_t r, rr, t, tt, q, temp, temp2;
    mpz_inits(r, rr, t, tt, q, temp, temp2, NULL);
    mpz_set(r, n);
    mpz_set(rr, a);
    mpz_set_ui(tt, 1);
    while (mpz_cmp_ui(rr, 0) != 0) {
        mpz_fdiv_q(q, r, rr);
        mpz_set(temp, r);
        mpz_set(r, rr);
        mpz_mul(temp2, q, rr);
        mpz_sub(rr, temp, temp2);
        mpz_set(temp, t);
        mpz_set(t, tt);
        mpz_mul(temp2, q, tt);
        mpz_sub(tt, temp, temp2);
    }
    if (mpz_cmp_ui(r, 1) > 0) {
        mpz_set_ui(t, 0);
    }
    if (mpz_cmp_ui(t, 0) < 0) {
        mpz_add(t, t, n);
    }
    mpz_set(o, t);
    mpz_clears(r, rr, t, tt, q, temp, temp2, NULL);
}

void pow_mod(mpz_t o, const mpz_t a, const mpz_t d, const mpz_t n) {
    mpz_t v, p, dd;
    mpz_inits(v, p, dd, NULL);
    mpz_set_ui(v, 1);
    mpz_set(p, a);
    mpz_set(dd, d);
    while (mpz_cmp_ui(dd, 0) > 0) {
        if (mpz_odd_p(dd) != 0) {
            mpz_mul(v, v, p);
            mpz_mod(v, v, n);
        }
        mpz_mul(p, p, p);
        mpz_mod(p, p, n);
        mpz_fdiv_q_ui(dd, dd, 2);
    }
    mpz_set(o, v);
    mpz_clears(v, p, dd, NULL);
}

bool is_prime(const mpz_t n, uint64_t iters) {
    if (mpz_cmp_ui(n, 1) <= 0) {
        return false;
    }
    if (mpz_cmp_ui(n, 4) == 0) {
        return false;
    }
    if (mpz_cmp_ui(n, 3) <= 0) {
        return true;
    }
    mpz_t r, a, y, j, nn, nsub, ssub, s;
    mpz_inits(r, a, y, j, nn, nsub, ssub, s, NULL);

    mpz_sub_ui(r, n, 1);

    while (mpz_odd_p(r) == 0) {
        mpz_fdiv_q_ui(r, r, 2);
        mpz_add_ui(s, s, 1);
    }

    mpz_sub_ui(nsub, n, 1);
    mpz_sub_ui(ssub, s, 1);

    for (uint64_t i = 1; i < iters; i++) {
        mpz_sub_ui(nn, n, 3);
        mpz_urandomm(a, state, nn);
        mpz_add_ui(a, a, 2);
        pow_mod(y, a, r, n);

        if (mpz_cmp_ui(y, 1) != 0 && mpz_cmp(y, nsub) != 0) {
            mpz_set_ui(j, 1);

            while (mpz_cmp(j, ssub) <= 0 && mpz_cmp(y, nsub) != 0) {
                mpz_mul(y, y, y);
                mpz_mod(y, y, n);

                if (mpz_cmp_ui(y, 1) == 0) {
                    mpz_clears(r, a, y, j, nn, nsub, ssub, s, NULL);
                    return false;
                }
                mpz_add_ui(j, j, 1);
            }

            if (mpz_cmp(y, nsub) != 0) {
                mpz_clears(r, a, y, j, nn, nsub, ssub, s, NULL);
                return false;
            }
        }
    }
    mpz_clears(r, a, y, j, nn, nsub, ssub, s, NULL);
    return true;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    mpz_t temp, pow, t;
    mpz_inits(temp, pow, t, NULL);
    mpz_set_ui(t, 2);
    uint64_t s = bits - 1;
    while (is_prime(temp, iters) == false) {
        mpz_urandomb(temp, state, bits);
        mpz_pow_ui(pow, t, s);
        mpz_add(temp, temp, pow);
    }
    mpz_set(p, temp);
    mpz_clears(temp, pow, t, NULL);
}
