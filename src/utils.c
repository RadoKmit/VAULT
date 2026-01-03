#include "utils.h"

#include <string.h>
#include <ctype.h>

char *trim(char *s) {
    char *end;
    // trim left
    while (*s == '\t' || *s == ' ') s++;
    if (*s == 0)
        return s;

    // trim right
    end = s + strlen(s) - 1;
    while (end > s && (*end == ' ' || *end == '\t' || *end == '\r'))
        end--;

    *(end + 1) = '\0';

    return s;
}

int contains_illegal_chars(const char *s) {
    if (s == NULL)
        return 1;

    for (; *s; s++) {
        unsigned char c = (unsigned char)*s;

        if (c < 32 || c == 127) {
            return 1;
        }

        if (c == '\n' || c == '\r' || c == '\t') {
            return 1;
        }
    }
    return 0;
}
