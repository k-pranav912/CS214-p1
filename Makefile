ww:
	gcc -fsanitize=address,undefined -Wall -Werror ww.c -o ww

clean:
	rm -f ww
