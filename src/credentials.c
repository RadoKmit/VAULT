#include <stdlib.h>
#include <string.h>

#include "credentials.h"
#include "utils.h"

void connect_credentials(struct credentials *credentials){
    memset(credentials->connected_credentials, ' ', TOTAL_SIZE);

    size_t len;

    len = strnlen(credentials->service, FIELD_SIZE-1);
    memcpy(credentials->connected_credentials + FIELD_SIZE * 0, credentials->service, len);

    len = strnlen(credentials->username, FIELD_SIZE-1);
    memcpy(credentials->connected_credentials + FIELD_SIZE * 1, credentials->username, len);

    len = strnlen(credentials->password, FIELD_SIZE-1);
    memcpy(credentials->connected_credentials + FIELD_SIZE * 2, credentials->password, len);

    len = strnlen(credentials->mail, FIELD_SIZE-1);
    memcpy(credentials->connected_credentials + FIELD_SIZE * 3, credentials->mail, len);

    for(int i = FIELD_SIZE-1; i < TOTAL_SIZE-1; i += FIELD_SIZE){
        credentials->connected_credentials[i] = ':';
    }

    credentials->connected_credentials[TOTAL_SIZE-1] = '\n';
}

struct credentials* create_credentials(char *service, char *username, char *password, char *mail){
    struct credentials *credentials = malloc(sizeof *credentials);

    credentials->service = calloc(FIELD_SIZE, sizeof(char));
    memcpy(credentials->service, service, strlen(service));

    credentials->username = calloc(FIELD_SIZE, sizeof(char));
    memcpy(credentials->username, username, strlen(username));

    credentials->password = calloc(FIELD_SIZE, sizeof(char));
    memcpy(credentials->password, password, strlen(password));

    credentials->mail = calloc(FIELD_SIZE, sizeof(char));
    memcpy(credentials->mail, mail, strlen(mail));

    credentials->connected_credentials = calloc(TOTAL_SIZE, sizeof(char));
    connect_credentials(credentials);

    return credentials;
}

void overwrite_credentials_from_line(struct credentials *credentials, char *buffer){
    for(int i = FIELD_SIZE-1; i < TOTAL_SIZE; i+=FIELD_SIZE){
        buffer[i] = '\0';
    }

    memcpy(credentials->service, buffer+FIELD_SIZE*0, FIELD_SIZE);
    credentials->service[FIELD_SIZE-1] = '\0';
    trim(credentials->service);

    memcpy(credentials->username, buffer+FIELD_SIZE*1, FIELD_SIZE);
    credentials->username[FIELD_SIZE-1] = '\0';
    trim(credentials->username);

    memcpy(credentials->password, buffer+FIELD_SIZE*2, FIELD_SIZE);
    credentials->password[FIELD_SIZE-1] = '\0';
    trim(credentials->password);

    memcpy(credentials->mail, buffer+FIELD_SIZE*3, FIELD_SIZE);
    credentials->mail[FIELD_SIZE-1] = '\0';
    trim(credentials->mail);

    connect_credentials(credentials);
}

struct credentials* create_credentials_from_line(char *buffer){
    struct credentials *credentials = malloc(sizeof *credentials);

    credentials->service = calloc(FIELD_SIZE, sizeof(char));
    credentials->username = calloc(FIELD_SIZE, sizeof(char));
    credentials->password = calloc(FIELD_SIZE, sizeof(char));
    credentials->mail = calloc(FIELD_SIZE, sizeof(char));

    credentials->connected_credentials = calloc(TOTAL_SIZE, sizeof(char));
    overwrite_credentials_from_line(credentials, buffer);

    return credentials;
}

void overwrite_credentials(struct credentials *credentials, char *service, char *username, char *password, char *mail){
    memcpy(credentials->service, service, FIELD_SIZE);
    memcpy(credentials->username, username, FIELD_SIZE);
    memcpy(credentials->password, password, FIELD_SIZE);
    memcpy(credentials->mail, mail, FIELD_SIZE);

    connect_credentials(credentials);
}

void free_credentials(struct credentials *credentials){
    if(!credentials){
        return;
    }

    free(credentials->service);
    free(credentials->username);
    free(credentials->password);
    free(credentials->mail);
    free(credentials->connected_credentials);
    free(credentials);

    credentials = NULL;
}
