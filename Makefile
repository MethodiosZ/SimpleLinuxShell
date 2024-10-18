all:	hy345sh

hy345sh:	hy345sh.c
		gcc hy345sh.c -o hy345sh

clean:
	rm -f hy345sh
	clear
