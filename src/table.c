#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "table.h"

void table_init(Table *table, const char *name){
    strncpy((*table).name, name, MAX_NAME_LENGTH - 1);

    (*table).name[MAX_NAME_LENGTH - 1] = '\0';
    (*table).column_count = 0;

    (*table).rows = NULL;
    (*table).row_count = 0;
    (*table).row_capacity = 0;


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


int table_insert_row(Table *table, Row *row){
    if(row == NULL) return 0;

    if((*row).value_count != (*table).column_count) return 0;

    if((*table).row_count >= (*table).row_capacity){
        int new_capacity;

        if((*table).row_capacity == 0){
            new_capacity = 8;
        }else{
            new_capacity = (*table).row_capacity * 2;

        }

        Row **new_rows = realloc((*table).rows, sizeof(Row *)*new_capacity);

        if(new_rows == NULL) return 0;
        (*table).rows = new_rows;
        (*table).row_capacity = new_capacity;

    }
    (*table).rows[(*table).row_count] = row;
    (*table).row_count++;

    return 1;
}


void table_free(Table *table) {

    for(int i = 0; i < (*table).row_count; i++){
        row_free((*table).rows[i]);
    }

    free((*table).rows);

    (*table).rows = NULL;
    (*table).row_count = 0;
    (*table).row_capacity = 0;
}