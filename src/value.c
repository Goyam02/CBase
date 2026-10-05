#include <stdio.h>
#include <string.h>

#include "value.h"

void print_value(const Value *value){
    switch((*value).type){

        case TYPE_INT:
            printf("%d", (*value).int_value);
            break;
        case TYPE_FLOAT:
            printf("%.2f", (*value).float_value);
            break;
        case TYPE_TEXT:
            printf("%s", (*value).text_value);
            break;   
        default:
            printf("NULL");
            break;         
    }
}
