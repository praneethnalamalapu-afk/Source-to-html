#include "s2html_event.h"
#include <ctype.h>
#include <string.h>

static const char *const keywords[] = {
    "auto","break","case","char","const","continue","default","do","double",
    "else","enum","extern","float","for","goto","if","int","long","register",
    "return","short","signed","sizeof","static","struct","switch","typedef",
    "union","unsigned","void","volatile","while","_Bool","_Complex","_Imaginary",
    "inline","restrict"
};

static int keyword(const char *s)
{
    size_t i;
    for (i = 0; i < sizeof(keywords) / sizeof(keywords[0]); ++i)
        if (strcmp(s, keywords[i]) == 0) return 1;
    return 0;
}

static void put(S2Event *e, int c)
{
    size_t n = strlen(e->text);
    if (n + 1 < sizeof(e->text)) {
        e->text[n] = (char)c;
        e->text[n + 1] = '\0';
    }
    if (c == '\n') e->next_line++;
}

static int get(FILE *in, S2Event *e)
{
    int c = fgetc(in);
    if (c == '\n') e->next_line++;
    return c;
}

static void unget(FILE *in, S2Event *e, int c)
{
    if (c == EOF) return;
    if (c == '\n') e->next_line--;
    ungetc(c, in);
}

static int is_operator(int c)
{
    return strchr("+-*/%=!<>&|^~?:", c) != NULL;
}

static int is_special(int c)
{
    return strchr("(){}[];,.", c) != NULL;
}

static int is_pair(int a, int b)
{
    return (a == '+' && (b == '+' || b == '=')) ||
           (a == '-' && (b == '-' || b == '=' || b == '>')) ||
           ((a == '*' || a == '/' || a == '%' || a == '=' || a == '!' ||
             a == '<' || a == '>' || a == '&' || a == '|' || a == '^') && b == '=') ||
           ((a == '<' || a == '>') && b == a) ||
           ((a == '&' || a == '|') && b == a);
}

int s2_next_event(FILE *in, int first, int line, S2Event *e)
{
    int c, quote, escaped = 0, closed = 0;
    memset(e, 0, sizeof(*e));
    e->start_line = line;
    e->next_line = line;

    if (isspace((unsigned char)first)) {
        e->type = S2_TEXT;
        put(e, first);
        while ((c = get(in, e)) != EOF && isspace((unsigned char)c)) put(e, c);
        unget(in, e, c);
        return 0;
    }

    if (first == '#') {
        e->type = S2_PREPROCESSOR;
        put(e, first);
        while ((c = get(in, e)) != EOF && c != '\n') put(e, c);
        unget(in, e, c);
        return 0;
    }

    if (isalpha((unsigned char)first) || first == '_') {
        e->type = S2_IDENTIFIER;
        put(e, first);
        while ((c = get(in, e)) != EOF && (isalnum((unsigned char)c) || c == '_')) put(e, c);
        unget(in, e, c);
        if (keyword(e->text)) e->type = S2_KEYWORD;
        return 0;
    }

    if (isdigit((unsigned char)first)) {
        e->type = S2_NUMBER;
        put(e, first);
        while ((c = get(in, e)) != EOF &&
               (isalnum((unsigned char)c) || c == '.' || c == '_')) put(e, c);
        unget(in, e, c);
        return 0;
    }

    if (first == '"' || first == '\'') {
        quote = first;
        e->type = quote == '"' ? S2_STRING : S2_CHARACTER;
        put(e, first);
        while ((c = get(in, e)) != EOF) {
            put(e, c);
            if (escaped) escaped = 0;
            else if (c == '\\') escaped = 1;
            else if (c == quote) { closed = 1; break; }
            else if (c == '\n') break;
        }
        (void)closed; /* Preserve malformed literals as visible source text. */
        return 0;
    }

    if (first == '/') {
        c = get(in, e);
        if (c == '/') {
            e->type = S2_COMMENT;
            put(e, first); put(e, c);
            while ((c = get(in, e)) != EOF && c != '\n') put(e, c);
            unget(in, e, c);
            return 0;
        }
        if (c == '*') {
            int prev = 0;
            e->type = S2_COMMENT;
            put(e, first); put(e, c);
            while ((c = get(in, e)) != EOF) {
                put(e, c);
                if (prev == '*' && c == '/') break;
                prev = c;
            }
            return 0;
        }
        unget(in, e, c);
        e->type = S2_OPERATOR;
        put(e, first);
        return 0;
    }

    if (is_operator(first)) {
        e->type = S2_OPERATOR;
        put(e, first);
        c = get(in, e);
        if (is_pair(first, c)) {
            put(e, c);
            if ((first == '<' || first == '>') && c == first) {
                int third = get(in, e);
                if (third == '=') put(e, third);
                else unget(in, e, third);
            }
        } else {
            unget(in, e, c);
        }
        return 0;
    }

    if (is_special(first)) e->type = S2_SPECIAL;
    else e->type = S2_TEXT;
    put(e, first);
    return 0;
}

static int escaped_write(FILE *out, const char *s)
{
    for (; *s; ++s) {
        switch (*s) {
            case '&': if (fputs("&amp;", out) == EOF) return -1; break;
            case '<': if (fputs("&lt;", out) == EOF) return -1; break;
            case '>': if (fputs("&gt;", out) == EOF) return -1; break;
            case '"': if (fputs("&quot;", out) == EOF) return -1; break;
            default: if (fputc((unsigned char)*s, out) == EOF) return -1;
        }
    }
    return 0;
}

int s2_write_event(FILE *out, const S2Event *e)
{
    const char *class_name = NULL;
    switch (e->type) {
        case S2_KEYWORD: class_name = "keyword"; break;
        case S2_IDENTIFIER: class_name = "identifier"; break;
        case S2_NUMBER: class_name = "number"; break;
        case S2_STRING: class_name = "string"; break;
        case S2_CHARACTER: class_name = "character"; break;
        case S2_COMMENT: class_name = "comment"; break;
        case S2_PREPROCESSOR: class_name = "preprocessor"; break;
        case S2_OPERATOR: class_name = "operator"; break;
        case S2_SPECIAL: class_name = "special"; break;
        case S2_TEXT: default: break;
    }
    if (class_name && fprintf(out, "<span class=\"%s\">", class_name) < 0) return -1;
    if (escaped_write(out, e->text) != 0) return -1;
    if (class_name && fputs("</span>", out) == EOF) return -1;
    return 0;
}
