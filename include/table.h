#ifndef TABLE_H
#define TABLE_H

#define MAX_TABLES 32
#define MAX_COLUMNS 32
#define MAX_NAME_LENGTH 32
#define MAX_ROWS 1000

#include "row.h"
#include "types.h"

typedef struct{
    char name[MAX_NAME_LENGTH];
    Datatype type;
} Column;

typedef struct{
    char name[MAX_NAME_LENGTH];
    int column_count;
    Column columns[MAX_COLUMNS];

    Row **rows;
    int row_count;
    int row_capacity;

} Table;

void table_init(Table *table, const char *name);

int table_add_column(
    Table *table,
    const char *name,
    Datatype type
);

int table_insert_row(Table *table, Row *row);

void table_print_schema(const Table *table);
void table_free(Table *table);
#endif




