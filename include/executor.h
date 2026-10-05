#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "database.h"

typedef enum{
    QUERY_SELECT,
    QUERY_UPDATE,
    QUERY_DELETE
}QueryType;

typedef enum {
    OP_EQUAL,
    OP_NOT_EQUAL,
    OP_LESS,
    OP_LESS_EQUAL,
    OP_GREATER,
    OP_GREATER_EQUAL
}Operator;

typedef struct{
    int has_condition;
    char column[MAX_NAME_LENGTH];
    Operator operator;
    char value[MAX_TEXT_LENGTH];

}Condition;

typedef struct{
    QueryType type;
    char table_name[MAX_NAME_LENGTH];
    int select_all;

    Condition condition;
    char condition_column[MAX_NAME_LENGTH];
    Operator condition_operator;
    char condition_value[MAX_TEXT_LENGTH];
}SelectQuery;

typedef struct{
    QueryType type;
    char table_name[MAX_NAME_LENGTH];
    char update_column[MAX_NAME_LENGTH];
    char update_value[MAX_NAME_LENGTH];

    Condition condition;
    // char condition_column[MAX_NAME_LENGTH];
    Operator condition_column;
    char condition_value[MAX_NAME_LENGTH];
}UpdateQuery;

typedef struct {
    QueryType type;
    char table_name[MAX_NAME_LENGTH];
    Condition condition;
}DeleteQuery;


int execute_select(Database *database, const SelectQuery *query);
int execute_update(Database *database, const UpdateQuery *query);
int execute_delete(Database *database, const DeleteQuery *query);


#endif
