#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "read.h"
#include "eval.h"
#include "print.h"

int main() {
    do {
	// Read.
	printf("> ");
	ppx_value_t *e = ppx_read();
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
