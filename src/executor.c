#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "executor.h"

static int create_condition_value(const char *text,Datatype type,Value *value){
    (*value).type = type;

    switch (type){

        case TYPE_INT: {
            char *end;

            long parsed = strtol(text,&end,10);
            if(*end != '\0'){
                return 0;
            }

            (*value).int_value = (int)parsed;

            return 1;
        }

        case TYPE_FLOAT: {
            char *end;

            float parsed = strtof(text,&end);
            if(*end != '\0'){
                return 0;
            }
           (*value).float_value = parsed;

            return 1;
        }

        case TYPE_TEXT:
            strncpy((*value).text_value,text,MAX_TEXT_LENGTH - 1);
            (*value).text_value[MAX_TEXT_LENGTH - 1] = '\0';

            return 1;

        default:
            return 0;
    }
}

static int find_column_index(const Table *table, const char *column_name){
    
    for(int i = 0; i < (*table).column_count; i++){
        if(strcmp((*table).columns[i].name,column_name) == 0){
            return i;
        }
    }
    return -1;
}

static int compare_values(const Value *left,Operator operator, const Value *right){
    if ((*left).type != (*right).type) {
        return 0;
    }

    switch ((*left).type) {

        case TYPE_INT: {
            int a = (*left).int_value;
            int b = (*right).int_value;

            switch (operator){
                case OP_EQUAL:
                    return a == b;

                case OP_NOT_EQUAL:
                    return a != b;

                case OP_LESS:
                    return a < b;

                case OP_LESS_EQUAL:
                    return a <= b;

                case OP_GREATER:
                    return a > b;

                case OP_GREATER_EQUAL:
                    return a >= b;
            }

            break;
        }

        case TYPE_FLOAT: {
            float a = (*left).float_value;
            float b = (*right).float_value;

            switch (operator) {
                case OP_EQUAL:
                    return a == b;

                case OP_NOT_EQUAL:
                    return a != b;

                case OP_LESS:
                    return a < b;

                case OP_LESS_EQUAL:
                    return a <= b;

                case OP_GREATER:
                    return a > b;

                case OP_GREATER_EQUAL:
                    return a >= b;
            }

            break;
        }

        case TYPE_TEXT: {
            int result = strcmp(
                (*left).text_value,
                (*right).text_value
            );

            switch (operator) {
                case OP_EQUAL:
                    return result == 0;

                case OP_NOT_EQUAL:
                    return result != 0;

                case OP_LESS:
                    return result < 0;

                case OP_LESS_EQUAL:
                    return result <= 0;

                case OP_GREATER:
                    return result > 0;

                case OP_GREATER_EQUAL:
                    return result >= 0;
            }

            break;
        }

        default:
            return 0;
    }

    return 0;
}


int execute_select(Database *database, const SelectQuery *query){
    Table *table = database_find_table(database, (*query).table_name);

    if(table == NULL){
        printf("Table '%s' doesnt exist", (*query).table_name);
        return 0;
    }
    int condition_column_index = -1;
    Value condition_value;

    if((*query).condition.has_condition){
        condition_column_index =find_column_index(table, (*query).condition.column);
        if (condition_column_index == -1){
            printf("Column '%s' does not exist.\n",(*query).condition.column);
            return 0;
        }
        Datatype type =(*table).columns[condition_column_index].type;

        if(!create_condition_value((*query).condition.value, type, &condition_value)){
            printf("Invalid WHERE value.\n");
            return 0;
        }
    }

    for(int i = 0; i < (*table).column_count;i++){
        printf("%s",(*table).columns[i].name);
        if (i < (*table).column_count - 1){
            printf(" | ");
        }
    }
    printf("\n");

    for(int i = 0;i < (*table).row_count;i++){
        Row *row = (*table).rows[i];

        if((*query).condition.has_condition){

            Value *actual = &(*row).values[condition_column_index];
            if(!compare_values(actual, (*query).condition.operator, &condition_value)){
                continue;
            }
        }
        row_print((*table).rows[i]);
    }
    return 1;
}

int execute_update(Database *database, const UpdateQuery *query){
    Table *table = database_find_table(database, (*query).table_name);

    if(table == NULL){
        printf("Table '%s' does not exist.\n", (*query).table_name);
        return 0;
    }

    int update_column_index = find_column_index(table, (*query).update_column);
    if(update_column_index == -1){
        printf("Column '%s' does not exist.\n", (*query).update_column);
        return 0;
    }

    Datatype update_type = (*table).columns[update_column_index].type;
    Value new_value;

    if(!create_condition_value((*query).update_value, update_type, &new_value)){
        printf("Invalid UPDATE value.\n");
        return 0;
    }

    int condition_column_index = -1;
    Value condition_value;

    if((*query).condition.has_condition){
        condition_column_index = find_column_index(table, (*query).condition.column);
        if(condition_column_index == -1){
            printf("Column '%s' does not exist.\n", (*query).condition.column);
            return 0;
        }

        Datatype condition_type = (*table).columns[condition_column_index].type;

        if(!create_condition_value((*query).condition.value, condition_type, &condition_value)){
            printf("Invalid WHERE value.\n");
            return 0;
        }
    }

    int updated_count = 0;

    for(int i = 0; i < (*table).row_count; i++){
        Row *row = (*table).rows[i];

        if((*query).condition.has_condition){
            Value *actual = &(*row).values[condition_column_index];
            if(!compare_values(actual, (*query).condition.operator, &condition_value)){
                continue;
            }
        }

        (*row).values[update_column_index] = new_value;
        updated_count++;
    }

    printf("Updated %d row(s).\n", updated_count);
    return 1;
}
