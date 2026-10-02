#include <stdio.h>
#include <stdlib.h>

#include "row.h"


Row *row_create(int value_count){

    if(value_count <=0) return NULL;

    Row *row = malloc(sizeof(Row));

    if(row == NULL) return NULL;

    (*row).values = malloc(sizeof(Value)*value_count);

    if((*row).values == NULL){
        free(row);
        return NULL;
    }

    (*row).value_count = value_count;
    return row;


}


void row_free(Row *row){
    if(row == NULL) return;

    free((*row).values);
    free(row);

}

void row_print(const Row *row){
    if(row == NULL) return;

    for(int i = 0; i < (*row).value_count; i++){
        print_value(&(*row).values[i]);
        if(i < (*row).value_count - 1){
            printf(" | ");
        }
    }
    printf("\n");
}


