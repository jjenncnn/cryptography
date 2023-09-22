CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic
LDFLAGS = -lm -lgmp
ENCRYPT_OBJS = encrypt.o ss.o numtheory.o randstate.o
DECRYPT_OBJS = decrypt.o ss.o numtheory.o randstate.o
KEYGEN_OBJS = keygen.o ss.o numtheory.o randstate.o

all: keygen encrypt decrypt

keygen: $(KEYGEN_OBJS)
	$(CC) $(LDFLAGS) -o keygen $(KEYGEN_OBJS)

encrypt: $(ENCRYPT_OBJS)
	$(CC) $(LDFLAGS) -o encrypt $(ENCRYPT_OBJS)

decrypt: $(DECRYPT_OBJS)
	$(CC) $(LDFLAGS) -o decrypt $(DECRYPT_OBJS)

debug: CFLAGS+= -g -gdwarf-4
debug: keygen encrypt decrypt

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f keygen encrypt decrypt $(KEYGEN_OBJS) $(ENCRYPT_OBJS) $(DECRYPT_OBJS)

format:
	clang-format -i -style=file *.[ch]

scan-build: clean
	scan-build --use-cc=$(CC) make
