#include "cli.h"
#include "database.h"

int main(void){

    Database database;

    database_init(&database);

    start_cli(&database);
    database_free(&database);

    return 0;
}
