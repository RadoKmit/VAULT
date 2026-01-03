#include <complex.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "utils.h"
#include "password_manager.h"

/*
 * HELPER FUNCTIONS
 */

int lines_count(struct config *config){
    struct stat st;
    if(stat(config->path_to_credentials, &st) != 0){
        perror("ERROR: stat");
        return -1;
    }
    if(st.st_size == 0){
        return 0;
    }
    int lines = st.st_size / TOTAL_SIZE;
    return lines;
}

void read_line(char *file_path, char *buffer, int line){
    int pswd_fd = open(file_path, O_RDONLY);
    if(pswd_fd < 0){
        perror("ERROR: open password file!");
        return;
    }

    lseek(pswd_fd, line * TOTAL_SIZE, SEEK_SET);

    if(read(pswd_fd, buffer, TOTAL_SIZE) == -1){
        perror("ERROR: reading from password file!");
        close(pswd_fd);
        return;
    }

    close(pswd_fd);
}

int find_password_by_service(struct config *config, char *service){
    int lines = lines_count(config);
    char buffer[TOTAL_SIZE];
    char compare_buffer[FIELD_SIZE];
    memset(compare_buffer+(FIELD_SIZE-1), '\0', 1);

    for(int i = 0; i < lines; i++){
        read_line(config->path_to_credentials, buffer, i);
        memcpy(compare_buffer, buffer, FIELD_SIZE-1);
        trim(compare_buffer);
        if(strcmp(compare_buffer, service) == 0){
            return i;
        }
    }

    return -1;
}

/*
 * Password managing functions
 */

int get_empty_line(struct config *config){
    char buffer[TOTAL_SIZE];
    int lines = lines_count(config);
    if(lines == 0){
        return -1;
    }

    char empty[TOTAL_SIZE];
    memset(empty, ' ', TOTAL_SIZE);
    empty[TOTAL_SIZE-1] = '\n';

    for(int l = 0; l < lines; l++){
        read_line(config->path_to_credentials, buffer, l);
        if(memcmp(empty, buffer, TOTAL_SIZE) == 0){
            return l;
        }
    }

    return -1;
}

void overwrite_password_file(char *file_path, char *buffer, int line){
    int pswd_fd = open(file_path, O_WRONLY|O_BINARY);
    if(pswd_fd < 0){
        perror("ERROR: open password file!");
        return;
    }

    lseek(pswd_fd, line * TOTAL_SIZE, SEEK_SET);

    if(write(pswd_fd, buffer, TOTAL_SIZE) < TOTAL_SIZE){
        perror("ERROR: writing to password file!");
        close(pswd_fd);
        return;
    }

    close(pswd_fd);
}

void append_password_file(char *file_path, char *buffer){
    int pswd_fd = open(file_path, O_WRONLY|O_APPEND|O_CREAT|O_BINARY, 0666);
    if(pswd_fd < 0){
        perror("ERROR: open password file!");
        return;
    }
    if(write(pswd_fd, buffer, TOTAL_SIZE) < TOTAL_SIZE){
        printf("ERROR: appending to password file!");
        close(pswd_fd);
        return;
    }
    close(pswd_fd);
}


int check_if_exists(struct config *config){
    struct stat st;
    if (stat(config->path_to_credentials, &st) != 0) {
        return -1;
    }

    if (st.st_size == 0) {
        return 0;
    }

    if (st.st_size % TOTAL_SIZE != 0) {
        printf("Corrupted credentials file!\n");
        return -2;
    }

    return 1;
}


/*
 * Public functions
 */

void add_password(struct config *config, char *service, char *username, char *password, char *mail){
    struct credentials *credentials = create_credentials(service, username, password, mail);
    int state = check_if_exists(config);
    if(state <= 0){
        append_password_file(config->path_to_credentials, credentials->connected_credentials);
    }else {
        int empty_line = get_empty_line(config);
        if (empty_line != -1) {
            overwrite_password_file(config->path_to_credentials, credentials->connected_credentials, empty_line);
        } else {
            append_password_file(config->path_to_credentials, credentials->connected_credentials);
        }
    }

    free_credentials(credentials);
}


void remove_pasword(struct config *config, char *service){
    int line = find_password_by_service(config, service);
    if(line == -1){
        printf("Password not found!\n");
        return;
    }

    char buffer[TOTAL_SIZE];
    memset(buffer, ' ', TOTAL_SIZE);
    buffer[TOTAL_SIZE-1] = '\n';

    overwrite_password_file(config->path_to_credentials, buffer, line);
}

char *get_password_to_edit(struct config *config, char *service){
    int line = find_password_by_service(config, service);
    if(line == -1){
        printf("Password not found!\n");
        return NULL;
    }

    char *buffer = calloc(128, sizeof(char));
    read_line(config->path_to_credentials, buffer, line);
    return buffer;
}

void edit_password(struct config *config, char *buffer, int line){
    overwrite_password_file(config->path_to_credentials, buffer, line);
}
