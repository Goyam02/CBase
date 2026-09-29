#include <stdio.h>
#include <string.h>

#include "cli.h"
#define INPUT_BUFFER_SIZE 1024

static void print_prompt(void){
    printf("cbase >");
}

static void print_help(void){
    printf("\n");
    printf("Cbase commands \n");
    printf("--------------\n");
    printf(".help   Show this help message\n");
    printf(".exit   Exit CBase\n");
    printf("\n");


}

void start_cli(void){
    char input[INPUT_BUFFER_SIZE];

    printf("A lightweight database engine written in C\n\n");

    while(1){
        print_prompt();

        if(fgets(input, sizeof(input), stdin) == NULL) break;

        input[strcspn(input, "\n")] = '\0';

        if(strcmp(input, ".exit") == 0){
            printf("bye \n");
            break;
        }
        if(strcmp(input, ".help") == 0){
            print_help();
            continue;
        }
        if(strlen(input) == 0){
            continue;
        
        }
        printf("Unknown command: %s\n", input);      

    }
}