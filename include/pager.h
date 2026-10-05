#ifndef PAGER_H
#define PAGER_H

#include <stdio.h>
#include "storage.h"

typedef struct{
    FILE *file;
    int page_count;
}Pager;

Pager *pager_open(const char *filename);

void pager_close(Pager *pager);

unsigned char *pager_get_page(Pager *pager, int page_number);

int pager_write_page(Pager *pager, int page_number, const unsigned char *data);

#endif
