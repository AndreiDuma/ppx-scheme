#ifndef PPX_LIST_H
#define PPX_LIST_H

#include <stdio.h>

#include "types.h"

/**
 * List utilities.
 */
bool ppx_is_list(ppx_value_t *v) {
    // TODO: implement iteratively.
    if (ppx_is_null(v))
	return true;
    if (ppx_is_cons(v))
	return ppx_is_list(((ppx_cons_t *)v)->cdr);
    return false;
}

int64_t ppx_list_length(ppx_value_t *v) {
    int64_t length = 0;
    while (true) {
	if (ppx_is_null(v))
	    return length;
	if (ppx_is_cons(v)) {
	    length += 1;
	    v = ((ppx_cons_t *)v)->cdr;
	    continue;
	}
	fprintf(stderr, "length: invalid list\n");
	exit(-1);
    }
}

ppx_value_t *ppx_list_reverse(ppx_value_t *list) {
    ppx_value_t *prev = PPX_NULL, *next;
    while (true) {
	if (ppx_is_null(list))
	    return prev;
	if (ppx_is_cons(list)) {
	    next = ((ppx_cons_t *)list)->cdr;
	    ((ppx_cons_t *)list)->cdr = prev;
	    prev = list;
	    list = next;
	    continue;
	}
	fprintf(stderr, "reverse: invalid list\n");
	exit(-1);
    }
}

#endif
