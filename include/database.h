#ifndef DATABASE_H
#define DATABASE_H

#include "table.h"

typedef struct
{
    int table_count;
    Table tables[MAX_TABLES];
} Database;

void database_init(Database *database);

Table *database_create_table(
    Database* database,
    const char* name
);

Table *database_find_table(
    Database *database,
    const char *name
);

void database_list_tables(
    const Database *database
);

int database_add_table(
    Database *database,
    const Table *table
);

#endif
