CRYPTSRCS=../src/crypt/enc_type_*.lo ../src/crypt/fst_crypt.c ../src/md5.c
CFLAGS=-g -O2 -Wall -I../src/crypt -I../src $(CRYPTSRCS)

all: xor filter sniff

sniff: sniff.c
	$(CC) $(CFLAGS) sniff.c -o sniff -lnids -lnet -lpcap 
