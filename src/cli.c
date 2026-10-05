#include <stdio.h>
#include <string.h>

#include "cli.h"
#include "parser.h"
#include "executor.h"

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
    printf(".schema <table> Show table schema\n");
    printf(".exit   Exit CBase\n");
    printf("\n");
}

static void handle_create_table(Database *database, const char *input){
    Table table;
    if(!parse_create_table(input, &table)){
        printf("Syntax error in CREATE TABLE.\n");
        return;
    }
    if(database_find_table(database, table.name) != NULL){
        printf("Table '%s' already exists.\n",table.name);
        return;
    }

    if(!database_add_table(database, &table)){
        printf("Could not create table.\n");
        return;
    }
    printf("Table '%s' created.\n", table.name);
}
// static void handle_create_table(Database *database, const char *input){
//     char table_name[MAX_NAME_LENGTH];
//     if (sscanf(
//             input,
//             "CREATE TABLE %31s",
//             table_name
//         ) != 1) {

//         printf("Invalid CREATE TABLE command.\n");
//         return;
//     }

//     Table *table = database_create_table(database, table_name);

//     if (table == NULL) {
//         printf("Could not create table.\n");
//         return;
//     }

//     printf("Table '%s' created.\n", table->name);
// }

static void handle_insert(Database *database, const char *input){
    char table_name[MAX_NAME_LENGTH];
    if (sscanf(input, "INSERT INTO %31s", table_name) != 1){
        printf("Invalid INSERT command.\n");
        return;
    }

    Table *table = database_find_table(database,table_name);

    if(table == NULL){
        printf("Table '%s' does not exist.\n",table_name);
        return;
    }

    Row *row = NULL;

    if(!parse_insert(input, table, &row)){
        printf("Syntax error in INSERT.\n");
        return;
    }

    if(!table_insert_row(table, row)){
        row_free(row);
        printf("Failed to insert row.\n");
        return;
    }

    printf("Inserted.\n");
}

static void handle_select(Database *database, const char *input){
    // const char *from = strstr(input, "FROM");
    SelectQuery query;
    if(!parse_select(input, &query)){
        printf("Invalid SELECT command.\n");
        return;
    }

    execute_select(database,&query);

    // if(from == NULL){
    //     printf("Invalid SELECT command.\n");
    //     return;
    // }

    // from += 4;

    // while(*from == ' '){
    //     from++;
    // }

    // char table_name[MAX_NAME_LENGTH];

    // if(sscanf(from, "%31s", table_name) != 1){
    //     printf("Invalid SELECT command.\n");
    //     return;
    // }

    // Table *table = database_find_table(database,table_name);
    // if (table == NULL) {
    //     printf("Table '%s' does not exist.\n",table_name);
    //     return;
    // }

    // for(int i = 0; i < (*table).column_count; i++){
    //     printf("%s",(*table).columns[i].name);
    //     if(i < (*table).column_count - 1){
    //         printf(" | ");
    //     }
    // }

    // printf("\n");

    // for(int i = 0; i < (*table).row_count; i++){
    //     row_print((*table).rows[i]);
    // }
}




void start_cli(Database *database){

    char input[INPUT_BUFFER_SIZE];

    printf("CBase\n");
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
        
        if(strncmp(input, ".schema", 7) == 0){

            const char *table_name = input +7;
             while(*table_name == ' '){
                table_name++;
            }
            if(*table_name == '\0'){
                printf("Usage: .schema <table>\n");
                continue;
            }

            Table *table = database_find_table(database, table_name);
            if(table == NULL){
                printf("Table '%s' does not exists yet\n", table_name);
            }else{
                table_print_schema(table);
            }
            continue;

        }
        if(strncmp(input, "INSERT INTO", 11) == 0){
            handle_insert(database, input);
            continue;
        }

        if(strncmp(input, "SELECT", 6) == 0){
            handle_select(database, input);
            continue;
        }

        printf("Unknown command: %s\n", input);
    }
}


