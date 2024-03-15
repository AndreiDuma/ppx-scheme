#ifndef PPX_LOAD_H
#define PPX_LOAD_H

#include "types.h"
#include "read.h"
#include "eval.h"

#include <stdio.h>

void ppx_load(const char *filename) {
    FILE *in = fopen(filename, "r");

    while (true) {
	ppx_value_t *e = ppx_read(in);
	if (ppx_is_eof(e)) {
	    break;
	}
	ppx_eval(e);
    }
    
    fclose(in);
}

#endif
