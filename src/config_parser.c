#include "config_parser.h"

#include <string.h>

#define CREDENTIALS_PATH "CREDENTIALS_PATH"

void parse_line(struct config *config, char *key, char *value){
    if(strcmp(key, CREDENTIALS_PATH) == 0){
        config->path_to_credentials = strdup(value);
    }
}
