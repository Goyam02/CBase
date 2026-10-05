#include <stdio.h>
#include<string.h>

#include "database.h"

void database_init(Database *database){
    (*database).table_count = 0;
}

Table *database_create_table(Database * database, const char *name){
    if((*database).table_count >= MAX_TABLES) return NULL;

    if((*database).table_count >0 && (*database).tables[(*database).table_count -1].name[0] == '\0') return NULL; //

    Table *table = &(*database).tables[(*database).table_count];

    table_init(table, name);
    (*database).table_count++;

    return table;



}

Table *database_find_table(Database *database, const char* name){
    for(int i = 0; i < (*database).table_count; i++){
        if(strcmp((*database).tables[i].name, name) == 0) return &(*database).tables[i];
    }

    return NULL;
}

void database_list_tables(const Database *database){
    if((*database).table_count == 0){
        printf("No tables.\n");
        return ;
    }

    for(int i = 0; i < (*database).table_count; i++){
        printf("%s\n", (*database).tables[i].name);
    }

}

int database_add_table(Database *database, const Table *table){
    if((*database).table_count >= MAX_TABLES) return 0;

    (*database).tables[(*database).table_count] = *table;

    (*database).table_count++;

    return 1;



}

void database_free(Database *database){

    for(int i = 0; i < (*database).table_count; i++){
        table_free(&(*database).tables[i]);
    }

    (*database).table_count = 0;
}
