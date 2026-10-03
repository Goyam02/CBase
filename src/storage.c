#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "storage.h"

int storage_save(Database *database){
    FILE *file = fopen(DATABASE_FILE, "wb");

    if(file == NULL){
        perror("Failed to open database file");
        return 0;
        
    }
    DatabaseHeader header;
    header.magic = CBASE_MAGIC;
    header.version = CBASE_VERSION;
    header.table_count = (*database).table_count;
    if(fwrite(&header, sizeof(DatabaseHeader),1, file) != 1){
        fclose(file);
        return 0;

    }
    for(int i = 0; i < (*database).table_count; i++){
        if(!storage_save_table(file, &(*database).tables[i])){
            fclose(file);
            return 0;
    }
    }

    fclose(file);
    return 1;
}

int storage_load(Database *database){
    FILE *file = fopen(DATABASE_FILE, "rb");
    if(file == NULL) return 1;

    DatabaseHeader header;
    if(fread(&header, sizeof(DatabaseHeader), 1, file) != 1){
        fclose(file);
        return 0;
    }

    if(header.magic != CBASE_MAGIC){
        printf("Invalid CBase database file.\n");
        fclose(file);
        return 0;
    }
    if(header.version != CBASE_VERSION){
        printf("Unsupported database version.\n");
        fclose(file);
        return 0;
    }
    if(header.table_count < 0 || header.table_count > MAX_TABLES) {
        printf("Invalid table count.\n");
        fclose(file);
        return 0;
    }
    for(int i = 0; i < header.table_count; i++){
        if(!storage_load_table(file, &(*database).tables[i])){
            fclose(file);
            return 0;
        }
    }

    (*database).table_count = header.table_count;
    fclose(file);
    return 1;

}

int storage_save_table(FILE *file, const Table *table){
    if(fwrite((*table).name, sizeof((*table).name), 1, file) != 1){
        return 0;
    }
    if(fwrite(&(*table).column_count, sizeof(int), 1, file) != 1){
        return 0;
    }

    for(int i = 0; i < (*table).column_count; i++){
        if(fwrite((*table).columns[i].name, sizeof((*table).columns[i].name), 1, file) != 1){
            return 0;
        }

        if(fwrite(&(*table).columns[i].type, sizeof(Datatype), 1, file) != 1){
            return 0;
        }

    }
    if(fwrite(&(*table).row_count, sizeof(int), 1, file) != 1){
        return 0;
    }
    for(int i = 0; i < table->row_count; i++){
        if(!storage_save_row(file, (*table).rows[i])){
            return 0;
        }
    }
    return 1;
}

int storage_load_table(FILE *file, Table *table){
    char table_name[MAX_NAME_LENGTH];
    int column_count;

    if(fread(table_name, sizeof(table_name), 1,file) != 1){
        return 0;
    }

    if(fread(&column_count, sizeof(int), 1, file) != 1){
        return 0;
    }

    if (column_count < 1 || column_count > MAX_COLUMNS) {
        return 0;
    }

    table_init(table, table_name);

    for (int i = 0; i < column_count; i++) {
        char column_name[MAX_NAME_LENGTH];
        Datatype type;

        if(fread(column_name, sizeof(column_name), 1, file) != 1){
            return 0;
        }

        if (fread(&type, sizeof(Datatype), 1, file) != 1){
            return 0;
        }

        if(!table_add_column(table, column_name, type)){
            return 0;
        }
    }
    int row_count;
    if(fread(&row_count, sizeof(int), 1, file) != 1){
        return 0;
    }
    if(row_count < 0) return 0;
    if(row_count > MAX_ROWS) return 0;

    for(int i = 0; i < row_count; i++){
        Row *row = row_create((*table).column_count);

        if(row == NULL) return 0;
        if(!storage_load_row(file, row)){
        row_free(row);
        return 0;
        }

        if(!table_insert_row(table, row)){
        row_free(row);
        return 0;
        }

    }
    return 1;
}

int storage_save_value(FILE *file, const Value *value){
    
    if(fwrite(&(*value).type, sizeof(Datatype), 1, file) != 1) return 0;

    switch((*value).type){
        case TYPE_INT:
            if(fwrite(&(*value).int_value, sizeof(int), 1, file) != 1) return 0;
            break;
        case TYPE_FLOAT:
            if(fwrite(&(*value).float_value, sizeof(float), 1, file) != 1) return 0;
            break;

        case TYPE_TEXT:
            if(fwrite((*value).text_value, sizeof((*value).text_value), 1, file) != 1) return 0;
            break;

        default:
            return 0;
    }
    return 1;
}

int storage_load_value(FILE *file, Value *value){

    if(fread(&(*value).type, sizeof(Datatype), 1, file) != 1) return 0;
    switch((*value).type){
        case TYPE_INT:
            if(fread(&(*value).int_value, sizeof(int), 1, file) != 1) return 0;
            break;

        case TYPE_FLOAT:
            if(fread(&(*value).float_value, sizeof(float), 1, file) != 1) return 0;
            break;
        case TYPE_TEXT:
            if(fread((*value).text_value, sizeof((*value).text_value), 1, file) != 1) return 0;

            (*value).text_value[MAX_TEXT_LENGTH - 1] = '\0';
            break;
        default:
            return 0;
    }
    return 1;

}


int storage_save_row(FILE *file, const Row *row){
    for(int i = 0; i < (*row).value_count; i++){
        if(!storage_save_value(file, &(*row).values[i])) return 0;

    }
    return 1;
}


int storage_load_row(FILE *file, Row *row){
    for(int i = 0; i < (*row).value_count; i++){
        if(!storage_load_value(file, &(*row).values[i])) return 0;

    }
    return 1;
}

