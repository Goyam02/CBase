#ifndef PARSER_H
#define PARSER_H

#include "table.h"
#include "executor.h"

int parse_create_table(const char* input, Table *table);

int parse_insert(const char* input, Table *table, Row **row);
int parse_select(const char *input, SelectQuery *query);
int parse_update(const char *input, UpdateQuery *query);
int parse_delete(const char *input, DeleteQuery *query);
#endif

