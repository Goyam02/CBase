#include <stdio.h>
#include <string.h>

#include "cli.h"

#define INPUT_BUFFER_SIZE 1024

static void print_prompt(void){
    printf("cbase> ");
}

static void print_help(void){
    printf("\n");
    printf("CBase Commands\n");
    printf("--------------\n");
    printf(".help   Show this help message\n");
    printf(".tables List tables\n");
    printf(".exit   Exit CBase\n");
    printf("\n");
}

static void handle_create_table(Database *database, const char *input){
    char table_name[MAX_NAME_LENGTH];
    if (sscanf(
            input,
            "CREATE TABLE %31s",
            table_name
        ) != 1) {

        printf("Invalid CREATE TABLE command.\n");
        return;
    }

    Table *table = database_create_table(database, table_name);

    if (table == NULL) {
        printf("Could not create table.\n");
        return;
    }

    printf("Table '%s' created.\n", table->name);
}

void start_cli(Database *database){

    char input[INPUT_BUFFER_SIZE];

    printf("CBase v0.2\n");
    printf("A lightweight database engine written in C\n\n");

    while(1){

        print_prompt();

        if(fgets(input, sizeof(input), stdin) == NULL){
            break;
        }
        input[strcspn(input, "\n")] = '\0';

        if(strcmp(input, ".exit") == 0){
            printf("Goodbye!\n");
            break;
        }

        if(strcmp(input, ".help") == 0){
            print_help();
            continue;
        }

        if(strcmp(input, ".tables") == 0){
            database_list_tables(database);
            continue;
        }

        if(strncmp(input, "CREATE TABLE",12) == 0){
            handle_create_table(database, input);
            continue;
        }

        if (strlen(input) == 0){
            continue;
        }

        printf("Unknown command: %s\n", input);
    }
}