#include "utils.h"
#include "config.h"
#include "credentials.h"
#include "password_manager.h"

#include <inttypes.h>
#include <pthread_time.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

void print_add_menu(){
    printf("#---------#ADD-------MENU#---------#\n");
    printf("# max lenght of all fields is 30   #\n");
    printf("# spaces will be deleted           #\n");
    printf("# -------------------------------- #\n");
    printf("# 'service' - service for which    #\n");
    printf("# you are saving password for (YT) #\n");
    printf("# -------------------------------- #\n");
    printf("# 'username' - your username for   #\n");
    printf("# that service                     #\n");
    printf("# -------------------------------- #\n");
    printf("# 'password' - password for that   #\n");
    printf("# service                          #\n");
    printf("# -------------------------------- #\n");
    printf("# 'mail' - mail for that service   #\n");
    printf("# (optional)                       #\n");
    printf("#---------#ADD-------MENU#---------#\n");
}

void loop_while_illegal_chars(char *buffer, char *field_name){
    int contains_illegal_char = 1;
    while(contains_illegal_char){
        printf("# %s ->", field_name);
        if(!fgets(buffer, FIELD_SIZE, stdin)){
            buffer[0] = '\0';
            printf("Problem occured while reading the input, try again\n");
            continue;
        }
        buffer[strcspn(buffer, "\n")] = '\0';
        if(buffer[0] == '\0'){
            printf("Can't be empty, try again.\n");
            continue;
        }
        contains_illegal_char = contains_illegal_chars(buffer);
    }
    trim(buffer);
}

void add_menu(struct config *config){
    print_add_menu();
    char service[FIELD_SIZE];
    char username[FIELD_SIZE];
    char password[FIELD_SIZE];
    char mail[FIELD_SIZE];

    loop_while_illegal_chars(service, "SERVICE");
    loop_while_illegal_chars(username, "USERNAME");
    loop_while_illegal_chars(password, "PASSWORD");
    loop_while_illegal_chars(mail, "MAIL");

    add_password(config, service, username, password, mail);
}

void print_del_menu(){
    printf("# Write service for which password \n");
    printf("# should be deleted \n");
    printf("# ->");
}

void del_menu(struct config *config){
    print_del_menu();
    char service[FIELD_SIZE];
    fgets(service, FIELD_SIZE, stdin);
    service[strcspn(service, "\n")] = '\0';
    if(service[0] == '\0'){
        printf("Can't be empty.\n");
        return;
    }
    remove_pasword(config, service);
    printf("# Password for %s was deleted :]\n", service);
}

void edit_loop(char *field_name){
    char buffer[FIELD_SIZE];
    int contains_illegal_char = 1;
    while(contains_illegal_char){
        printf("# Keep empty to keep old save\n");
        printf("# EDITING: %s ->", field_name);
        if(!fgets(buffer, FIELD_SIZE, stdin)){
            buffer[0] = '\0';
            printf("Problem occured while reading the input, try again\n");
            continue;
        }
        buffer[strcspn(buffer, "\n")] = '\0';
        if(buffer[0] == '\0'){
            return;
        }
        contains_illegal_char = contains_illegal_chars(buffer);
    }
    trim(buffer);
    printf("# New save ->%s\n", buffer);
    memcpy(field_name, buffer, strlen(buffer));
}

void print_edit_menu(struct credentials *credentials){
    edit_loop(credentials->service);
    edit_loop(credentials->username);
    edit_loop(credentials->password);
    edit_loop(credentials->mail);
}

void edit_menu(struct config *config){
    printf("# service to edit ->");
    char service[FIELD_SIZE];
    fgets(service, FIELD_SIZE, stdin);
    service[strcspn(service, "\n")] = '\0';
    if(service[0] == '\0'){
        printf("Can't be empty.\n");
        return;
    }
    char *buffer = get_password_to_edit(config, service);
    if(buffer == NULL){
        return;
    }

    struct credentials *credentials = create_credentials_from_line(buffer);
    print_edit_menu(credentials);
    connect_credentials(credentials);
    int line = find_password_by_service(config, service);
    edit_password(config, credentials->connected_credentials, line);
    free_credentials(credentials);
}

void print_menu(){
    printf("#---------#VAULT-----MENU#---------#\n");
    printf("# add password (ADD)               #\n");
    printf("# del password (DEL)               #\n");
    printf("# edit password (EDIT)             #\n");
    printf("# exit app (EXIT)                  #\n");
    printf("#---------#VAULT-----MENU#---------#\n");
    printf("# ->");
}

void menu_loop(struct config *config){
    unsigned int exit = 0;
    while(!exit){
        print_menu();
        char choice[FIELD_SIZE];
        fgets(choice, FIELD_SIZE, stdin);
        choice[strcspn(choice, "\n")] = '\0';

        if(strcmp(choice, "ADD") == 0){
            add_menu(config);
        }
        else if(strcmp(choice, "DEL") == 0){
            del_menu(config);
        }
        else if(strcmp(choice, "EDIT") == 0){
            edit_menu(config);
        }
        else if(strcmp(choice, "EXIT") == 0){
            exit = 1;
            printf("Exiting VAULT\n");
        }
    }
}

int main(){
    struct config *config = config_create();

    menu_loop(config);

    free_config(config);
    return 0;
}
