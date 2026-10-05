#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "database.h"

typedef enum{
    QUERY_SELECT
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
    QueryType type;
    char table_name[MAX_NAME_LENGTH];
    int select_all;
    int has_condition;
    char condition_column[MAX_NAME_LENGTH];
    Operator condition_operator;
    char condition_value[MAX_TEXT_LENGTH];
}SelectQuery;



int execute_select(Database *database, const SelectQuery *query);

#endif
