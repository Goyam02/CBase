#include <stdio.h>
#include<string.h>
#include<ctype.h>

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

