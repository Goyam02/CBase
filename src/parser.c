#include <stdio.h>
#include<string.h>
#include<ctype.h>
#include <stdlib.h>

#include "parser.h"

static void skip_spaces(const char **input){
    while(**input != '\0' && isspace((unsigned char)**input)){
        (*input)++;
    }
}

static int read_word(const char **input, char *buffer, int buffer_size){
    skip_spaces(input);
    int length = 0;
    while(**input != '\0' && !isspace((unsigned char) **input) && **input != '(' && **input != ')' && **input != ','){
        if(length >= buffer_size - 1) return 0;
        buffer[length++] = **input;
        (*input)++;
    }
    buffer[length] = '\0';
    return length > 0;
}

static int match_keyword(const char **input, const char *keyword){
    char word[MAX_NAME_LENGTH];

    if(!read_word(input, word, sizeof(word))) return 0;
    return strcmp(word, keyword) == 0;
}

static int parse_data_type(const char **input, Datatype *type){
    char word[MAX_NAME_LENGTH];

    if(!read_word(input, word, sizeof(word))) return 0;

    if(strcmp(word, "INT") == 0){
        *type = TYPE_INT;
        return 1;
    }
    if(strcmp(word, "FLOAT") == 0){
        *type = TYPE_FLOAT;
        return 1;
    }
    if(strcmp(word, "TEXT") == 0){
        *type = TYPE_TEXT;
        return 1;
    }

    return 0;
}

int parse_create_table(const char *input, Table *table){


    if(!match_keyword(&input, "CREATE")) return 0;
    if(!match_keyword(&input, "TABLE")) return 0;

    char table_name[MAX_NAME_LENGTH];
    if(!read_word(&input, table_name, sizeof(table_name))) return 0;

    skip_spaces(&input);

    if(*input != '(') return 0;

    input++;

    table_init(table, table_name);

    while(1){
        skip_spaces(&input);
        if(*input == ')'){
            input++;
            break;
        }
        char column_name[MAX_NAME_LENGTH];
        if(!read_word(&input, column_name, sizeof(column_name))) return 0;

        Datatype type;
        if(!parse_data_type(&input, &type)) return 0;

        if(!table_add_column(table, column_name, type)) return 0;

        skip_spaces(&input);

        if(*input == ','){
            input++;
            continue;
        }

        if(*input == ')'){
            input++;
            break;
        }

        return 0;
    }
    return (*table).column_count >0;


}

static int read_quoted_string(const char** input, char *buffer, int buffer_size){
    if(**input != '\''){
        return 0;
    }

    (*input)++;

    int length = 0;
    while(**input != '\0' && **input != '\''){
        if(length >= buffer_size - 1){
            return 0;
        }
        buffer[length++] = **input;
        (*input)++;
    }

    if(**input != '\''){
        return 0;
    }
    (*input)++;

    buffer[length] = '\0';
    return 1;

}

static int parse_value(const char **input, Datatype expected_type, Value *value){

    skip_spaces(input);

    (*value).type = expected_type;

    if(expected_type == TYPE_INT){

        char buffer[64];

        if(!read_word(input, buffer, sizeof(buffer))){
            return 0;
        }

        char *end;

        long parsed = strtol(buffer, &end, 10);
        if(*end != '\0'){
            return 0;
        }

        (*value).int_value = (int)parsed;

        return 1;
    }

    if(expected_type == TYPE_FLOAT){

        char buffer[64];

        if(!read_word(input, buffer, sizeof(buffer))){
            return 0;
        }

        char *end;

        float parsed = strtof(buffer, &end);
        if(*end != '\0'){
            return 0;
        }

        (*value).float_value = parsed;

        return 1;
    }

    if(expected_type == TYPE_TEXT){

        return read_quoted_string(input, (*value).text_value, MAX_TEXT_LENGTH);
    }
    return 0;


}


int parse_insert(const char* input, Table *table, Row **row){
    if(!match_keyword(&input, "INSERT")) return 0;
    if(!match_keyword(&input, "INTO")) return 0;

    char table_name[MAX_NAME_LENGTH];

    if(!read_word(&input, table_name, sizeof(table_name))) return 0;
    
    if(strcmp(table_name, (*table).name) != 0) return 0;

    if(!match_keyword(&input, "VALUES")) return 0;

    skip_spaces(&input);

    if(*input != '(') return 0;
    input++;

    Row *new_row = row_create((*table).column_count);

    if(new_row == NULL) return 0;

    for(int i = 0; i < (*table).column_count; i++){

        if(!parse_value(&input, (*table).columns[i].type, &new_row->values[i])){
            row_free(new_row);
            return 0;
        }

        skip_spaces(&input);

        if(i < (*table).column_count - 1){

            if(*input != ','){
                row_free(new_row);
                return 0;
            }

            input++;

        }else{
            if(*input != ')'){
                row_free(new_row);
                return 0;
            }
            input++;
        }
    }

    *row = new_row;

    return 1;
}
static int parse_operator(const char **input, Operator *operator){
    skip_spaces(input);

    if(strncmp(*input, ">=", 2) == 0){
        *operator = OP_GREATER_EQUAL;
        *input += 2;
        return 1;
    }

    if(strncmp(*input, "<=", 2) == 0){
        *operator = OP_LESS_EQUAL;
        *input += 2;
        return 1;
    }

    if(strncmp(*input, "!=", 2) == 0){
        *operator = OP_NOT_EQUAL;
        *input += 2;
        return 1;
    }

    if(**input == '='){
        *operator = OP_EQUAL;
        (*input)++;
        return 1;
    }

    if(**input == '>'){
        *operator = OP_GREATER;
        (*input)++;
        return 1;
    }

    if(**input == '<'){
        *operator = OP_LESS;
        (*input)++;
        return 1;
    }

    return 0;
}



static int parse_condition(const char **input, SelectQuery *query){

    if(!match_keyword(input, "WHERE")){
        return 0;
    }

    if(!read_word(input, (*query).condition_column, sizeof((*query).condition_column))){
        return 0;
    }

    if(!parse_operator(input, &(*query).condition_operator)){
        return 0;
    }

    skip_spaces(input);

    if(**input == '\''){
        if(!read_quoted_string(input, (*query).condition_value,sizeof((*query).condition_value))){
            return 0;
        }
        return 1;
    }

    
    if(!read_word(input,(*query).condition_value,sizeof((*query).condition_value))){
        return 0;
    }
    return 1;
}

int parse_select(const char *input, SelectQuery *query){
    memset(query, 0, sizeof(SelectQuery));

    if(!match_keyword(&input, "SELECT")) return 0;

    char column[MAX_NAME_LENGTH];

    if(!read_word(&input,column,sizeof(column))){
        return 0;
    }
    if(strcmp(column, "*") != 0){
        return 0;
    }

    if(!match_keyword(&input, "FROM")){
        return 0;
    }

    if(!read_word(&input,(*query).table_name,sizeof(query->table_name))){
        return 0;
    }

    (*query).type = QUERY_SELECT;
    (*query).select_all = 1;
    (*query).has_condition = 0;

    const char *remaining = input;

    skip_spaces(&remaining);
    if(*remaining != '\0'){
        if(!parse_condition(&remaining, query)){
            return 0;
        }

        (*query).has_condition = 1;
    }



    return 1;

}

