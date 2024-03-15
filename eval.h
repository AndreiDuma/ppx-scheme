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
	ppx_cons_t *e = (ppx_cons_t *)e;
	if (e->car == ppx_symbol("lambda")) {
	    return ppx_eval_lambda(e);
	} else
	    return ppx_eval_application(e);
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
