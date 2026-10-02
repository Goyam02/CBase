#ifndef VALUE_H
#define VALUE_H

#include "types.h"

#define MAX_TEXT_LENGTH 256

typedef struct{

    Datatype type;

    union{
        int int_value;
        float float_value;
        char text_value[MAX_TEXT_LENGTH];

    };
}Value;

void print_value(const Value *value);



#endif

