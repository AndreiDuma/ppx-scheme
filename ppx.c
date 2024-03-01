#include <stdio.h>
#include <stdlib.h>

// Types.
typedef enum ppx_type {
    PPX_TYPE_SYMBOL,
    PPX_TYPE_INTEGER,
    PPX_TYPE_CONS,
    PPX_TYPE_NIL,
} ppx_type_t;

// Values.
typedef struct ppx_value ppx_value_t;

ppx_value_t *ppx_read(void);

void ppx_print(ppx_value_t *v);

ppx_type_t ppx_typeof(ppx_value_t *v) { return *((ppx_type_t *) v); }

// Symbols.
typedef struct ppx_symbol {
    ppx_type_t type;
    char *name;
} ppx_symbol_t;

ppx_value_t *ppx_symbol(char *name) {
    ppx_symbol_t *s = malloc(sizeof(*s));
    s->type = PPX_TYPE_SYMBOL;
    s->name = name;
    return (ppx_value_t *) s;
}

void ppx_print_symbol(ppx_symbol_t *s) { printf("%s", s->name); }

// Integers.
typedef struct ppx_integer {
    ppx_type_t type;
    int64_t value;
} ppx_integer_t;

ppx_value_t *ppx_integer(int64_t value) {
    ppx_integer_t *i = malloc(sizeof(*i));
    i->type = PPX_TYPE_INTEGER;
    i->value = value;
    return (ppx_value_t *) i;
}

ppx_value_t *ppx_read_integer(void) {
    int64_t value;
    scanf("%lld", &value);
    return ppx_integer(value);
}

void ppx_print_integer(ppx_integer_t *i) { printf("%lld", i->value); }

// Conses (aka pairs).
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
    return (ppx_value_t *) c;
}

ppx_value_t *ppx_read_cons(void) {
    scanf("(");
    ppx_value_t *car = ppx_read();
    scanf(" . ");
    ppx_value_t *cdr = ppx_read();
    scanf(")");
    return ppx_cons(car, cdr);
}

void ppx_print_cons(ppx_cons_t *c) {
    printf("(");
    ppx_print(c->car);
    printf(" . ");
    ppx_print(c->cdr);
    printf(")");
}

// NIL.
typedef struct ppx_nil {
    ppx_type_t type;
} ppx_nil_t;

const ppx_value_t *ppx_nil = (ppx_value_t *) &(ppx_nil_t){PPX_TYPE_NIL};

const ppx_value_t *ppx_read_nil(void) {
    scanf("nil");
    return ppx_nil;
}

void ppx_print_nil(void) { printf("nil"); }

// Generic functions.
int fpeek(FILE *stream) {
    int c;

    c = fgetc(stream);
    ungetc(c, stream);

    return c;
}

ppx_value_t *ppx_read(void) {
    switch (fpeek(stdin)) {
        case '(':
            return ppx_read_cons();
        default:
            return ppx_read_integer();
    }
}

void ppx_print(ppx_value_t *v) {
    switch (ppx_typeof(v)) {
        case PPX_TYPE_INTEGER:
            ppx_print_integer((ppx_integer_t *) v);
            break;
        case PPX_TYPE_CONS:
            ppx_print_cons((ppx_cons_t *) v);
            break;
        case PPX_TYPE_NIL:
            ppx_print_nil();
            break;
    }
}

void ppx_println(ppx_value_t *v) {
    ppx_print(v);
    printf("\n");
}

int main() {
    ppx_value_t *i = ppx_integer(3);
    ppx_println(i);

    ppx_value_t *c = ppx_cons(i, ppx_integer(4));
    ppx_println(c);

    ppx_value_t *c2 = ppx_cons(ppx_integer(2), c);
    ppx_println(c2);

    printf("some reading...\n");

    ppx_value_t *x = ppx_read();
    ppx_println(x);

    return 0;
}
