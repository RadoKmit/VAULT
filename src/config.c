#include "config.h"
#include "config_parser.h"
#include "utils.h"

#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <unistd.h>
#include <fcntl.h>

#define CONFIG_FILE "config.ini"

void free_config(struct config *config){
    if(!config){
        return;
    }

    free(config->path_to_credentials);
    free(config);
}

void read_config(struct config *config){
    int conf_fd = open(CONFIG_FILE, O_RDONLY);
    if(conf_fd < 0){
        perror("ERROR: opening config file!");
        return;
    }

    char buf;
    char line[256];
    int pos = 0;

    while (read(conf_fd, &buf, 1) > 0) {
        if(buf == '\n'){
            line[pos] = '\0';
            pos = 0;

            char *eq = strchr(line, '=');
            if (eq) {
                *eq = '\0';
                char *key = line;
                char *value = eq + 1;
                key = trim(key);
                value = trim(value);
                parse_line(config, key, value);
            }
        } else {
            line[pos++] = buf;
        }
    }

    if (pos > 0) {
        line[pos] = '\0';

        char *eq = strchr(line, '=');
        if (eq) {
            *eq = '\0';
            char *key = trim(line);
            char *value = trim(eq + 1);
            parse_line(config, key, value);
        }
    }

    close(conf_fd);
}

struct config *config_create(){
    struct config *config = malloc(sizeof *config);

    config->path_to_credentials = NULL;

    read_config(config);

    return config;
}
