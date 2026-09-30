#include<stdio.h>
#include<string.h>

#include "table.h"

void table_init(Table *table, const char *name){
    strncpy((*table).name, name, MAX_NAME_LENGTH - 1);
    (*table).name[MAX_NAME_LENGTH - 1] = '\0';
    (*table).column_count = 0;
}

int table_add_column(Table *table, const char* name, Datatype type){


    if((*table).column_count >= MAX_COLUMNS) return 0;

    Column *column = &(*table).columns[(*table).column_count];
    strncpy((*column).name, name, MAX_NAME_LENGTH -1); // where to, from where, how many
    (*column).name[MAX_NAME_LENGTH - 1] = '\0';
    (*column).type = type;

    (*table).column_count++;

    return 1;

}

static const char *data_type_to_string(Datatype type){

    switch(type){
        case TYPE_INT:
            return "INT";
        case TYPE_FLOAT:
            return "FLOAT";
        case TYPE_TEXT:
            return "TEXT";
        default:
            return "UNKNOWN";
    }
}

void table_print_schema(const Table *table){

    printf("CREATE TABLE %s (\n", (*table).name);
    for(int i = 0; i < (*table).column_count; i++){
        printf("    %s  %s", 
        (*table).columns[i].name,
        data_type_to_string((*table).columns[i].type)
    );
    if(i < (*table).column_count - 1){
        printf(",");
    }
    printf("\n");

    }
    printf(");\n");
}

