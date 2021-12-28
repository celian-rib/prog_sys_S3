#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "StringVector.h"

void string_vector_init(struct StringVector *this, size_t capacity) {
    this->capacity = capacity;
    this->size = 0;
    this->strings = malloc(capacity * sizeof(char *));
}

void string_vector_free(struct StringVector *this) {
    for (int i = 0; i < (int)(this->size); i++) {
        free(this->strings[i]);
    }
    free(this->strings);
}

void string_vector_add(struct StringVector *this, const char *begin, const char *end) {
    if (this->capacity == this->size) {
        this->capacity *= 2;
        this->strings = realloc(this->strings, this->capacity * sizeof(char *));
    }
    if (NULL == begin)
        this->strings[this->size++] = NULL;
    else
        this->strings[this->size++] = strndup(begin, end - begin);
}

size_t string_vector_size(const struct StringVector *this) {
    return this->size;
}

char *string_vector_get(const struct StringVector *this, size_t index) {
    return this->strings[index];
}

struct StringVector split_line(char *line) {
    struct StringVector tokens;
    string_vector_init(&tokens, 8);

    char *start = NULL; // where the current token starts. NULL if no token
    for (char *p = line; *p != '\0'; p++) {
        if ((start == NULL) && !isspace(*p)) {
            // starting a new token
            start = p;
        } else if ((start != NULL) && isspace(*p)) {
            string_vector_add(&tokens, start, p);
            start = NULL;
        };
    }
    return tokens;
}

char *strjoinarray(char *dest, const struct StringVector *this, size_t fist, size_t last, char *glue) {
    size_t glue_length = strlen(glue);
    char *target = dest; // where to copy the next elements
    *target = '\0';
    for (size_t i = fist; i < last; i++) {
        if (i > 0) { // need glue ?
            strcat(target, glue);
            target += glue_length;
        }
        strcat(target, this->strings[i]);
        target += strlen(this->strings[i]); // move to the end
    };
    return dest;
}
