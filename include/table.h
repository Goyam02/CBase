#ifndef TABLE_H
#define TABLE_H

#define MAX_TABLES 32
#define MAX_COLUMNS 32
#define MAX_NAME_LENGTH 32

typedef enum{
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_TEXT
} Datatype;

typedef struct{
    char name[MAX_NAME_LENGTH];
    Datatype type;
} Column;

typedef struct{
    char name[MAX_NAME_LENGTH];
    int column_count;
    Column columns[MAX_COLUMNS];

} Table;

void table_init(Table *table, const char *name);

int table_add_column(
    Table *table,
    const char *name,
    Datatype type
);

void table_print_schema(const Table *table);
#endif


