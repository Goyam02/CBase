#include <stdio.h>
#include <string.h>

#include "executor.h"

int execute_select(Database *database, const SelectQuery *query){
    Table *table = database_find_table(database, (*query).table_name);

    if(table == NULL){
        printf("Table '%s' doesnt exist", (*query).table_name);
        return 0;
    }
    for(int i = 0; i < (*table).column_count;i++){
        printf("%s",(*table).columns[i].name);
        if (i < (*table).column_count - 1){
            printf(" | ");
        }
    }
    printf("\n");

    for(int i = 0;i < (*table).row_count;i++){
        row_print((*table).rows[i]);
    }
    return 1;
}
