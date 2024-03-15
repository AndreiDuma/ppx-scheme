#ifndef PPX_PRINT_H
#define PPX_PRINT_H

#include <stdio.h>

#include "types.h"

void ppx_print(ppx_value_t *v);

void ppx_print_symbol(const ppx_symbol_t *s) { printf("%s", s->name); }

void ppx_print_boolean(const ppx_boolean_t *b) {
    b->value ? printf("#t") : printf("#f");
}

void ppx_print_integer(const ppx_integer_t *i) { printf("%lld", i->value); }

void ppx_print_cons(const ppx_cons_t *c, bool print_parens) {
    if (print_parens)
        printf("(");

    ppx_print(c->car);

    if (ppx_is_null(c->cdr)) {
	// Do nothing.
    } else if (ppx_is_cons(c->cdr)) {
	printf(" ");
        ppx_print_cons((ppx_cons_t *)(c->cdr), false);
    } else {
        printf(" . ");
        ppx_print(c->cdr);
    }

    if (print_parens)
        printf(")");
}

void ppx_print_null(void) { printf("()"); }

void ppx_print_eof(void) { printf("#<eof>"); }

void ppx_print(ppx_value_t *v) {
    switch (ppx_typeof(v)) {
    case PPX_TYPE_SYMBOL:
	ppx_print_symbol((ppx_symbol_t *)v);
	break;
    case PPX_TYPE_BOOLEAN:
	ppx_print_boolean((ppx_boolean_t *)v);
	break;
    case PPX_TYPE_INTEGER:
	ppx_print_integer((ppx_integer_t *)v);
	break;
    case PPX_TYPE_CONS:
	ppx_print_cons((ppx_cons_t *)v, true);
	break;
    case PPX_TYPE_NULL:
	ppx_print_null();
	break;
    case PPX_TYPE_EOF:
	ppx_print_eof();
    }
}

void ppx_println(ppx_value_t *v) {
    ppx_print(v);
    printf("\n");
}

#endif
