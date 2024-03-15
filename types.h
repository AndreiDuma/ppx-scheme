#ifndef PPX_TYPES_H
#define PPX_TYPES_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/**
 * Types
 */
typedef enum ppx_type {
    PPX_TYPE_INTEGER,
    PPX_TYPE_BOOLEAN,
    PPX_TYPE_CONS,
    PPX_TYPE_NULL,
    PPX_TYPE_SYMBOL,
    PPX_TYPE_EOF,
} ppx_type_t;

/**
 * Values.
 */
typedef struct ppx_value ppx_value_t;

ppx_type_t ppx_typeof(ppx_value_t *v) { return *((ppx_type_t *)v); }

/**
 * Integers.
 */
typedef struct ppx_integer {
    ppx_type_t type;
    int64_t value;
} ppx_integer_t;

ppx_value_t *ppx_integer(const int64_t value) {
    ppx_integer_t *i = (ppx_integer_t *)malloc(sizeof(*i));
    i->type = PPX_TYPE_INTEGER;
    i->value = value;
    return (ppx_value_t *)i;
}

bool ppx_is_integer(ppx_value_t *v) {
    return ppx_typeof(v) == PPX_TYPE_INTEGER;
}

/**
 * Booleans.
 */
typedef struct ppx_boolean {
    ppx_type_t type;
    bool value;
} ppx_boolean_t;

ppx_value_t *ppx_boolean(const bool value) {
    ppx_boolean_t *b = (ppx_boolean_t *)malloc(sizeof(*b));
    b->type = PPX_TYPE_BOOLEAN;
    b->value = value;
    return (ppx_value_t *)b;
}

bool ppx_is_boolean(ppx_value_t *v) {
    return ppx_typeof(v) == PPX_TYPE_BOOLEAN;
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
    ppx_cons_t *c = (ppx_cons_t *)malloc(sizeof(*c));
    c->type = PPX_TYPE_CONS;
    c->car = car;
    c->cdr = cdr;
    return (ppx_value_t *)c;
}

bool ppx_is_cons(ppx_value_t *v) { return ppx_typeof(v) == PPX_TYPE_CONS; }

void ppx_cons_set_car(ppx_value_t *cons, ppx_value_t *v) {
    ((ppx_cons_t *)cons)->car = v;
}

void ppx_cons_set_cdr(ppx_value_t *cons, ppx_value_t *v) {
    ((ppx_cons_t *)cons)->cdr = v;
}

/**
 * Null.
 */
typedef struct ppx_null {
    ppx_type_t type;
} ppx_null_t;

const ppx_null_t _PPX_NULL = {PPX_TYPE_NULL};

ppx_value_t *PPX_NULL = (ppx_value_t *)&_PPX_NULL;

bool ppx_is_null(ppx_value_t *v) { return ppx_typeof(v) == PPX_TYPE_NULL; }

/**
 * Symbols.
 */
typedef struct ppx_symbol {
    ppx_type_t type;
    char *name;
} ppx_symbol_t;

ppx_value_t *ppx_symbol(const char *name) {
    // TODO: If symbol already exists, return it. Otherwise create a new one (intern it).
    ppx_symbol_t *s = (ppx_symbol_t *) malloc(sizeof(*s));
    s->type = PPX_TYPE_SYMBOL;
    s->name = strdup(name);
    return (ppx_value_t *)s;
}

bool ppx_is_symbol(ppx_value_t *v) { return ppx_typeof(v) == PPX_TYPE_SYMBOL; }

/**
 * EOF.
 */
typedef struct ppx_eof {
    ppx_type_t type;
} ppx_eof_t;

const ppx_eof_t _PPX_EOF = {PPX_TYPE_EOF};

ppx_value_t *PPX_EOF = (ppx_value_t *)&_PPX_EOF;

bool ppx_is_eof(ppx_value_t *v) { return ppx_typeof(v) == PPX_TYPE_EOF; }

#endif
