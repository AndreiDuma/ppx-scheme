#ifndef PPX_EVAL_H
#define PPX_EVAL_H

#include <stdio.h>

#include "types.h"

ppx_value_t *ppx_eval_lambda(ppx_cons_t *e) {
    // TODO
}

ppx_value_t *ppx_eval_application(ppx_cons_t *e) {
    // TODO
}

ppx_value_t *ppx_eval(ppx_value_t *e) {
    if (ppx_is_integer(e)) {
	return e;
    } else if (ppx_is_boolean(e)) {
	return e;
    } else if (ppx_is_symbol(e)) {
	return e;
    } else if (ppx_is_cons(e)) {
	ppx_cons_t *c = (ppx_cons_t *)c;
	if (c->car == ppx_symbol("quote")) {
	    return c->cdr;
	} else if (c->car == ppx_symbol("lambda")) {
	    return ppx_eval_lambda(c);
	} else
	    return ppx_eval_application(c);
    } else if (ppx_is_null(e)) {
	return e;
    } else if (ppx_is_eof(e)) {
	return e;
    } else {
	fprintf(stderr, "eval: unknown value type");
	exit(-1);
    }
}

#endif
