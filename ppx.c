#include <stdio.h>

#include "types.h"
#include "read.h"
#include "eval.h"
#include "print.h"

int main() {
    do {
	// Read.
	printf("> ");
	ppx_value_t *e = ppx_read(stdin);
	if (ppx_is_eof(e)) {
	    break;
	}

	// Eval.
	ppx_value_t *r = ppx_eval(e);

	// Print.
	ppx_println(r);

	// Loop.
    } while (true);
    
    return 0;
}
