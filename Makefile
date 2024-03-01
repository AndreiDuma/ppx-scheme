all: ppx

ppx: ppx.c
	cc -o ppx ppx.c

.PHONY: clean
clean:
	rm ppx
