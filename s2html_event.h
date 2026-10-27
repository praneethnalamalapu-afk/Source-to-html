#ifndef S2HTML_EVENT_H
#define S2HTML_EVENT_H

#include <stdio.h>
#include <stddef.h>

#define S2_EVENT_CAPACITY 8192

typedef enum {
    S2_TEXT,
    S2_KEYWORD,
    S2_IDENTIFIER,
    S2_NUMBER,
    S2_STRING,
    S2_CHARACTER,
    S2_COMMENT,
    S2_PREPROCESSOR,
    S2_OPERATOR,
    S2_SPECIAL
} S2TokenType;

typedef struct {
    S2TokenType type;
    char text[S2_EVENT_CAPACITY];
    int start_line;
    int next_line;
} S2Event;

/* first_char has already been read from input. */
int s2_next_event(FILE *input, int first_char, int line, S2Event *event);
int s2_write_event(FILE *output, const S2Event *event);

#endif
