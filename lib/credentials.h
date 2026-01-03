#ifndef CREDENTIALS_H
#define CREDENTIALS_H

struct credentials {
    char *service;
    char *username;
    char *password;
    char *mail;

    char *connected_credentials;
};

void connect_credentials(struct credentials *credentials);

struct credentials* create_credentials(char *service, char *username, char *password, char *mail);

void overwrite_credentials_from_line(struct credentials *credentials, char *new_credentials);

struct credentials* create_credentials_from_line(char *buffer);

void overwrite_credentials(struct credentials *credentials, char *service, char *username, char *password, char *mail);

void free_credentials(struct credentials *credentials);

#endif
