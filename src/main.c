#include "../include/text.h"

int main (){

    FILE *file_pointer ; 
    char input_text[100];

    printf("Rich Editor!\n");
    
    file_pointer = fopen("output.txt","w"); // Pointer creates the output file 

    while(file_pointer == NULL){
        exit(1);
    }

    fgets(input_text, sizeof(input_text),stdin); // for entire line of text (including whitespace!)
    fprintf(file_pointer, "%s", input_text); // writes to file 





    fclose(file_pointer);
    return 0;
}



/*
Implement further
*/