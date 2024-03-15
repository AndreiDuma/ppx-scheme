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
    // Read-Print-Eval Loop.
    while (true) {
	// Read.
	ppx_value_t *e = ppx_read();

	// Eval.
	ppx_value_t *r = ppx_eval(e);

	// Print.
	ppx_print(r);
    }
    
    ppx_value_t *i = ppx_integer(3);
    ppx_println(i);

    ppx_value_t *c = ppx_cons(i, ppx_integer(4));
    ppx_println(c);

    ppx_value_t *c2 = ppx_cons(ppx_integer(2), c);
    ppx_println(c2);

    ppx_value_t *l = ppx_cons(ppx_integer(2),
			      ppx_cons(ppx_integer(3),
				       PPX_NULL));
    printf("%lld\n", ppx_list_length(l));
    ppx_println(l);
    ppx_println(ppx_list_reverse(l));

    printf("some reading...\n");

    // char token[PPX_MAX_TOKEN_LENGTH];
    // ppx_token_type_p tt = ppx_read_token(token);
    // printf("token: type=%d chars=<%s>\n", tt, token);

    ppx_value_t *x = ppx_read();
    ppx_println(x);

    return 0;
}
