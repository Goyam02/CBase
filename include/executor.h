#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "database.h"

typedef enum{
    QUERY_SELECT
}QueryType;

typedef struct{
    QueryType type;
    char table_name[MAX_NAME_LENGTH];
    int select_all;
}SelectQuery;

int execute_select(Database *database, const SelectQuery *query);

#endif