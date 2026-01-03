#ifndef PASSWORD_MANAGER_H
#define PASSWORD_MANAGER_H

#include "config.h"

void read_line(char *file_path, char *buffer, int line);

int find_password_by_service(struct config *config, char *service);

void add_password(struct config *config, char *service, char *username, char *password, char *mail);

void remove_pasword(struct config *config, char *service);

char *get_password_to_edit(struct config *config, char *service);

void edit_password(struct config *config, char *buffer, int line);

#endif
