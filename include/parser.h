#ifndef PARSER_H
#define PARSER_H

#include "table.h"
#include "executor.h"

int parse_create_table(const char* input, Table *table);

int parse_insert(const char* input, Table *table, Row **row);
int parse_select(const char *input, SelectQuery *query);
#endif

