CC = gcc
CCFLAGS = -Wall -Werror

all: vdecrypt vencrypt scrypt sbdecrypt sbencrypt 

vencrypt: vencrypt.c
	$(CC) $(CCFLAGS) -o vencrypt vencrypt.c

vdecrypt: vdecrypt.c
	$(CC) $(CCFLAGS) -o vdecrypt vdecrypt.c

scrypt: scrypt.c
	$(CC) $(CCFLAGS) -o scrypt scrypt.c

sbdecrypt: sbdecrypt.c
	$(CC) $(CCFLAGS) -o sbdecrypt sbdecrypt.c

sbencrypt: sbencrypt.c
	$(CC) $(CCFLAGS) -o sbencrypt sbencrypt.c

clean:
	rm -f vencrypt vdecrypt scrypt sbencrypt sbdecrypt