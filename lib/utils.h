#ifndef UTILS_H
#define UTILS_H

#define FIELD_SIZE 32
#define TOTAL_SIZE (FIELD_SIZE * 4)

char *trim(char *s);

int contains_illegal_chars(const char *s);

#endif
