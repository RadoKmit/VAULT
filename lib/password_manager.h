#ifndef PASSWORD_MANAGER_H
#define PASSWORD_MANAGER_H

void read_line(char *file_path, char *buffer, int line);

int find_password_by_service(char *file_path, char *service);

void add_password(char *file_path, char *service, char *username, char *password, char *mail);

void remove_pasword(char *file_path, char *service);

char *get_password_to_edit(char *file_path, char *service);

void edit_password(char *file_path, char *buffer, int line);

#endif
