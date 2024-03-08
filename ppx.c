#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Types
 */
typedef enum ppx_type {
    PPX_TYPE_SYMBOL,
    PPX_TYPE_BOOLEAN,
    PPX_TYPE_INTEGER,
    PPX_TYPE_CONS,
    PPX_TYPE_NULL,
} ppx_type_t;

/**
 * Values.
 */
typedef struct ppx_value ppx_value_t;

ppx_type_t ppx_typeof(ppx_value_t *v) { return *((ppx_type_t *)v); }

/**
 * Symbols.
 */
typedef struct ppx_symbol {
    ppx_type_t type;
    char *name;
} ppx_symbol_t;

ppx_value_t *ppx_symbol(char *name) {
    ppx_symbol_t *s = malloc(sizeof(*s));
    s->type = PPX_TYPE_SYMBOL;
    s->name = name;
    return (ppx_value_t *)s;
}

bool ppx_is_symbol(ppx_value_t *v) { return ppx_typeof(v) == PPX_TYPE_SYMBOL; }

/**
 * Booleans.
 */
typedef struct ppx_boolean {
    ppx_type_t type;
    bool value;
} ppx_boolean_t;

ppx_value_t *ppx_boolean(const bool value) {
    ppx_boolean_t *b = malloc(sizeof(*b));
    b->type = PPX_TYPE_BOOLEAN;
    b->value = value;
    return (ppx_value_t *)b;
}

bool ppx_is_boolean(ppx_value_t *v) {
    return ppx_typeof(v) == PPX_TYPE_BOOLEAN;
}

/**
 * Integers.
 */
typedef struct ppx_integer {
    ppx_type_t type;
    int64_t value;
} ppx_integer_t;

ppx_value_t *ppx_integer(const int64_t value) {
    ppx_integer_t *i = malloc(sizeof(*i));
    i->type = PPX_TYPE_INTEGER;
    i->value = value;
    return (ppx_value_t *)i;
}

bool ppx_is_integer(ppx_value_t *v) {
    return ppx_typeof(v) == PPX_TYPE_INTEGER;
}

/**
 * Conses (aka pairs).
 */
typedef struct ppx_cons {
    ppx_type_t type;
    ppx_value_t *car;
    ppx_value_t *cdr;
} ppx_cons_t;

ppx_value_t *ppx_cons(ppx_value_t *car, ppx_value_t *cdr) {
    ppx_cons_t *c = malloc(sizeof(*c));
    c->type = PPX_TYPE_CONS;
    c->car = car;
    c->cdr = cdr;
    return (ppx_value_t *)c;
}

bool ppx_is_cons(ppx_value_t *v) { return ppx_typeof(v) == PPX_TYPE_CONS; }

/**
 * Null.
 */
typedef struct ppx_null {
    ppx_type_t type;
} ppx_null_t;

const ppx_value_t *PPX_NULL = (ppx_value_t *)&(ppx_null_t){PPX_TYPE_NULL};

ppx_value_t *ppx_null(void) { return (ppx_value_t *)PPX_NULL; }

bool ppx_is_null(ppx_value_t *v) { return ppx_typeof(v) == PPX_TYPE_NULL; }

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
    ppx_value_t *prev = ppx_null(), *next;
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

/**
 * Printing.
 */
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
    }
}

void ppx_println(ppx_value_t *v) {
    ppx_print(v);
    printf("\n");
}

/**
 * Parsing.
 */
int fpeek(FILE *stream) {
    const int c = fgetc(stream);
    ungetc(c, stream);

    return c;
}

typedef enum ppx_token_type {
    PPX_TOKEN_ERROR,
    PPX_TOKEN_EOF,
    PPX_TOKEN_LEFT_PAREN,
    PPX_TOKEN_RIGHT_PAREN,
    PPX_TOKEN_QUOTE,
    PPX_TOKEN_QUASIQUOTE,
    PPX_TOKEN_UNQUOTE,
    PPX_TOKEN_UNQUOTE_SPLICE,
    PPX_TOKEN_NUMERIC,
    PPX_TOKEN_IDENTIFIER,
} ppx_token_type_t;

const size_t PPX_MAX_TOKEN_LENGTH = 256;

ppx_token_type_t ppx_read_token(char *token) {
    int c;
    // First, skip all whitespace.
    do {
	c = getc(stdin);
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
            return PPX_TOKEN_UNQUOTE_SPLICE;
        }
        // Simple unquote.
        ungetc(c, stdin);
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

ppx_value_t *ppx_read(ppx_value_t *list) {
    ppx_value_t *ret;
    while (true) {
	char token[PPX_MAX_TOKEN_LENGTH];
	switch (ppx_read_token(token)) {
	case PPX_TOKEN_ERROR: {
	    fprintf(stderr, "max token length exceeded\n");
	    exit(-1);
	}
	case PPX_TOKEN_LEFT_PAREN: {
	    // Call recursively, creating an implicit stack.
	    ret = ppx_read(ppx_null());
	    break;
	}
	case PPX_TOKEN_RIGHT_PAREN: {
	    return ppx_list_reverse(list);
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
	}
	if (list == NULL) {
	    // "Top-level" value.
	    return ret;
	}
	// We're reading inside a list.
	list = ppx_cons(ret, list);
    }
}

int main() {
    ppx_value_t *i = ppx_integer(3);
    ppx_println(i);

    ppx_value_t *c = ppx_cons(i, ppx_integer(4));
    ppx_println(c);

    ppx_value_t *c2 = ppx_cons(ppx_integer(2), c);
    ppx_println(c2);

    ppx_value_t *l = ppx_cons(ppx_integer(2),
			      ppx_cons(ppx_integer(3),
				       ppx_null()));
    printf("%lld\n", ppx_list_length(l));
    ppx_println(l);
    ppx_println(ppx_list_reverse(l));

    printf("some reading...\n");

    // char token[PPX_MAX_TOKEN_LENGTH];
    // ppx_token_type_p tt = ppx_read_token(token);
    // printf("token: type=%d chars=<%s>\n", tt, token);

    ppx_value_t *x = ppx_read(NULL);
    ppx_println(x);

    return 0;
}
