#ifndef STORAGE_H
#define STORAGE_H

#include<stdio.h>
#include "database.h"

#define PAGE_SIZE 4096
#define DATABASE_FILE "data/cbase.db"
#define CBASE_MAGIC 0x43424153
#define CBASE_VERSION 1


typedef struct{
    int magic;
    int version;
    int table_count;
}DatabaseHeader;


int storage_save(Database *database);
int storage_load(Database *database);
int storage_save_table(FILE *file, const Table *table);
int storage_load_table(FILE *file, Table *table);
int storage_save_value(FILE *file, const Value *value);
int storage_load_value(FILE *file, Value *value);
int storage_save_row(FILE *file, const Row *row);
int storage_load_row(FILE *file, Row *row);
#endif

