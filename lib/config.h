#ifndef CONFIG_H
#define CONFIG_H

struct config {
    char *path_to_credentials;
};

void free_config(struct config *config);

struct config *config_create();

void read_config(struct config *config);

#endif
