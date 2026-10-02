#ifndef ROW_H
#define ROW_H

#include "value.h"

typedef struct {

    Value *values;
    int value_count;

}Row;


Row *row_create(int value_count);
void row_free(Row *row);
void row_print(const Row *row);

#endif

