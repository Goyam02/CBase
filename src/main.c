#include "cli.h"
#include "database.h"
#include "storage.h"

int main(void){

    Database database;

    database_init(&database);
    storage_load(&database);


    start_cli(&database);
    storage_save(&database);

    database_free(&database);

    return 0;
}
