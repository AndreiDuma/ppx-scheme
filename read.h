#ifndef PPX_READ_H
#define PPX_READ_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "list.h"

/* int fpeek(FILE *stream) { */
/*     const int c = fgetc(stream); */
/*     ungetc(c, stream); */

/*     return c; */
/* } */

typedef enum ppx_token_type {
    PPX_TOKEN_ERROR,
    PPX_TOKEN_EOF,
    PPX_TOKEN_LEFT_PAREN,
    PPX_TOKEN_RIGHT_PAREN,
    PPX_TOKEN_QUOTE,
    PPX_TOKEN_QUASIQUOTE,
    PPX_TOKEN_UNQUOTE,
    PPX_TOKEN_UNQUOTE_SPLICING,
    PPX_TOKEN_NUMERIC,
    PPX_TOKEN_IDENTIFIER,
} ppx_token_type_t;

const size_t PPX_MAX_TOKEN_LENGTH = 256;

ppx_token_type_t ppx_read_token(FILE *in, char *token) {
    int c;
    // First, skip all whitespace.
    do {
	c = fgetc(in);
    } while (isspace(c));

    switch (c) {
    case EOF:
        return PPX_TOKEN_EOF;
    case '(':
        return PPX_TOKEN_LEFT_PAREN;
    case ')':
        return PPX_TOKEN_RIGHT_PAREN;
    case '\'':
        return PPX_TOKEN_QUOTE;
    case '`':
        return PPX_TOKEN_QUASIQUOTE;
    case ',': {
        c = getc(stdin);
        if (c == '@') {
            return PPX_TOKEN_UNQUOTE_SPLICING;
        }
        // Simple unquote.
        ungetc(c, in);
        return PPX_TOKEN_UNQUOTE;
    }
    default: {
        // This is the start of a "word", either a number or an identifier.
        int i = 0;
        bool is_numeric = true;
        while (true) {
            // Store the current character.
            token[i++] = c;
            if (i >= PPX_MAX_TOKEN_LENGTH) {
                return PPX_TOKEN_ERROR;
            }

            // Distinguish identifiers from numbers;
            if (!isdigit(c)) {
		is_numeric = false;
            }

            // Read the next character.
	    c = getc(stdin);

	    // Stop if the character cannot be part of a valid number or identifier.
	    if (c == EOF) {
		break;
	    }
	    if (isspace(c) || strchr("()'`,#", c)) {
		ungetc(c, stdin);
		break;
	    }
        }
        token[i] = '\0';
	return is_numeric ? PPX_TOKEN_NUMERIC : PPX_TOKEN_IDENTIFIER;
    }
    }
}

ppx_value_t *ppx_read_into(FILE *in, ppx_value_t *list) {
    ppx_value_t *ret;
    while (true) {
        char token[PPX_MAX_TOKEN_LENGTH];
        switch (ppx_read_token(in, token)) {
        case PPX_TOKEN_ERROR: {
            fprintf(stderr, "max token length exceeded\n");
            exit(-1);
        }
	case PPX_TOKEN_EOF: {
	    return PPX_EOF;
	}
        case PPX_TOKEN_LEFT_PAREN: {
            // Call recursively, creating an implicit stack.
            ret = ppx_read_into(in, PPX_NULL);
            break;
        }
        case PPX_TOKEN_RIGHT_PAREN: {
            return ppx_list_reverse(list);
        }
	case PPX_TOKEN_QUOTE: {
	    // Read another datum and wrap it in a quote.
	    ppx_value_t *next = ppx_read_into(in, list);
	    ret = ppx_cons(ppx_symbol("quote"), next);
	    break;
	}
        case PPX_TOKEN_NUMERIC: {
            const int64_t value = atol(token);
            ret = ppx_integer(value);
            break;
        }
        case PPX_TOKEN_IDENTIFIER: {
            char *name = strndup(token, PPX_MAX_TOKEN_LENGTH);
            ret = ppx_symbol(name);
            break;
        }
        default: {
            fprintf(stderr, "read: uninplemented token");
            exit(-1);
        }
        }
        if (list == NULL) {
            // "Top-level" value.
            return ret;
        }
        // We're reading inside a list.
        list = ppx_cons(ret, list);
    }
}

ppx_value_t *ppx_read(FILE *in) {
    return ppx_read_into(in, NULL);
}

#endif
