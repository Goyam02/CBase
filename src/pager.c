#include <stdio.h>
#include <stdlib.h>

#include "pager.h"

Pager *pager_open(const char *filename){

    FILE *file = fopen(filename, "r+b");

    if(file == NULL){
        file = fopen(filename, "w+b");
    }

    if(file == NULL){
        perror("Failed to open database file");
        return NULL;
    }
    Pager *pager = malloc(sizeof(Pager));

    if(pager == NULL){
        fclose(file);
        return NULL;
    }

    (*pager).file = file;
    fseek(file, 0, SEEK_END);

    long file_size = ftell(file);

    if(file_size < 0){
        fclose(file);
        free(pager);
        return NULL;
    }

    (*pager).page_count = (int)((file_size + PAGE_SIZE - 1) / PAGE_SIZE);
    return pager;
}

void pager_close(Pager *pager){

    if(pager == NULL){
        return;
    }

    fclose((*pager).file);
    free(pager);
}

unsigned char *pager_get_page(Pager *pager, int page_number){
    if(pager == NULL) return NULL;

    if(page_number < 0) return NULL;

    unsigned char *page = malloc(PAGE_SIZE);

    if(page == NULL) return NULL;
    

    long offset = (long)page_number * PAGE_SIZE;

    if(fseek((*pager).file, offset, SEEK_SET) != 0){
        free(page);
        return NULL;
    }

    size_t bytes_read = fread(page, 1, PAGE_SIZE, (*pager).file);

    if(bytes_read < PAGE_SIZE){

        for(size_t i = bytes_read; i < PAGE_SIZE; i++){
            page[i] = 0;
        }
    }

    return page;
}

int pager_write_page(Pager *pager,int page_number,const unsigned char *data){

    if(pager == NULL || data == NULL) return 0;

    if(page_number < 0) return 0;

    long offset = (long)page_number * PAGE_SIZE;

    if(fseek((*pager).file, offset, SEEK_SET) != 0) return 0;

    size_t written = fwrite(data, 1, PAGE_SIZE,(*pager).file);

    if(written != PAGE_SIZE) return 0;
    
    fflush((*pager).file);

    if(page_number >= (*pager).page_count){
        (*pager).page_count = page_number + 1;
    }

    return 1;
}
